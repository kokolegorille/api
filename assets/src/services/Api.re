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

external promiseErrorToJsObj : Js.Promise.error => Js.t('a) = "%identity";

let signIn = data =>
  Js.Promise.(
    Axios.postData(apiRoot ++ "/authentication", data)
    |> then_(
      response => response##data 
        |> Decode.session 
        |> (session => Belt.Result.Ok(session) |> Js.Promise.resolve)
    )
    |> catch(error => {
      /* 
      Check https://github.com/meafmira/bs-axios for error handling
       */
      let error = error |> promiseErrorToJsObj;
      let errorMsg = error##response##data##error;
      resolve(Belt.Result.Error(errorMsg));
    })
  ); 
  

let signUp = data =>
  Js.Promise.(
    Axios.postData(apiRoot ++ "/registration", data)
    |> then_(
      response => response##data 
        |> Decode.session 
        |> (session => Belt.Result.Ok(session) |> Js.Promise.resolve)
    )
    |> catch(error => {
      let error = error |> promiseErrorToJsObj;
      /* 
      The error.response.data is a js.t with this shape...
      It comes from changeset error!
      {
        errors: [
          key: value
        ]
      }
       */
      let errorObj = error##response##data;
      Js.log(errorObj);
      resolve(Belt.Result.Error(errorObj));
    })
  );

let refreshToken = token => 
  Js.Promise.(
    Axios.patchDatac(apiRoot ++ "/authentication/refresh", {"session": "session"}, authConfig(token))
    |> then_(
      response => response##data 
        |> Decode.session 
        |> (session => Belt.Result.Ok(session) |> Js.Promise.resolve)
    )
    |> catch(error => {
      let error = error |> promiseErrorToJsObj;
      let errorMsg = error##response##data##error;
      resolve(Belt.Result.Error(errorMsg));
    })
  );

