import React, { useState, useCallback, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import { Text } from "@arwes/core";
import ActionButton from "../../components/ActionButton";
import Actions from "../../components/Actions";
import CenteredContent from "../../components/CenteredContent";
import LinkButton from "../../components/LinkButton";
import Window from "../../components/Window";
import { UpdateBrokerRequest } from "../../stores/types";
import flashSlice from "../../stores/slices/flash";
import { useUpdateConfigMutation, useGetBrokerQuery, useRestartMutation } from "../../stores";
import { useForm } from "../../hooks/formHook";
import { useAppDispatch } from "../../stores/store";

export default function ConnectBrokerPage() {
  const navigate = useNavigate();
  const dispatch = useAppDispatch();
  const {
    data,
    isSuccess
  } = useGetBrokerQuery();

  let { form, setForm, onFormChange } = useForm<UpdateBrokerRequest>({
    address: "",
    password: "",
    username: "",
    port: 0
  });

  const [updateConfig] = useUpdateConfigMutation();
  const [restart] = useRestartMutation();

  useEffect(() => {
    if (isSuccess) {
      setForm(data.broker);
    }
  }, [isSuccess, data])

  const updateBroker = useCallback(async (e) => {
    e.preventDefault();

    const data = new FormData();
    data.append("broker.json", new Blob([JSON.stringify(form)], { type: "application/json" }));
    await updateConfig({ key: "broker.json", data });
    await restart(null);

    dispatch(flashSlice.actions.show({ message: "Updated broker configuration", type: "success" }));
    navigate("/broker", { replace: true });
  }, [form])

  return (
    <CenteredContent>
      <form onSubmit={updateBroker}>
        <Window title="Connect to MQTT Broker" activate={isSuccess}>
          <label htmlFor="address"><Text>Address:</Text></label>
          <input id="address" type="text" value={form.address} onChange={onFormChange} />
          <label htmlFor="port"><Text>Port:</Text></label>
          <input id="port" type="text" value={form.port} onChange={onFormChange} />
          <label htmlFor="username"><Text>Username:</Text></label>
          <input id="username" type="text" value={form.username} onChange={onFormChange} />
          <label htmlFor="password"><Text>Password:</Text></label>
          <input id="password" type="password" value={form.password} onChange={onFormChange}/>
        </Window>
        <Actions>
          <ActionButton disabled={false} active activate={isSuccess} palette="primary" onClick={updateBroker}>
            Connect
          </ActionButton>
          <LinkButton to="/broker">
            Back
          </LinkButton>
        </Actions>
      </form>
    </CenteredContent>
  )
}
