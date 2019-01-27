
type page =
  | Home
  | Users
  | Channels
  | SignIn
  | Forms
  | About
  | NotFound;

module Config = {
  type route = page;
  let toRoute = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | []           => Home
    | ["users"]    => Users
    | ["channels"] => Channels
    | ["sign_in"]  => SignIn
    | ["forms"]    => Forms
    | ["about"]    => About
    | _            => NotFound
    };

  let toUrl = route =>
    switch (route) {
    | Home     => "/"
    | Users    => "/users"
    | Channels => "channels"
    | SignIn   => "/sign_in"
    | Forms    => "/forms"
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
        <nav role="navigation" className="navbar navbar-expand navbar-dark bg-dark">
          <a className="navbar-brand" href="#">(str("Phoenix ReasonML client"))</a>
          <div>
            <ul className="navbar-nav">
              <li className="nav-item"><Link href=(Config.toUrl(Home)) label=(() => str("Home")) /></li>
              <li className="nav-item"><Link href=(Config.toUrl(Users)) label=(() => str("Users")) /></li>
              <li className="nav-item"><Link href=(Config.toUrl(Channels)) label=(() => str("Channels")) /></li>
              <li className="nav-item"><Link href=(Config.toUrl(SignIn)) label=(() => str("SignIn")) /></li>
              <li className="nav-item"><Link href=(Config.toUrl(Forms)) label=(() => str("Forms")) /></li>
              <li className="nav-item"><Link href=(Config.toUrl(About)) label=(() => str("About")) /></li>
            </ul>
          </div>
        </nav>
      </header>
      <main role="main" className="container">
        <Router 
          render=(({route}) => 
            switch route {
            | Home => <Home />
            | Users => <Users />
            | Channels => <Channels />
            | SignIn => <SignIn handleSubmit=(values => Js.log(values)) />
            | Forms => <Forms handleSubmit=(values => Js.log(values)) />
            | About => <About />
            | NotFound => <NotFound />
            }
          )
        />
      </main>
    </div>
  },
};
