/* https://medium.com/@sharifsbeat/reasonable-form-handling-d117d52ebc8a */

/* https://gist.github.com/busypeoples/62b12da2a2a662454f112c5f7581c9d8 */



type formTypes =
  | UserName
  | RepeatUserName;

type formState = {
  userName: string,
  repeatUserName: string,
};

module Configuration = {
  type state = formState;
  type field = formTypes;
  let update = (field, value, state) =>
    switch (field) {
    | UserName => {...state, userName: value}
    | RepeatUserName => {...state, repeatUserName: value}
    };
  let get = (field, state) =>
    switch (field) {
    | UserName => state.userName
    | RepeatUserName => state.repeatUserName
    };
};

let equalUserName = (value, values) => value === values.userName;

let emptyMsg = "Field is required";


let rules = [
  (UserName, [(CreateForm.NotEmpty, emptyMsg)]),
  (
    RepeatUserName,
    [
      (CreateForm.NotEmpty, emptyMsg),
      (CreateForm.Custom(equalUserName), "UserName and RepeatUserName have to be same"),
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

let getValue = event => ReactEvent.Form.target(event)##value;

let preventDefault = event => ReactEvent.Synthetic.preventDefault(event);

let component = ReasonReact.statelessComponent("Form");
let make = (~handleSubmit, _children) => {
  ...component,
  render: _self =>
    <SpecialForm
      initialState={userName: "", repeatUserName: ""}
      rules
      render=(
        ({form, handleChange}) =>
          <form
            onSubmit=(
              e => {
                preventDefault(e);
                handleSubmit(form.values);
              }
            )>
            <label>
              (str("UserName: "))
              <br />
              <input
                value=form.values.userName
                onChange=(e => getValue(e) |> handleChange(UserName))
              />
            </label>
            <p> (getError(UserName, form.errors)) </p>
            <label>
              (str("Repeat UserName: "))
              <br />
              <input
                value=form.values.repeatUserName
                onChange=(e => getValue(e) |> handleChange(RepeatUserName))
              />
            </label>
            <p> (getError(RepeatUserName, form.errors)) </p>
            <br />
            <button> (str("Submit")) </button>
          </form>
      )
    />,
};



/* 
type validation('a) =
  | NotEmpty
  | Custom('a);

type t = string;

let validate = (rule, value, values) =>
  switch (rule) {
  | NotEmpty => String.length(value) > 0
  | Custom(fn) => fn(value, values)
  };

let validateField = (validations, value, values) =>
  List.fold_left(
    (errors, (fn, msg)) =>
      validate(fn, value, values) ? errors : List.append(errors, [msg]),
    [],
    validations,
  );

let validation = (rules, get, data) =>
  List.fold_left(
    (errors, (field, validations)) =>
      {
        let value = get(field, data);
        validateField(validations, value, data);
      }
      |> (
        fieldErrors =>
          List.length(fieldErrors) > 0 ?
            List.append(errors, [(field, fieldErrors)]) : errors
      ),
    [],
    rules,
  );

module type Config = {
  type state;
  type field;
  let update: (field, t, state) => state;
  let get: (field, state) => t;
};

module FormComponent = (Config: Config) => {
  type field = Config.field;
  type values = Config.state;
  type errors = list((field, list(string)));
  type state = {
    errors,
    values,
  };
  type form = {
    form: state,
    handleChange: (field, t) => unit,
  };
  type action =
    | UpdateValues(field, t);
  let component = ReasonReact.reducerComponent("FormComponent");
  let make = (~initialState, ~rules, ~render, _children) => {
    ...component,
    initialState: () => {errors: [], values: initialState},
    reducer: (action, state) =>
      switch (action) {
      | UpdateValues(name, value) =>
        let values = Config.update(name, value, state.values);
        ReasonReact.Update({
          values,
          errors: validation(rules, Config.get, values),
        });
      },
    render: ({state, send}) => {
      let handleChange = (field, value) => send(UpdateValues(field, value));
      render({form: state, handleChange});
    },
  };
};

type formTypes =
  | UserName
  | RepeatUserName;

type formState = {
  userName: string,
  repeatUserName: string,
};

module Configuration = {
  type state = formState;
  type field = formTypes;
  let update = (field, value, state) =>
    switch (field) {
    | UserName => {...state, userName: value}
    | RepeatUserName => {...state, repeatUserName: value}
    };
  let get = (field, state) =>
    switch (field) {
    | UserName => state.userName
    | RepeatUserName => state.repeatUserName
    };
};

let equalUserName = (value, values) => value === values.userName;

let emptyMsg = "Field is required";

let rules = [
  (UserName, [(NotEmpty, emptyMsg)]),
  (
    RepeatUserName,
    [
      (NotEmpty, emptyMsg),
      (Custom(equalUserName), "UserName and RepeatUserName have to be same"),
    ],
  ),
];

module SpecialForm = FormComponent(Configuration);

let se = ReasonReact.string;

let first = list => List.length(list) > 0 ? Some(List.hd(list)) : None;

let getError = (field, errors) =>
  List.filter(((name, _)) => name === field, errors)
  |> first
  |> (
    errors =>
      switch (errors) {
      | Some((_, msgs)) => se(List.hd(msgs))
      | None => ReasonReact.null
      }
  );

let getValue = event => ReactEvent.Form.target(event)##value;

let preventDefault = event => ReactEvent.Synthetic.preventDefault(event);

let component = ReasonReact.statelessComponent("Form");
let make = (~handleSubmit, _children) => {
  ...component,
  render: _self =>
    <SpecialForm
      initialState={userName: "", repeatUserName: ""}
      rules
      render=(
        ({form, handleChange}) =>
          <form
            onSubmit=(
              e => {
                preventDefault(e);
                handleSubmit(form.values);
              }
            )>
            <label>
              (se("UserName: "))
              <br />
              <input
                value=form.values.userName
                onChange=(e => getValue(e) |> handleChange(UserName))
              />
            </label>
            <p> (getError(UserName, form.errors)) </p>
            <label>
              (se("Repeat UserName: "))
              <br />
              <input
                value=form.values.repeatUserName
                onChange=(e => getValue(e) |> handleChange(RepeatUserName))
              />
            </label>
            <p> (getError(RepeatUserName, form.errors)) </p>
            <br />
            <button> (se("Submit")) </button>
          </form>
      )
    />,
}; */
