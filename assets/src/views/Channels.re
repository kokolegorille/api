
module Socket = {
  /* open Abstract; */
  type function_ = unit => unit
  type any = Js_json.t;
  type t = {
    .
    [@bs.meth] "protocol": unit => string,
    [@bs.meth] "endPointURL": unit => string,
    [@bs.meth] "log": (string, string, any) => unit,
    [@bs.meth] "hasLogger": unit => bool,
    [@bs.meth] "onOpen": function_ => unit,
    [@bs.meth] "onClose": function_ => unit,
    [@bs.meth] "onError": function_ => unit,
    [@bs.meth] "onMessage": function_ => unit,
    [@bs.meth] "connectionState": unit => string,
    [@bs.meth] "isConnected": unit => bool,
  };
  type endPoint = string;

  [@bs.module "phoenix"][@bs.new]
  external init : (endPoint, ~opts: Js.t('opts)=?, unit) => t = "Socket";
  let init = (~opts: option(Js.t('opts))=?, path) => init(path, ~opts?, ());

  [@bs.send]
  external disconnect :
    (t, ~callback: function_=?, ~code: string=?, ~reason: Js.t('reason)=?, unit) => unit =
    "disconnect";

  let disconnect =
    (
      ~callback: option(function_)=?,
      ~code: option(string)=?,
      ~reason: option(Js.t('reason))=?,
      socket
    ) => {
    let _ = disconnect(~callback?, ~code?, ~reason?, socket, ());
    socket;
  };

  [@bs.send] external connect : (t, ~params: Js.t('params)=?, unit) => unit = "connect";

  let connect = (~params: option(Js.t('params))=?, socket) => {
    let _ = connect(~params?, socket, ());
    socket;
  };
};

/* 
let opts = [%bs.obj {
  timeout: 10000,
  params: {
    userToken: "123"
  }
}]; 
*/

let opts = [%bs.obj {
  timeout: 10000,
  logger: (kind, msg, data) => Js.log(kind ++ ": " ++ msg ++ ", " ++ data)
}];

let mySocket = Socket.init(~opts=opts, "/socket");

Socket.connect(mySocket);

Js.log(mySocket);

Js.log(mySocket##protocol());

Js.log(mySocket##endPointURL());

Js.log(mySocket##isConnected());

/* 
Socket.disconnect(mySocket);

Js.log(mySocket); 
*/


/* 
[@bs.val][@bs.scope "Math"] external random : unit => float = "";
Js.log(random()); 

let myMap = Js.Dict.empty();
Js.Dict.set(myMap, "Allison", 10);

Js.log(myMap); 

/* Wrapping globals */

let dirname: option(string) = [%bs.node __dirname];
let filename: option(string) = [%bs.node __filename];
let _module: option(Node.node_module) = [%bs.node _module];

Js.log(dirname);
Js.log(filename);
Js.log(_module);

/* Building object with optional options */
module Make = {
  type socketOptions;
  [@bs.obj]
  external makeOptions : (
    ~ping: int=?,
    ~pong: int=?,
    unit
  ) => socketOptions = "";
} 

let options = Make.makeOptions(~ping=10, ());
Js.log(options);
*/

/* React */

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Channels");
let make = _children => {
    ...component,
    render: _self => <div> <h2> (str("Channels")) </h2> </div>,
};