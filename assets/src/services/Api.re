let apiRoot = "http://localhost:4000/api/v1";

/* 
authHeaders should be build with the makeConfig helper
https://github.com/meafmira/bs-axios/blob/master/src/axios.re
*/
let authConfig = token => {
  /* DO NOT ADD KEY headers: IT IS ADDED BY MAKECONFIG! */
  let headers = {
    "Accept": "application/json",
    "Content-Type": "application/json",
    "Authorization": {j|Bearer $token|j},
  };
  Axios.makeConfig(~headers=headers, ~withCredentials=true, ());
};

let signIn = data =>
  Js.Promise.(
    Axios.postData(apiRoot ++ "/authentication", data)
    |> then_(
      response => response##data 
        |> Decode.session 
        |> (session => Some(session) |> Js.Promise.resolve)
    )
    |> catch(_err => resolve(None))
  );

let signUp = data =>
  Js.Promise.(
    Axios.postData(apiRoot ++ "/registration", data)
    |> then_(
      response => response##data 
        |> Decode.session 
        |> (session => Some(session) |> Js.Promise.resolve)
    )
    |> catch(_err => resolve(None))
  );

let refreshToken = token => 
  Js.Promise.(
    Axios.patchDatac(apiRoot ++ "/authentication/refresh", {"session": "session"}, authConfig(token))
    |> then_(
      response => response##data 
        |> Decode.session 
        |> (session => Some(session) |> Js.Promise.resolve)
    )
    |> catch(_err => resolve(None))
  );

