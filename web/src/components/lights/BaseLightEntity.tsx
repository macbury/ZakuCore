import React from "react";
import { Entity } from "../../stores";
import SimpleLedEntity from "./SimpleLedEntity";
import RGBLedStripEntity from "./RGB/RGBLedStripEntity";
import { Animator } from "@arwes/animation";

export interface IBaseLightEntityProps {
  entity : Entity;
}

export default function BaseLightEntity({ entity, ...props } : IBaseLightEntityProps) {
  const animator = {
    manager: 'sequence',
    combine: true,
    activate: true,
    duration: { enter: 500, exit: 500 }
  }

  switch (entity.kind) {
    case "SIMPLE_LED":
      return (
        <Animator animator={animator}>
          <SimpleLedEntity entity={entity} {...props} />
        </Animator>
      )

    case "RGB_LED":
      return (
        <Animator animator={animator}>
          <RGBLedStripEntity entity={entity} {...props} />
        </Animator>
      )
    default:
      throw "Unsupported kind: " + entity.kind
  }
}
