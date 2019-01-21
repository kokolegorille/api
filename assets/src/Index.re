[%bs.raw {|require("../css/app.scss")|}];

module App = {
  let component = ReasonReact.statelessComponent("About");
  let make = _children => {
    ...component,
    render: _self => <div> <h2> (ReasonReact.string("Yo! c moi Meli7 :-) la babibouchette!")) </h2> </div>,
  };
}

ReactDOMRe.renderToElementWithId(<App />, "app"); 