import React, { useLayoutEffect, useState, useCallback, useRef } from "react";
import { loadEntities } from "../actions/entities";
import Entities from '../slices/entities';
import Networks from '../slices/networks';
import { useAppDispatch } from "../store";
import { zakuCoreApi } from "../api";

const ACTION_SEND_MESSAGE = "ws/send";

interface IContext {
  ws?: WebSocket
}

const Context : IContext = {};

export const websocketMiddleware = store => next => action => {
  if (action.type === ACTION_SEND_MESSAGE) {
    Context.ws?.send(
      JSON.stringify(action.payload)
    )
  }
  next(action)
}

export function useSendMessage() {
  const dispatch = useAppDispatch()

  return useCallback((topic: string, payload : any) => {
    dispatch({
      type: ACTION_SEND_MESSAGE,
      payload: { topic, payload }
    })
  }, [dispatch]);
}

export function useSetupWebsocket() {
  const versionRef = useRef(0);
  const dispatch = useAppDispatch()
  const [reconnectTime, setReconnectTime] = useState(0);

  useLayoutEffect(() => {
    console.log("Started websocket");

    const ws = new WebSocket(`ws://${document.location.host}/api/ws`)

    ws.onmessage = (ev) => {
      const message = JSON.parse(ev.data);

      if (message.topic || message.topicMapping) {
        dispatch(Entities.actions.consumeMessage(message));
      } else if (message.fps) {
        console.log(`FPS: ${message.fps}`);
      } else if (message.network) {
        dispatch(Networks.actions.consumeMessage(message));
      } else if (message.status) {
        const action = zakuCoreApi.util.updateQueryData('getStatus', undefined, () => message.status)
        dispatch(action as any)
      } else if (message.reload) {
        const { reload } = message
        if (versionRef.current != reload) {
          console.log("New entity version, reloading: ", reload)
          versionRef.current = reload;
          dispatch(loadEntities() as any)
        } else {
          console.log("The same version, skipping")
        }
      } else {
        console.error("Unhandled command: ", message);
      }
    }

    ws.onerror = (ev) => {
      console.error("Failed with ws connection", ev);
    }

    ws.onclose = (ev) => {
      versionRef.current = -1;
      console.error("Closed ws connection", ev);
      dispatch(Entities.actions.clear());
      setTimeout(() => {
        setReconnectTime((v) => v+1);
      }, 1000);
    }

    Context.ws = ws;

    return () => {
      console.log("Closed connection");
      ws.close() // strict mode of react caused double rendering...
      Context.ws = undefined;
    }
  }, [reconnectTime])
}
