
type page =
  | Home
  | Users
  | Channels
  | About
  | NotFound;

module Config = {
  type route = page;
  let toRoute = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | []           => Home
    | ["users"]    => Users
    | ["channels"] => Channels
    | ["about"]    => About
    | _            => NotFound
    };
  let toUrl = route =>
    switch (route) {
    | Home     => "/"
    | Users    => "/users"
    | Channels => "channels"
    | About    => "/about"
    | _        => "/not_found"
    };
};

module Router = CreateRouter.Make(Config);

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,

  render: _self => {
    <div>
      <header>
        <nav role="navigation">
          <ul>
            <li><Link href=(Config.toUrl(Home)) label=(() => str("Home")) /></li>
            <li><Link href=(Config.toUrl(Users)) label=(() => str("Users")) /></li>
            <li><Link href=(Config.toUrl(Channels)) label=(() => str("Channels")) /></li>
            <li><Link href=(Config.toUrl(About)) label=(() => str("About")) /></li>
          </ul>
        </nav>
      </header>
      <main role="main" className="container">
        <Router 
          render=(({route}) => 
            switch route {
            | Home => <Home />
            | Users => <Users />
            | Channels => <Channels />
            | About => <About />
            | NotFound => <NotFound />
            }
          )
        />
      </main>
    </div>
  },
};
