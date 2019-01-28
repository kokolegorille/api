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
