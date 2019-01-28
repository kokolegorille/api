let currentUser: (Js_json.t) => Abstract.User.t = json =>
  Json.Decode.{
    id: field("id", int, json),
    name: field("name", string, json),
  };

let session: (Js_json.t) => Abstract.SessionData.t = json =>
  Json.Decode.{
    currentUser: field("user", currentUser, json),
    token: field("token", string, json),
  };