import React, { useCallback, useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import { Text } from "@arwes/core";
import Actions from "../Actions";
import ActionButton from "../ActionButton";
import Window from "../Window";
import { GetGundamResponse } from "../../stores/types";
import { useForm } from "../../hooks/formHook";
import flashSlice from "../../stores/slices/flash";
import { useRestartMutation, useUpdateConfigMutation, useGetStatusQuery } from '../../stores'
import { useAppDispatch } from "../../stores/store";

export default function ChangeName(props) {
  const navigate = useNavigate();
  const dispatch = useAppDispatch();
  const {
    isLoading,
    isSuccess,
    isError,
    data,
  } = useGetStatusQuery();
  const [updateConfig] = useUpdateConfigMutation();
  const [restart] = useRestartMutation();
  const { form, setForm, onFormChange } = useForm<GetGundamResponse>({
    hostname: "",
    model: "",
    name: ""
  });

  useEffect(() => {
    if (isSuccess) {
      setForm(data.gundam)
    }
  }, [isSuccess])

  useEffect(() => {
    let value = encodeURIComponent(form.name.toLowerCase().replaceAll(' ', '-').replaceAll(/[^a-z0-9 -_]+/gi, '-'))
    setForm((s) => {
      return {...s, hostname: value }
    })
  }, [form.name])

  const submitForm = useCallback(async (e) => {
    e.preventDefault();

    const data = new FormData();
    data.append("gundam.json", new Blob([JSON.stringify(form)], { type: "application/json" }));
    const { write, size } = await updateConfig({ key: "gundam.json", data }).unwrap()

    if (write) {
      dispatch(flashSlice.actions.show({ message: "Updated settings!", type: "success" }))
      await restart({})
      navigate(`/reboot/${form.hostname}`, { replace: true });
    } else {
      dispatch(flashSlice.actions.show({ message: "Could not update settings", type: "error" }))
    }
  }, [form]);

  return (
    <form onSubmit={submitForm} {...props}>
      <Window title="Change name">
        <Text as="p">Name is used to identify this device in local network, and if you have enabled HomeAssistant integration for generating entity names and device name</Text>
        <label htmlFor="name"><Text>Name:</Text></label>
        <input disabled={isLoading} id="name" type="text" value={form.name} onChange={onFormChange} />
        <Text as="p">You will find this panel at address:&nbsp;</Text>
        <Text as="p">"http://{form.hostname}.local"</Text>
        <label htmlFor="model"><Text>Model:</Text></label>
        <input disabled={isLoading} id="model" type="text" value={form.model} onChange={onFormChange} />
        <Text as="p">This is used for details about current gunpla model, for example: RX-78-02 GUNDAM</Text>
      </Window>

      <Actions>
        <ActionButton disabled={isLoading} active palette="primary" onClick={submitForm}>
          Update settings
        </ActionButton>
      </Actions>
    </form>
  )
}
