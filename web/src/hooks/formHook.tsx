import { useState, useCallback } from 'react'

export function useForm<T>(defaultValue : T) {
  let [form, setForm] = useState<T>(defaultValue);

  const onFormChange = useCallback(({ target } : React.ChangeEvent<HTMLInputElement>) => {
    setForm((prevState) => {
      let nextState = { ...prevState };
      nextState[target.id] = target.value
      return nextState
    })
  }, [setForm])

  return {
    form, setForm, onFormChange
  }
}
