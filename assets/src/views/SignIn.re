type formTypes =
  | Name
  | Password;

type formState = {
  name: string,
  password: string,
};

module Configuration = {
  type state = formState;
  type field = formTypes;
  let update = (field, value, state) =>
    switch (field) {
    | Name => {...state, name: value}
    | Password => {...state, password: value}
    };
  let get = (field, state) =>
    switch (field) {
    | Name => state.name
    | Password => state.password
    };
};

let minLengthPassord = (value, _values) => String.length(value) >= 6;

let emptyMsg = "Field is required";

let rules = [
  (Name, [(CreateForm.NotEmpty, emptyMsg)]),
  (
    Password,
    [
      (CreateForm.NotEmpty, emptyMsg),
      (CreateForm.Custom(minLengthPassord), "Password length must be equal or larger than 6"),
    ],
  ),
];

module SpecialForm = CreateForm.Make(Configuration);

let str = ReasonReact.string;

let first = list => List.length(list) > 0 ? Some(List.hd(list)) : None;

let getError = (field, errors) =>
  List.filter(((name, _)) => name === field, errors)
  |> first
  |> (
    errors =>
      switch (errors) {
      | Some((_, msgs)) => str(List.hd(msgs))
      | None => ReasonReact.null
      }
  );

let getInputClass = (field, errors) : string => {
  List.filter(((name, _)) => name === field, errors)
  |> first
  |> (
    errors =>
      switch (errors) {
      | Some((_, _msgs)) => "form-control is-invalid"
      | None => "form-control"
      }
  );
};

let getValue = event => ReactEvent.Form.target(event)##value;

let preventDefault = event => ReactEvent.Synthetic.preventDefault(event);

/* IMPORT ABSTRACT TYPES */

type user = Abstract.User.t;
type sessionData = Abstract.SessionData.t;

type state = 
  | Display
  | DisplayWithErrors(string)
  | Loading;

type action = 
  | Load(SpecialForm.state)
  | Loaded(sessionData)
  | Failed(string);

let component = ReasonReact.reducerComponent("Form");
let make = (~handleSubmit, _children) => {
  ...component,
  initialState: () => Display,
  reducer: (action, _state) => {
    switch (action) {
    | Load(data) => {
      let {name: name, password: password} = data.values;

      ReasonReact.UpdateWithSideEffects(
        Loading,
        self => 
          Js.Promise.(
            Api.signIn({"session": {"name": name, "password": password}})
            |> then_(result => 
              switch (result) {
                | Some(sessionData) =>
                  resolve(self.send(Loaded(sessionData)))
                | None => 
                  resolve(self.send(Failed("Wrong combination of name/password!")))
                }
            )
            |> ignore
          )
      )
    }
    | Loaded(sessionData) => {
      handleSubmit(sessionData);
      ReasonReact.Update(Display)
    }
    | Failed(error) => ReasonReact.Update(DisplayWithErrors(error));
    };
  },
  render: self =>
    <div>
      {
        switch (self.state) {
        | Display => ReasonReact.null
        | DisplayWithErrors(error) => <p>(str(error))</p>
        | Loading => <p>(str("Loading..."))</p>
        }
      }
      <SpecialForm
        initialState={name: "", password: ""}
        rules
        render=(
          ({form, handleChange}) =>
            <form
              onSubmit=(
                e => {
                  preventDefault(e);
                  self.send(Load(form));
                }
              )>
              <div className="form-group">
                <label htmlFor="userName">(str("Name: "))</label>
                <input
                  id="userName"
                  placeholder="Name"
                  value=form.values.name
                  onChange=(e => getValue(e) |> handleChange(Name))
                  className=(getInputClass(Name, form.errors))
                  required=true
                />
                <div className="invalid-feedback"> (getError(Name, form.errors)) </div>
              </div>
              <div className="form-group">
                <label htmlFor="userPassword">(str("Password: "))</label>
                <input
                  id="userPassword"
                  placeholder="Password"
                  type_="password"
                  value=form.values.password
                  onChange=(e => getValue(e) |> handleChange(Password))
                  className=(getInputClass(Password, form.errors))
                  required=true
                />
                <div className="invalid-feedback"> (getError(Password, form.errors)) </div>
              </div>
              <button className="btn btn-primary"> (str("Submit")) </button>
            </form>
        )
      />
    </div>,
};



/* 
type signInParams = {
  name: string,
  password: string
};

type user = {
  name: string,
  email: string
};

type authentication = {
  user: user,
  token: string
};

type state =
  | DisplayForm
  | Loading;
  /* | Failure
  | Success; */

type action = 
  /* | Submit(signInParams)
  | SignedIn(authentication) */
  | Failed 
  | Click;

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("SignIn");

let make = _children => {
  ...component,
  initialState: () => DisplayForm,
  reducer: (action, _state) => 
    switch (action) {
    | Click => {
      Js.log("YOLO!");
      ReasonReact.NoUpdate
    }
    | Failed => 
      Js.log("YILING!");
      ReasonReact.Update(Loading)
    },
  render: self => {
    (
      <form> 
        <h2> (str("SignIn")) </h2> 
        <div className="form-group">
          <input type_="name" placeholder="Username" autoFocus=true className="form-control"/>
        </div>
        <div className="form-group">
          <input type_="password" placeholder="Password" className="form-control"/>
        </div>
        <button 
          onClick=(e => {
            ReactEvent.Mouse.preventDefault(e);
            self.send(Click)
          })
        >
          (str("Submit"))
        </button>
        <button 
          onClick=(e => {
            ReactEvent.Mouse.preventDefault(e);
            self.send(Failed)
          })
        >
          (str("Failed"))
        </button>
      </form>
    )
    
  },
}; */