import React, { useCallback, useLayoutEffect, useState } from "react";
import styled from "@emotion/styled";
import { Text } from "@arwes/core";
import { useBleeps } from "@arwes/sounds";
import csx from "classnames";
import { MdLight } from "react-icons/md";
import { useAnimator } from "@arwes/animation";
import { Color } from "../../stores/types";
import { rgbToHex } from "../../helpers/color";


const TileCard = styled.div`
  backdrop-filter: blur(8px);
  color: rgb(126, 252, 246);
  border: 1px solid rgba(126, 252, 246, 0.2);
  background-color: rgba(5, 198, 193, 0.4);
  border-radius: 12px;
  transition: background-color 100ms ease-out 0s, opacity 500ms ease-out 0s;
  width: 100%;
  margin-bottom: 15px;
  opacity: 0;

  &:active {
    background-color: rgba(5, 198, 193, 0.3);
    border: 1px solid rgba(126, 252, 246, 0.1);
  }

  &.visible {
    opacity: 1;
  }
`

const Header = styled.div`
  padding: 12px;
  display: flex;
  flex-direction: row;
  align-items: center;
`;

const IconContainer = styled.div`
  margin-right: 6px;
`;

const IconCircle = styled.div`
  cursor: pointer;
  width: 40px;
  height: 40px;
  border-radius: 20px;
  display: flex;
  align-items: center;
  justify-content: center;
  background-color: rgb(5, 198, 193, 0.2);
  border: 1px solid transparent;
  transition: background-color 180ms ease-in-out 0s, color 180ms ease-in-out 0s;

  &:hover {
    border-color: #fff;
  }

  &.on {
    background-color: var(--background-color, rgba(126, 252, 246, 0.2));

    svg {
      fill: #fff;
    }
  }
  svg {
    width: 24px;
    height: 24px;
  }
`;

const Features = styled.div`

`;

const Title = styled.div`
  min-width: 0px;
  min-height: 40px;
  padding: 2px 8px;

  cursor: pointer;
  border-radius: 5px;
  flex: 1;

  &:hover {
    background: rgba(255, 255, 255, 0.1);
  }

  & > .primary, & > .secondary {
    display: flex;
  }

  & > .primary {
    font-weight: 500;
    font-size: 14px;
    line-height: 20px;
    letter-spacing: 0.1px;
  }

  & > .secondary {
    font-weight: 400;
    font-size: 12px;
    line-height: 16px;
    letter-spacing: 0.4px;
  }
`;


export interface ITileCardProps {
  on: boolean
  color?: Color,
  brightness: number
  name: string
  children: any,
  onToggleLight()
  onToggleIcon();
}

export default function TileCardEntity({ color, on, name, brightness, onToggleIcon, onToggleLight, children, ...props } : ITileCardProps) {
  const bleeps = useBleeps();
  const [visible, setVisible] = useState(false);
  const animator = useAnimator();
  const brightnessPercent = Math.round(((brightness || 0.0) / 255.0) * 100)
  const stopEvents = useCallback((ev) => {
    ev.stopPropagation()
  }, []);

  const triggerClick = useCallback(() => {
    onToggleLight();
    bleeps.click.play();
  }, [bleeps, onToggleLight]);

  const triggerIconClick = useCallback(() => {
    onToggleIcon();
    bleeps.click.play();
  }, [bleeps, onToggleIcon]);

  useLayoutEffect(() => {
    switch(animator?.flow.value) {
      case "entering":
      case "exiting":
        bleeps.assemble.play()
      break;

      case "entered":
      case "exited":
        bleeps.assemble.stop()
      break
    }
  }, [animator?.flow.value]);

  useLayoutEffect(() => {
    switch(animator?.flow.value) {
      case "entering":
      case "entered":
        setVisible(true)
      break;

      case "exited":
      case "exiting":
        setVisible(false)
      break
    }
  }, [animator?.flow.value, setVisible]);

  const variables = color ? {
    "--accent-color": `rgba(${color.r}, ${color.g}, ${color.b}, 1.0)`,
    "--background-color": `rgba(${color.r}, ${color.g}, ${color.b}, 0.2)`,
    "--track-color": `rgba(${color.r}, ${color.g}, ${color.b}, 0.7)`,
  } : {}

  return (
    <TileCard className={csx({ visible, "tile-card": true })} {...props} style={variables as any}>
      <Header>
        <IconContainer onClick={triggerIconClick}>
          <IconCircle className={csx({ on })}>
            <MdLight />
          </IconCircle>
        </IconContainer>
        <Title onClick={triggerClick}>
          <Text as="span" className="primary">{name}</Text>
          <Text as="span" className="secondary">{on ? `${brightnessPercent}%` : "OFF"}</Text>
        </Title>
      </Header>

      <Features onClick={stopEvents}>
        {children}
      </Features>
    </TileCard>
  )
}
