let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("About");
let make = _children => {
    ...component,
    render: _self => <div> <h2> (str("About")) </h2> </div>,
};