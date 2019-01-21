/* FUNCTOR */

module type Config = {
    type route;
    let toUrl: route => string;
    let toRoute: ReasonReact.Router.url => route;
  };
  
  module Make = (Config: Config) => {
    type route = Config.route;
    type renderProps = {
      updateRoute: route => unit,
      route,
    };
    type state = {route};
    type action =
      | UpdateRoute(route);
    let component = ReasonReact.reducerComponent("Router");
    let make = (~render, _children) => {
      ...component,
        initialState: () => {
        route: ReasonReact.Router.dangerouslyGetInitialUrl()
          |> Config.toRoute,
      },
      reducer: (action, _state) =>
        switch (action) {
        | UpdateRoute(route) => ReasonReact.Update({route: route})
        },
      didMount: self => {
        let watcherID = ReasonReact.Router.watchUrl(url => 
          self.send(UpdateRoute(Config.toRoute(url)))
        );
        self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
      },
      render: self =>
        render({
          updateRoute: route => self.send(UpdateRoute(route)),
          route: self.state.route,
        }),
    };
  };
      