let formatTimestamp: float => string = timestamp => 
  timestamp
  -> Js.Date.fromFloat
  -> Js.Date.toLocaleTimeString;