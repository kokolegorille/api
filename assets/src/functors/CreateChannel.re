type socket = Phoenix.Socket.t;
type channel = Phoenix.Channel.t;

module RR = ReasonReact;
let str = RR.string;

/* Helpers */

let putOn = (event, f: Js_json.t => unit, channel) => {
  let _ = channel##on(event, f);
  channel;
};

let putReceive = (event, handleReceive, push) => {
  let _ = push##receive(event, handleReceive);
  push;
};

let putOnError = (f: Js_json.t => unit, channel) => {
  let _ = channel##onError(f);
  channel;
};

let putOnClose = (f: Js_json.t => unit, channel) => {
  let _ = channel##onClose(f);
  channel;
};

let joinChannel = Phoenix.Channel.join;

/* let listBy = [%bs.raw {|
  (id, { metas: [first, ...rest] }) =>
    Object.assign({}, first, { id, count: rest.length + 1 })
|}];  */

let defaultSync = (topic, list) => Js.log2(topic, list);

/* FUNCTOR */

module Make = () => {
  type state =
    | Initialized
    | Joined(channel)
    | Left;
  
  type action = 
    | LeaveChannel(channel)
    | ChannelJoined(channel)
    | ChannelFailed
    | ChannelClosed;

  let component = RR.reducerComponent("ChannelComponent");
  let make = (
    ~socket, ~topic, ~render, ~withPresence=false, 
    ~handleReceive, ~handleEvent, ~handleSync=defaultSync, _children
  ) => {
    ...component,
    initialState: () => Initialized,
    reducer: (action, _state) => 
      switch (action) {
      | LeaveChannel(channel) =>
        RR.UpdateWithSideEffects(
          Left,
          _self => {let _ = Phoenix.Channel.leave(channel)}
        )
      | ChannelJoined(channel) => RR.Update(Joined(channel))
      | ChannelFailed => RR.Update(Left)
      | ChannelClosed => RR.Update(Left)
      },
    didMount: ({send, onUnmount}) => {
      Js.log("Mounted");
      let myChannel = Phoenix.Socket.channel(topic, socket);

      if (withPresence) {
        let myPresence = Phoenix.Presence.init(myChannel);

        let _ = myPresence##onJoin(() => {
          Js.log("Presence joined!");
        });

        let _ = myPresence##onSync(() => {
          Js.log(myPresence##list);
          Js.log("------------");

          /* let loggedInUsers = myPresence##list(listBy);
          handleSync(topic, loggedInUsers); */

          handleSync(topic, myPresence##list);
        });
      };

      let _ = myChannel
      |> putOn("ping", handleEvent("ping"))
      |> joinChannel
      |> putReceive("ok", handleReceive("ok"))
      |> putReceive("error", handleReceive("error"))
      |> putReceive("timeout", handleReceive("timeout"));

      /* let _ = myChannel
      |> putOnError(handleEvent("error"))
      |> putOnClose();  */

      send(ChannelJoined(myChannel));

      onUnmount(() => {
        Js.log("Disconnect channel!")
        let _ = Phoenix.Channel.leave(myChannel);
      });
    },
    render: ({state}) => 
      switch (state) {
      | Initialized => <div>(str("Initialized..."))</div>
      | Left => <div>(str("Left..."))</div>
      | Joined(channel) => render(channel)
      }
  }
};

