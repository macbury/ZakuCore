import React, { useCallback, useState, useMemo } from "react";
import { RgbColorPicker } from "react-colorful";
import throttle from "lodash/throttle";

import { RGBLightEntity } from "../../../stores";
import { useSendMessage } from "../../../stores/middleware/websocket";
import { selectEntityState } from "../../../stores/selectors/entities";
import { useAppSelector } from "../../../stores/store";
import Modal from "../../Modal";
import BrightnessFeature from "../Features/BrightnessFeature";
import TileCardEntity from "../TileCardEntity";
import Actions from "../../Actions";
import ActionButton from "../../ActionButton";

export interface IRGBLightEntityProps {
  entity : RGBLightEntity;
}

export default function RGBLightEntity({ entity, ...props } : IRGBLightEntityProps) {
  const sendMessage = useSendMessage();
  const [colorPickerVisible, setColorPickerVisible] = useState(false);

  const {
    state,
    brightness,
    color
  } = useAppSelector((state) => selectEntityState(state, entity));

  const on = state === "ON"

  const toggleLight = useCallback(() => {
    if (entity.topics) {
      sendMessage(entity.topics.set, { state: on ? "OFF" : "ON" })
    }
  }, [on, entity])

  const showColorPicker = useCallback(() => {
    setColorPickerVisible(true)
  }, [on, entity])

  const setBrightness = useCallback((brightness) => {
    if (entity.topics) {
      sendMessage(entity.topics.set, { state: "ON", brightness })
    }
  }, [entity])

  const setColor = useCallback((color) => {
    if (entity.topics) {
      sendMessage(entity.topics.set, { state: "ON", color })
    }
    return true;
  }, [entity])

  const changeColor = useMemo(() => throttle(setColor, 100), [setColor])

  return (
    <TileCardEntity
      brightness={brightness || 0}
      name={entity.name}
      color={color}
      on={on}
      onToggleIcon={showColorPicker}
      onToggleLight={toggleLight}
      {...props}>
      <BrightnessFeature
        enabled={on}
        onValueChange={setBrightness}
        value={brightness || 0}
        max={255} />
      <Modal visible={colorPickerVisible}>
        <RgbColorPicker
          color={color}
          onChange={changeColor} />
        <Actions>
          <ActionButton onClick={() => setColorPickerVisible(false)}>
            Done
          </ActionButton>
        </Actions>
      </Modal>
    </TileCardEntity>
  )
}
