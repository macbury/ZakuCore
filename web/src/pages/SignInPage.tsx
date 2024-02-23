import React, { useCallback, useState } from "react";
import { Text } from "@arwes/core";
import Actions from "../components/Actions";
import CenteredContent from "../components/CenteredContent";
import ActionButton from "../components/ActionButton";
import Window from "../components/Window";
import Toast from "../components/Toast";
import { SignInRequest } from "../stores/types";
import { useForm } from "../hooks/formHook";
import { useSignInMutation } from '../stores'

export interface ISignInPageProps {
  refetchSession();
}

export default function SignInPage({ refetchSession } : ISignInPageProps) {
  const [errors, setError] = useState<string[] | null>()
  const [signIn, { isLoading }] = useSignInMutation();
  const { form, onFormChange } = useForm<SignInRequest>({
    password: "",
  });

  const submitForm = useCallback(async (e) => {
    e.preventDefault();

    const { success, errors } = await signIn(form).unwrap()

    if (success) {
      setError(null);
      refetchSession();
    } else {
      setError(errors);
    }
  }, [form, setError]);

  return (
    <CenteredContent>
      <form onSubmit={submitForm}>
        <Window title="Please sign in">
          <Toast palette={errors ? 'error' : null}>
            {
              errors ?
                (<span>{errors.join(",")}</span>)
                :
                (<span>Default password is <b>"archangel"</b></span>)
            }
          </Toast>
          <label htmlFor="password"><Text>Password:</Text></label>
          <input disabled={isLoading} id="password" type="password" value={form.password} onChange={onFormChange} autoFocus />
        </Window>

        <Actions>
          <ActionButton disabled={isLoading} active palette="primary" onClick={submitForm}>
            Sign in
          </ActionButton>
        </Actions>
      </form>
    </CenteredContent>
  )
}
