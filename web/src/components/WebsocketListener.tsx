import React from "react";
import { useSetupWebsocket } from "../stores/middleware/websocket";

export default function WebsocketListener({ children }) {
  useSetupWebsocket();

  return (
    <React.Fragment>
      {children}
    </React.Fragment>
  )
}
