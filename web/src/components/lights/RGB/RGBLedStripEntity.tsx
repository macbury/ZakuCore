import React from "react";
import styled from "@emotion/styled";
import { Text } from "@arwes/core";
import { RGBStripEntity } from "../../../stores";
import RGBLightEntity from "./RGBLightEntity";

const Wrapper = styled.div`
  background: rgb(126, 252, 246, 0.1);
  padding: 10px;
  border-radius: 10px;
  margin-bottom: 15px;

  .tile-card:last-child {
    margin-bottom: 0px;
  }
`

const Header = styled(Text)`
  display: block;
  line-height: 1;
  font-size: 14px;
  color: rgba(0, 248, 248, 0.75);
  margin-top: 5px;
  margin-left: 10px;
  margin-bottom: 20px;
  text-transform: uppercase;
`

export interface IRGBLedStripEntityProps {
  entity : RGBStripEntity;
}

export default function RGBLedStripEntity({ entity, ...props } : IRGBLedStripEntityProps) {
  const items = entity.lights.map((e) => <RGBLightEntity entity={e} key={e.id} /> )

  return (
    <Wrapper {...props}>
      <Header>{entity.name}</Header>
      {items}
    </Wrapper>
  )
}
