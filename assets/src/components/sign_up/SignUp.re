type formTypes =
  | Name
  | Email
  | Password;

type formState = {
  name: string,
  email: string,
  password: string,
};

module Configuration = {
  type state = formState;
  type field = formTypes;
  let update = (field, value, state) =>
    switch (field) {
    | Name => {...state, name: value}
    | Email => {...state, email: value}
    | Password => {...state, password: value}
    };
  let get = (field, state) =>
    switch (field) {
    | Name => state.name
    | Email => state.email
    | Password => state.password
    };
};

let minLengthPassord = (value, _values) => String.length(value) >= 6;
let validateEmail = (value, _values) => 
  /* Do not put // in the regex! */
  Js.Re.fromString("^([^@\s]+)@((?:[-a-z0-9]+\.)+[a-z]{2,})$")
  |> Js.Re.test(value);

let emptyMsg = "Field is required";

let rules = [
  (Name, [(CreateForm.NotEmpty, emptyMsg)]),
  (
    Email,
    [
      (CreateForm.NotEmpty, emptyMsg),
      (CreateForm.Custom(validateEmail), "Email is not valid."),
    ],
  ), 
  (
    Password,
    [
      (CreateForm.NotEmpty, emptyMsg),
      (CreateForm.Custom(minLengthPassord), "Password length must be equal or larger than 6."),
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
      let {name: name, email: email, password: password} = data.values;

      ReasonReact.UpdateWithSideEffects(
        Loading,
        self => 
          Js.Promise.(
            Api.signUp({"user": {"name": name, "email": email, "password": password}})
            |> then_(result => 
              switch (result) {
                | Belt.Result.Ok(sessionData) =>
                  resolve(self.send(Loaded(sessionData)))
                | Belt.Result.Error(_errorObj) => 
                  resolve(self.send(Failed("Could not create account.")))
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
        initialState={name: "", email: "", password: ""}
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
                <label htmlFor="registerUserName">(str("Name: "))</label>
                <input
                  id="registerUserName"
                  placeholder="Name"
                  value=form.values.name
                  onChange=(e => getValue(e) |> handleChange(Name))
                  className=(getInputClass(Name, form.errors))
                  required=true
                />
                <div className="invalid-feedback"> (getError(Name, form.errors)) </div>
              </div>
              <div className="form-group">
                <label htmlFor="registerEmail">(str("Email: "))</label>
                <input
                  id="registerUserEmail"
                  placeholder="Email"
                  value=form.values.email
                  onChange=(e => getValue(e) |> handleChange(Email))
                  className=(getInputClass(Email, form.errors))
                  required=true
                />
                <div className="invalid-feedback"> (getError(Email, form.errors)) </div>
              </div>
              <div className="form-group">
                <label htmlFor="registerUserPassword">(str("Password: "))</label>
                <input
                  id="registerUserPassword"
                  placeholder="Password"
                  type_="password"
                  value=form.values.password
                  onChange=(e => getValue(e) |> handleChange(Password))
                  className=(getInputClass(Password, form.errors))
                  required=true
                />
                <div className="invalid-feedback"> (getError(Password, form.errors)) </div>
              </div>
              <button className="btn btn-primary"> (str("Sign Up")) </button>
            </form>
        )
      />
    </div>,
};