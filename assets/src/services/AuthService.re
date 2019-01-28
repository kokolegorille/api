/* This cannot be run outside of the browser! */

let localStorageNamespace = "phoenixApi/reasonmlClient@authToken";

let loadToken = () =>
  Dom.Storage.(localStorage |> getItem(localStorageNamespace));

let saveToken = token => 
  Dom.Storage.(localStorage |> setItem(localStorageNamespace, token));

let removeToken = () => 
  Dom.Storage.(localStorage |> removeItem(localStorageNamespace));

let isLoggedIn = () => 
  switch(loadToken()) {
  | None => false
  | Some(_) => true
  };