let component = ReasonReact.statelessComponent("Link");
let make = (~href, ~label, _children) => {
    ...component,
    render: _self => {
    let onClick = e => {
        ReactEvent.Mouse.preventDefault(e);
        ReasonReact.Router.push(href);
    };
    <a href onClick> (label()) </a>;
    },
};