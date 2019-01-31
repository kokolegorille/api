module SpecialSocket = CreateSocket.Make();

module SpecialChannel = CreateChannel.Make();

let endPoint = "ws://localhost:4000/socket";

let socketOpts = token => [%bs.obj {
  params: {token: token},
  timeout: 10000,
  logger: (kind, msg, data) => Js.log(kind ++ ": " ++ msg ++ ", " ++ data)
}];

/* Callbacks */

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

/* React Component */

module RR = ReasonReact;

let str = RR.string;

type presence = Abstract.PresenceData.t;

type state = {
  systemPresences: list(presence)
};

let initialState = {
  systemPresences: []
};

type action = 
  | UpdatePresence(string, list(presence));

let reducer = (action, _state) => 
  switch (action) {
  | UpdatePresence(topic, loggedInUsers) => 
    switch (topic) {
    | "system" => 
      RR.Update({systemPresences: loggedInUsers})
    | _ => {
      Js.log("topic not matched : " ++ topic)
      RR.NoUpdate
    }
    };
  };

let component = RR.reducerComponent("Member");
let make = (~token, ~userName, ~userId, _children) => {
  ...component,
  initialState: () => initialState,
  reducer,
  render: self => {
    
    let handleSync = (topic, loggedInUsers) => {
      let _ = Js.log(("handleSync:" ++ topic, loggedInUsers));

      /* let result = List.map(x => {
        Js.log(x);
        Js.log("============================>");
        x;
      }, loggedInUsers)

      Js.log(result); */

      /* 
      TODO, transform loggedInUsers into a list of presence!
       */

      self.send(UpdatePresence(topic, []));
    };

    <div> 
      <h2> (str(userName)) </h2> 
      <p> (str("Id: " ++ string_of_int(userId))) </p> 
      <p> (str("Token: " ++ token)) </p> 
      <SpecialSocket
        endPoint
        socketOpts=socketOpts(token)
        render=(
          socket => {
            /* At this level, I receive the socket from SpecialSocket! */
            Js.log("Socket joined");

            let systemTopic = "system";
            let privateTopic = "user:" ++ string_of_int(userId);
            <div>
              <p>(str("Socket connected!"))</p>
              <SpecialChannel
                topic=systemTopic
                socket
                handleEvent
                handleReceive
                withPresence=true
                handleSync
                render=(
                  channel => {
                    /* At this level, I receive the channel from SpecialChannel! */
                    Js.log("Channel joined with topic : " ++ channel##topic);

                    <p>(str("Channel connected with topic : " ++ systemTopic))</p>
                  }
                )
              />
              <SpecialChannel
                topic=privateTopic
                socket
                handleEvent
                handleReceive
                render=(
                  channel => {
                    /* At this level, I receive the channel from SpecialChannel! */
                    Js.log("Channel joined with topic : " ++ channel##topic);

                    <p>(str("Channel connected with topic : " ++ privateTopic))</p>
                  }
                )
              />
            </div>
          }
        )
      />
    </div>
  },
};