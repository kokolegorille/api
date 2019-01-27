/* FUNCTOR */

let str = ReasonReact.string;

module Make = (Config: {type data}) => {
  type state = 
    | Loading
    | Failure
    | Success(Config.data);
  type action = 
    | Load
    | Loaded(Config.data)
    | Failed;
  let component = ReasonReact.reducerComponent("FetcherComponent");
  let make = (~fetch, ~failedMessage, ~render, _children) => {
    ...component,
    initialState: () => Loading,
    reducer: (action, _state) =>
      switch (action) {
      | Load => 
          ReasonReact.UpdateWithSideEffects(
            Loading,
            (
              self =>
                Js.Promise.(
                  fetch()
                  |> then_(result => 
                    switch result {
                    | Some(data) => resolve(self.send(Loaded(data)))
                    | None => resolve(self.send(Failed))
                    }
                  )
                  |> ignore
                )
            )
          );
      | Loaded(data) => ReasonReact.Update(Success(data))
      | Failed => ReasonReact.Update(Failure)
      },
    didMount: self => self.send(Load),
    render: ({state}) => {
      switch (state) {
      | Loading => <div>(str("Loading..."))</div>
      | Failure => <div>(str(failedMessage))</div>
      | Success(data) => render(data)
      }
    }
  };
};