import React, { useCallback, useState } from "react";
import { Text } from "@arwes/core";
import { Animator } from "@arwes/animation";
import Actions from "../Actions";
import CenteredContent from "../CenteredContent";
import ActionButton from "../ActionButton";
import Window from "../Window";
import Toast from "../Toast";
import { UpdatePasswordRequest } from "../../stores/types";
import { useForm } from "../../hooks/formHook";
import flashSlice from "../../stores/slices/flash";
import { useUpdatePasswordMutation } from '../../stores'
import { useAppDispatch } from "../../stores/store";

export default function ChangePassword(props) {
  const dispatch = useAppDispatch();
  const [updatePassword, { isLoading, isSuccess, data: updateResponse }] = useUpdatePasswordMutation();
  const { form, onFormChange } = useForm<UpdatePasswordRequest>({
    password: "",
    currentPassword: ""
  });

  const submitForm = useCallback(async (e) => {
    e.preventDefault();

    const { success, errors } = await updatePassword(form).unwrap()

    if (success) {
      dispatch(flashSlice.actions.show({ message: "Updated password!", type: "success" }))
    } else {
      dispatch(flashSlice.actions.show({ message: "Could not update password: " + errors.join(", "), type: "error" }))
    }
  }, [form]);

  return (
    <form onSubmit={submitForm} {...props}>
      <Window title="Change password">
        <Text as="p">It is advised to change the password from default <b>"archangel"</b> to something else</Text>
        <label htmlFor="currentPassword"><Text>Current Password:</Text></label>
        <input disabled={isLoading} id="currentPassword" type="password" value={form.currentPassword} onChange={onFormChange} />
        <label htmlFor="password"><Text>New Password:</Text></label>
        <input disabled={isLoading} id="password" type="password" value={form.password} onChange={onFormChange} />
      </Window>

      <Actions>
        <ActionButton disabled={isLoading} active palette="primary" onClick={submitForm}>
          Change password
        </ActionButton>
      </Actions>
    </form>
  )
}
