let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Home");
let make = _children => {
    ...component,
    render: _self => <div> <h2> (str("Home")) </h2> </div>,
};