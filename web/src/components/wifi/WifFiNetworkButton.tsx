import React, { useState, useEffect } from "react";
import { FrameHexagon } from "@arwes/core";
import styled from "@emotion/styled";
import LinkButton from "../LinkButton";
import { WiFi } from "../../stores/types";
import SignalStrengthsIcon from "./SignalStrengthsIcon";

export interface WiFiNetworkButtonProps {
  network: WiFi
}

const Wrapper = styled.div`
  padding-bottom: 15px;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
`

const MacAddress = styled.span`
  opacity: 0.6;
  padding-left: 10px;
`

const Button = styled(LinkButton)`
  /* backdrop-filter: blur(0px); */
  width: 100%;

  button, a {
    display: block;
    width: 100%;
  }
`

export default function WiFiNetworkButton({ network } : WiFiNetworkButtonProps) {
  const [activate, setActivate] = useState(false)
  useEffect(() => {
    const handler = setTimeout(() => {
      setActivate(true)
    }, 10)

    return () => {
      clearTimeout(handler)
      setActivate(false)
    }
  }, [network])

  return (
    <Wrapper>
      <Button
        icon={<SignalStrengthsIcon rssi={network.rssi} />}
        active={true}
        activate={activate}
        to={`/wifi/${encodeURIComponent(network.ssid)}`}
        FrameComponent={FrameHexagon}>
        {network.ssid}
        <MacAddress>{network.bssid}</MacAddress>
      </Button>
    </Wrapper>
  )
}
