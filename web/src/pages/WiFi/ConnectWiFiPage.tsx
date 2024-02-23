import React, { useState, useCallback, useEffect } from "react";
import { useNavigate, useParams } from "react-router-dom";
import { Text } from "@arwes/core";
import ActionButton from "../../components/ActionButton";
import Actions from "../../components/Actions";
import CenteredContent from "../../components/CenteredContent";
import LinkButton from "../../components/LinkButton";
import Window from "../../components/Window";
import { ConnectNetworkRequest, useGetStatusQuery, useUpdateConfigMutation, useRestartMutation } from "../../stores";
import { useForm } from "../../hooks/formHook";

export default function ConnectWifiPage() {
  const navigate = useNavigate();
  const {
    data: status,
    isSuccess
  } = useGetStatusQuery();

  let { ssid } = useParams();
  let { form, setForm, onFormChange } = useForm<ConnectNetworkRequest>({
    ssid: ssid || "",
    password: "",
  });

  const [updateConfig] = useUpdateConfigMutation();
  const [restart] = useRestartMutation();

  const connectToWifi = useCallback(async (e) => {
    e.preventDefault();
    const data = new FormData();
    data.append("wifi.json", new Blob([JSON.stringify(form)], { type: "application/json" }));
    const { write, size } = await updateConfig({ key: "wifi.json", data }).unwrap()
    await restart(null)
    navigate(`/reboot/${status?.wifi.hostname}`, { replace: true });
  }, [form, status])


  return (
    <CenteredContent>
      <form onSubmit={connectToWifi}>
        <Window title="Connect to Network" activate={isSuccess}>
          <label htmlFor="ssid"><Text>SSID:</Text></label>
          <input id="ssid" type="text" value={form.ssid} onChange={onFormChange} />
          <label htmlFor="password"><Text>Password:</Text></label>
          <input id="password" type="password" value={form.password} onChange={onFormChange}/>
        </Window>
        <Actions>
          <ActionButton disabled={false} active activate={isSuccess} palette="primary" onClick={connectToWifi}>
            Connect
          </ActionButton>
          <LinkButton to="/wifi/setup">
            Back
          </LinkButton>
        </Actions>
      </form>
    </CenteredContent>
  )
}
