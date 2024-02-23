import React, { useCallback, useEffect, useState } from 'react'
import { decode, encode } from "@msgpack/msgpack";
import { LoadingBars } from '@arwes/core';
import styled from "@emotion/styled";
import CenteredContent from '../../components/CenteredContent';
import { useAppDispatch } from '../../stores/store';
import Actions from '../../components/Actions';
import ActionButton from '../../components/ActionButton';
import { useGetConfigQuery, useReloadEntitiesMutation, useUpdateConfigMutation } from '../../stores';
import flashSlice from '../../stores/slices/flash';

const CodeEditor = styled.textarea`
  width: 640px;
`

const ENTITIES_FILE_NAME = "entities.db";

export default function NewLightPage() {
  const dispatch = useAppDispatch()
  const [reload] = useReloadEntitiesMutation();
  const { data: entities, isLoading: loading } = useGetConfigQuery({ key: ENTITIES_FILE_NAME });
  const [newEntities, setNewEntities] = useState("")
  const [updateConfig, { isLoading }] = useUpdateConfigMutation();

  useEffect(() => {
    if (entities != null) {
      setNewEntities(JSON.stringify(entities, null, 4))
    }
  }, [setNewEntities, entities != null])

  const submitForm = useCallback(async () => {
    try {
      // const data = JSON.parse(newEntities)
      const data = new FormData();
      const blob = encode(JSON.parse(newEntities));
      data.append(ENTITIES_FILE_NAME, new Blob([blob]));

      const { write, size } = await updateConfig({ key: ENTITIES_FILE_NAME, data }).unwrap();

      if (write) {
        dispatch(flashSlice.actions.show({ message: "Updated entities, reloading entities...", type: "success" }))
        await reload()
      } else {
        dispatch(flashSlice.actions.show({ message: "Could not update entities", type: "error" }))
      }
    } catch (e) {
      dispatch(flashSlice.actions.show({ message: `invalid json: ${e}`, type: "error" }))
    }

  }, [newEntities])

  if (loading) {
    return (
      <CenteredContent>
        <LoadingBars size={2} animator={{ activate: true }} />
      </CenteredContent>
    )
  }

  return (
    <CenteredContent>
      <CodeEditor
        disabled={isLoading}
        onChange={(e) => setNewEntities(e.target.value)}
        value={newEntities}
        rows={20} />
      <Actions>
        <ActionButton active palette="primary" onClick={submitForm} disabled={isLoading}>
          Save
        </ActionButton>
      </Actions>
    </CenteredContent>
  )
}
