module User = {
  type t = {
    id: int,
    name: string
  };  
};

module SessionData = {
  type t = {
    currentUser: User.t, 
    token: string,
  };
};

module PresenceData = {
  type t = {
    count: int,
    id: int,
    online_at: int,
    phx_ref: string,
  }
}