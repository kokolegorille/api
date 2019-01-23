

module Presence = {
  type t = {
    .
  };
};

module Push = {
  type any = Js_json.t;
  type t = {
    .
    [@bs.meth] "resend": float => unit,
    [@bs.meth] "send": unit,
    [@bs.meth] "receive": (string, any => unit) => t,
  };
};

module Channel = {
  type function_ = unit => unit;
  type any = Js_json.t;
  type ref = int;
  type t = {
    .
    [@bs.meth] "rejoinUntilConnected": function_ => unit,
    [@bs.meth] "onClose": function_ => unit,
    [@bs.meth] "onError": function_ => unit,
    [@bs.meth] "on": (string, any => unit) => ref,
    [@bs.meth] "off": (string, ref) => unit,
    [@bs.meth] "onMessage": (string, any, any) => any,
  };

  [@bs.send] 
  external join : (t, ~timeout: float=?, unit) => Push.t = "join";
  let join = (~timeout: option(float)=?, channel) => join(channel, ~timeout?, ());

  [@bs.send] 
  external leave : (t, ~timeout: float=?, unit) => Push.t = "leave";
  let leave = (~timeout: option(float)=?, channel) => leave(channel, ~timeout?, ());

  [@bs.new] [@bs.module "phoenix"]
  external initPush : (t, string, Js.t('payload), float) => Push.t = "Push";
  let initPush = (event, payload, timeout, channel) => initPush(channel, event, payload, timeout);

  [@bs.send]
  external push : (t, string, Js.t('payload), ~timeout: float=?, unit) => Push.t = "push";
  let push = (event, payload, ~timeout=?, channel) => push(channel, event, payload, ~timeout?, ());
};

module Socket = {
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

  [@bs.send] 
  external connect : (t, ~params: Js.t('params)=?, unit) => unit = "connect";
  let connect = (~params: option(Js.t('params))=?, socket) => {
    let _ = connect(~params?, socket, ());
    socket;
  };

  [@bs.new] [@bs.module "phoenix"]
  external initChannel : (string, ~params: Js.t('params)=?, ~socket: t=?, unit) => Channel.t = "Channel";
  let initChannel = (~params=?, ~socket=?, topic) => initChannel(topic, ~params?, ~socket?, ());

  [@bs.send]
  external channel : (t, string, ~chanParams: Js.t('chanParams)=?, unit) => Channel.t = "channel";
  let channel = (topic, ~chanParams=?, socket) => channel(socket, topic, ~chanParams?, ());
};

/* ============================================================== */
/* START SOCKET TESTS */
/* ============================================================== */

let socketOpts = [%bs.obj {
  timeout: 10000,
  logger: (kind, msg, data) => Js.log(kind ++ ": " ++ msg ++ ", " ++ data)
}];

let mySocket = Socket.init(~opts=socketOpts, "/socket");

Socket.connect(mySocket);

mySocket##onOpen(() => {
  let handleReceive = (event, any) =>
    switch event {
    | "ok" => Js.log(("handleReiceive:" ++ event, "Joined"))
    | "error" => Js.log(("handleReiceive:" ++ event, "Failed to join channel"))
    | _ => Js.log(("handleReiceive:" ++ event, any))
    };

  let handleEvent = (event, response) => {
    let _ = Js.log(("handleEvent:" ++ event, response));
    ();
  };

  let putOn = (event, f: Js_json.t => unit, channel) => {
    let _ = channel##on(event, f);
    channel;
  };

  let putReceive = (event, handleReiceive, push) => {
    let _ = push##receive(event, handleReiceive);
    push;
  };

  let channelOpts = [%raw "{}"];

  let myChannel = Socket.initChannel(~params=channelOpts, ~socket=mySocket, "system");

  let _ = myChannel
    |> putOn("from_server", handleEvent("from:server"))
    |> Channel.join
    |> putReceive("ok", handleReceive("ok"))
    |> putReceive("error", handleReceive("error"));

  Js.log(myChannel);

  Js.log(mySocket##isConnected())

  Js.log("Socket is open!")
});

mySocket##onError(() => Js.log("Socket in error!"));

mySocket##onClose(() => Js.log("Socket closed!"));

/* DELAY CODE TO CHECK IF SOCKET IS CONNECTED */
/* ============================================================== */

/* [%raw {|setTimeout(() => console.log("Yiiick!"), 10000)|}]; */

[@bs.val] external setTimeout : (unit => unit, int) => float = "setTimeout";

/* setTimeout(() => {
  
  let handleReiceive = (event, any) =>
    switch event {
    | "ok" => Js.log(("handleReiceive:" ++ event, "Joined"))
    | "error" => Js.log(("handleReiceive:" ++ event, "Failed to join channel"))
    | _ => Js.log(("handleReiceive:" ++ event, any))
    };

  /* let handleEvent = (event, response) => {
    let _ = Js.log(("handleEvent:" ++ event, response));
    ();
  }; */

  let putReceive = (event, handleReiceive, push) => {
    let _ = push##receive(event, handleReiceive);
    push;
  };

  let channelOpts = [%raw "{}"];

  let myChannel = Socket.initChannel(~params=channelOpts, ~socket=mySocket, "system");

  let _ = myChannel
    |> Channel.join
    |> putReceive("ok", handleReiceive("ok"))
    |> putReceive("error", handleReiceive("error"));

  Js.log(myChannel);

  Js.log(mySocket##isConnected())
}, 1000); */

/* END DELAY CODE */
/* ============================================================== */

Js.log(mySocket);

Js.log(mySocket##protocol());

Js.log(mySocket##endPointURL());

Js.log(mySocket##isConnected());

/* START CHANNEL TESTS */

Js.log("==============================================");

/* let handleReiceive = (event, any) =>
  switch event {
  | "ok" => Js.log(("handleReiceive:" ++ event, "Joined"))
  | "error" => Js.log(("handleReiceive:" ++ event, "Failed to join channel"))
  | _ => Js.log(("handleReiceive:" ++ event, any))
  };

let handleEvent = (event, response) => {
  let _ = Js.log(("handleEvent:" ++ event, response));
  ();
};

let putReceive = (event, handleReiceive, push) => {
  let _ = push##receive(event, handleReiceive);
  push;
};

let channelOpts = [%bs.obj {
  timeout: 10000
}];

let myChannel = Socket.initChannel(~params=channelOpts, ~socket=mySocket, "system");

let _ = myChannel
  /* |> Channel.join */
  |> putReceive("ok", handleReiceive("ok"))
  |> putReceive("error", handleReiceive("error"));

Js.log(myChannel); */

/* DISCONNECT AFTER DELAY */

/* setTimeout(() => {
  let _ = Socket.disconnect(mySocket);
  Js.log("==============================================");
}, 60000); */

/* END CHANNEL TESTS */

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