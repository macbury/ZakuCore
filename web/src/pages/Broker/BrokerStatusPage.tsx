import React from "react";
import { Text, List } from "@arwes/core";
import styled from '@emotion/styled';
import { SiHomeassistant } from "react-icons/si";
import { useGetBrokerQuery } from "../../stores";
import CenteredContent from "../../components/CenteredContent";
import Window from "../../components/Window";
import Actions from "../../components/Actions";
import LinkButton from "../../components/LinkButton";

const Value = styled(Text)`
  margin-left: 5px;
`

function passwordPlaceholder(password : string) {
  if (!password) {
    return '...'
  }
  const length = password.length;
  return Array(length).fill('*', 0, length).join('')
}

export default function BrokerStatusPage() {
  const {
    data,
    isSuccess
  } = useGetBrokerQuery(null, { pollingInterval: 1000 });

  return (
    <CenteredContent>
      <Window title="MQTT/HomeAssistant" activate={isSuccess}>
        <Text as="p">
          MQTT (aka MQ Telemetry Transport) is a machine-to-machine or “Internet of Things” connectivity protocol on top of TCP/IP. It allows extremely lightweight publish/subscribe messaging transport.
        </Text>
        <List>
          <li>
            <Text as="b">Status: </Text>
            <Value>{data?.broker?.connected ? "Connected" : "Disconnected"}</Value>
          </li>
          <li>
            <Text as="b">Address: </Text>
            <Value>{data?.broker?.address || '...'}</Value>
          </li>
          <li>
            <Text as="b">Port: </Text>
            <Value>{data?.broker?.port || '...'}</Value>
          </li>
          <li>
            <Text as="b">Username: </Text>
            <Value>{data?.broker?.username || '...'}</Value>
          </li>
          <li>
            <Text as="b">Password: </Text>
            <Value>{passwordPlaceholder(data?.broker?.password)}</Value>
          </li>
        </List>
      </Window>

      <Actions>
        <LinkButton icon={<SiHomeassistant />} to="/broker/setup" active activate={isSuccess} palette="primary">
          Configure HomeAssistant
        </LinkButton>
      </Actions>
    </CenteredContent>
  )
}
