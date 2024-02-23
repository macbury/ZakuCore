import React from "react";
import { Text, List } from "@arwes/core";
import styled from '@emotion/styled';
import { useGetStatusQuery } from "../../stores";
import CenteredContent from "../../components/CenteredContent";
import Window from "../../components/Window";
import Actions from "../../components/Actions";
import LinkButton from "../../components/LinkButton";

const Value = styled(Text)`
  margin-left: 5px;
`

export default function WiFiStatusPage() {
  const {
    data: status,
    isSuccess
  } = useGetStatusQuery();

  return (
    <CenteredContent>
      <Window title="Network" activate={isSuccess}>
        <List>
          <li>
            <Text as="b">Status: </Text>
            <Value>{status?.wifi?.connected ? "Connected" : "Disconnected"}</Value>
          </li>
          <li>
            <Text as="b">SSID: </Text>
            <Value>{status?.wifi?.ssid || '...'}</Value>
          </li>
          <li>
            <Text as="b">IP Address: </Text>
            <Value>{status?.wifi?.ip || '...'}</Value>
          </li>
          <li>
            <Text as="b">Hostname: </Text>
            <Value>{status?.wifi?.hostname || '...'}</Value>
          </li>
          <li>
            <Text as="b">Mac Address: </Text>
            <Value>{status?.wifi?.mac || '...'}</Value>
          </li>
          <li>
            <Text as="b">RSSI: </Text>
            <Value>{status?.wifi?.rssi || '...'}</Value>
          </li>
        </List>
      </Window>

      <Actions>
        <LinkButton to="/wifi/setup" active activate={isSuccess} palette="primary">
          Change Network
        </LinkButton>
      </Actions>
    </CenteredContent>
  )
}
