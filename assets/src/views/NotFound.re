let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("NotFound");
let make = _children => {
    ...component,
    render: _self => <div> <h2> (str("NotFound")) </h2> </div>,
};