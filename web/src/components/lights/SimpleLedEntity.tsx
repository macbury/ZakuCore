import React, { useCallback } from "react";

import { SimpleLightEntity } from "../../stores";
import { selectEntityState } from "../../stores/selectors/entities";
import { useAppSelector } from "../../stores/store";
import { useSendMessage } from "../../stores/middleware/websocket";

import TileCardEntity from "./TileCardEntity";
import BrightnessFeature from './Features/BrightnessFeature'


export interface ISimpleLightEntityProps {
  entity : SimpleLightEntity;
}

export default function SimpleLedEntity({ entity, ...props } : ISimpleLightEntityProps) {
  const sendMessage = useSendMessage();

  const {
    state,
    brightness
  } = useAppSelector((state) => selectEntityState(state, entity));

  const on = state === "ON"

  const toggleLight = useCallback(() => {
    if (entity.topics) {
      sendMessage(entity.topics.set, { state: on ? "OFF" : "ON" })
    }
  }, [on, entity])

  const setBrightness = useCallback((brightness) => {
    if (entity.topics) {
      sendMessage(entity.topics.set, { state: "ON", brightness })
    }
  }, [entity])

  return (
    <TileCardEntity
      brightness={brightness || 0}
      name={entity.name}
      on={on}
      onToggleIcon={toggleLight}
      onToggleLight={toggleLight}
      {...props}>
      <BrightnessFeature
        enabled={on}
        onValueChange={setBrightness}
        value={brightness || 0}
        max={255} />
    </TileCardEntity>
  )
}
