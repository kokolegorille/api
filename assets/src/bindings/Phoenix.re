module Abstract = {
  type void = unit;
  type function_ = unit => unit;
  type any = Js_json.t;
};

module Push = {
  open Abstract;
  type t = {
    .
    [@bs.meth] "resend": float => void,
    [@bs.meth] "send": unit => void,
    [@bs.meth] "receive": (string, any => void) => t,
  };
};

module Channel = {
  open Abstract;
  type ref = int;
  type t = {
    .
    [@bs.get] "topic": string,
    [@bs.get] "state": string,
    [@bs.meth] "rejoinUntilConnected": unit => unit,
    [@bs.meth] "onClose": unit => unit,
    [@bs.meth] "onError": unit => unit,
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

  [@bs.send]
  external push : (t, string, Js.t('payload), ~timeout: float=?, unit) => Push.t = "push";
  let push = (event, payload, ~timeout=?, channel) => push(channel, event, payload, ~timeout?, ());
};

module Socket = {
  open Abstract;
  type t = {
    .
    [@bs.meth] "protocol": unit => string,
    [@bs.meth] "endPointURL": unit => string,
    [@bs.meth] "log": (string, string, any) => void,
    [@bs.meth] "hasLogger": unit => bool,
    [@bs.meth] "onOpen": function_ => void,
    [@bs.meth] "onClose": function_ => void,
    [@bs.meth] "onError": function_ => void,
    [@bs.meth] "onMessage": function_ => void,
    [@bs.meth] "connectionState": unit => void,
    [@bs.meth] "isConnected": unit => bool,
  };

  [@bs.module "phoenix"][@bs.new]
  external init : (string, ~opts: Js.t('opts)=?, unit) => t = "Socket"; 
  let init = (~opts: option(Js.t('opts))=?, endPoint) => init(endPoint, ~opts?, ());

  [@bs.send] 
  external connect : (t, ~params: Js.t('params)=?, unit) => unit = "connect";
  let connect = (~params: option(Js.t('params))=?, socket) => {
    let _ = connect(~params?, socket, ());
    socket;
  };

  [@bs.send]
  external 
  disconnect : (t, ~callback: function_=?, ~code: string=?, ~reason: Js.t('reason)=?, unit) => unit = "disconnect";
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
  external channel : (t, string, ~params: Js.t('params)=?, unit) => Channel.t = "channel";
  let channel = (~params: option(Js.t('params))=?, topic, socket) => channel(socket, topic, ~params?, ());
};

module Presence = {
  open Abstract;
  type t = {
    .
    [@bs.meth] "onJoin": function_ => void,
    [@bs.meth] "onLeave": function_ => void,
    [@bs.meth] "onSync": function_ => void,
    [@bs.meth] "list": function_ => any,
    [@bs.meth] "inPendingSyncState": unit => bool,
  };

  [@bs.module "phoenix"][@bs.new]
  external init : (Channel.t, ~opts: Js.t('opts)=?, unit) => t = "Presence"; 
  let init = (~opts: option(Js.t('opts))=?, channel) => init(channel, ~opts?, ());
};


/* 
module Abstract = {
  type id = int;
  type phx_ref = string;

  type presence = {
    id: id,
    phx_ref: phx_ref
  };

  type metas = list(presence);
};

module Push = {
  type event = string;
  type timeoutMs = float;
  type t;

  [@bs.send.pipe : t] 
  external receive : (event, Js.t('payload) => unit) => t = "";
  [@bs.send.pipe : t] 
  external resend : timeoutMs => _ = "";
  [@bs.send] 
  external send : t => _ = "";
};

module rec Channel: {
  type t;

} = Channel
and Socket: {
  type t;
  type event = Push.event;
  [@bs.send.pipe : t] external channel : (string, ~chanParams: Js.t('params)=?) => Channel.t = "";
  [@bs.send.pipe : t] external connect : Js.t('params) => unit = "";
  [@bs.send] external connectionState : t => string = "";
  [@bs.send.pipe : t] external disconnect : (unit => 'a, 'code, string) => 'a = "";
  [@bs.send] external endPointURL : t => string = "";
  [@bs.send] external isConnected : t => bool = "";
  [@bs.new] [@bs.module "phoenix"] external make : (string, Js.t('opts)) => t = "Socket";
  [@bs.send.pipe : t] external onClose : (event => 'a) => unit = "";
  [@bs.send] external onConnOpen : t => unit = "";
  [@bs.send.pipe : t] external onConnClose : event => unit = "";
  [@bs.send.pipe : t] external onConnError : event => unit = "";
  [@bs.send.pipe : t] external onError : (event => 'a) => unit = "";
  [@bs.send.pipe : t] external onMessage : (event => 'a) => unit = "";
  [@bs.send.pipe : t] external onOpen : (event => 'a) => unit = "";
  [@bs.send] external protocol : t => string = "";
  [@bs.send.pipe : t] external remove : Channel.t => unit = "";
  [@bs.send] external triggerChanError : t => unit = "";
} = Socket;

module Presence = {
  type t;
};

/* 
type event = Push.event;

type timeoutMs = Push.timeoutMs;

module rec Channel: {
  type t;
  [@bs.send.pipe : t] external canPush : unit => bool = "";
  [@bs.send.pipe : t] external join : (~timeoutMs: timeoutMs=?) => Push.t = "";
  [@bs.send.pipe : t] external leave : (~timeoutMs: timeoutMs=?) => Push.t = "";
  [@bs.new] [@bs.module "phoenix"]
  external make : (string, Js.t('params), Socket.t) => t = "Channel";
  [@bs.send.pipe : t] external off : event => unit = "";
  [@bs.send.pipe : t] external on : (event, 'reason => 'a) => unit = "";
  [@bs.send.pipe : t] external onClose : ('reason => 'a) => unit = "";
  [@bs.send.pipe : t] external onError : ('reason => 'a) => unit = "";
  [@bs.send.pipe : t]
  external push : (event, Js.t('payload), ~timeoutMs: timeoutMs=?) => Push.t = "";
  [@bs.send] external rejoinUntilConnected : t => _ = "";
} = Channel
and Socket: {
  type t;
  [@bs.send.pipe : t] external channel : (string, ~chanParams: Js.t('params)=?) => Channel.t = "";
  [@bs.send.pipe : t] external connect : Js.t('params) => unit = "";
  [@bs.send] external connectionState : t => string = "";
  [@bs.send.pipe : t] external disconnect : (unit => 'a, 'code, string) => 'a = "";
  [@bs.send] external endPointURL : t => string = "";
  [@bs.send] external isConnected : t => bool = "";
  [@bs.new] [@bs.module "phoenix"] external make : (string, Js.t('opts)) => t = "Socket";
  [@bs.send.pipe : t] external onClose : (event => 'a) => unit = "";
  [@bs.send] external onConnOpen : t => unit = "";
  [@bs.send.pipe : t] external onConnClose : event => unit = "";
  [@bs.send.pipe : t] external onConnError : event => unit = "";
  [@bs.send.pipe : t] external onError : (event => 'a) => unit = "";
  [@bs.send.pipe : t] external onMessage : (event => 'a) => unit = "";
  [@bs.send.pipe : t] external onOpen : (event => 'a) => unit = "";
  [@bs.send] external protocol : t => string = "";
  [@bs.send.pipe : t] external remove : Channel.t => unit = "";
  [@bs.send] external triggerChanError : t => unit = "";
} = Socket; 
*/
 */
