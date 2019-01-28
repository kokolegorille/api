let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Member");
let make = _children => {
    ...component,
    render: _self => <div> <h2> (str("Member")) </h2> </div>,
};