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

/* let presence: (Js_json.t) => Abstract.PresenceData.t = json =>
  Json.Decode.{
    count: field("count", int, json),
    id: field("id", int, json),
    online_at: field("online_at", int, json),
    phx_ref: field("phx_ref", string, json),
  }; */
