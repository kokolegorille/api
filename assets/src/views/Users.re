
type user = {
	id: int,
	name: string,
	email: string,
};

type state = 
	| NotAsked
	| Loading
	| Failure
	| Success(list(user));

type action =
	| LoadUsers
	| LoadedUsers(list(user))
	| LoadUsersFailed;

module Decode = {
	let user = json =>
		Json.Decode.{
			id: field("id", int, json),
			name: field("name", string, json),
			email: field("email", string, json),
		};
	let users = json => 
    Json.Decode.(field("data", list(user), json));
};

let url = "http://localhost:4000/api/v1/users";

/* let fetchUsers = () =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.json)
    |> then_(json => json |> Decode.users |> (users => Some(users) |> resolve))
    |> catch(_err => resolve(None))
  ); */

let fetchUsers = () =>
  Js.Promise.(
    Axios.get(url)    
    |> then_(
      response => response##data 
        |> Decode.users 
        |> (users => Some(users) |> Js.Promise.resolve)
    )
    |> catch(_err => resolve(None))
  );

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("Users");

let make = _children => {
  ...component,
	initialState: () => NotAsked,
	reducer: (action, _state) =>
    switch (action) {
    | LoadUsers =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              fetchUsers()
              |> then_(result => 
								switch (result) {
									| Some(users) =>
										 resolve(self.send(LoadedUsers(users)))
									| None => resolve(self.send(LoadUsersFailed))
									}
							)
              |> ignore
            )
        ),
      )
    | LoadedUsers(users) => ReasonReact.Update(Success(users))
    | LoadUsersFailed => ReasonReact.Update(Failure)
		},
	render: self =>
    switch (self.state) {
    | NotAsked =>
      <div>
        (str("Click to start load Users"))
        <button onClick=(_event => self.send(LoadUsers))>
          (str("Load Users"))
        </button>
      </div>
    | Loading => <div> (str("Loading...")) </div>
    | Failure => <div> (str("Something went wrong!")) </div>
    | Success(users) =>
      <div>
        <h2> (str("Users")) </h2>
        <ul>
          (
            users
            |> List.map(user =>
                 <li key=(string_of_int(user.id))> 
                   (str(user.name)) 
                 </li>
               )
            |> Array.of_list
            |> ReasonReact.array
          )
        </ul>
      </div>
    },
};
