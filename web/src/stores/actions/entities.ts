import { createAsyncThunk, isAnyOf } from '@reduxjs/toolkit';
import { decode } from "@msgpack/msgpack";
import { EntitiesResponse } from '../types';

// resolved by api?
async function fetchEntities() : Promise<EntitiesResponse> {
  try {
    const response = await fetch("/api/config?key=entities.db")
    const blob = await response.blob();
    const bytes = await blob.arrayBuffer();
    const object = decode(bytes) as EntitiesResponse;
    return object;
  } catch (error) {
    console.error("Could not fetch entities", error);
    return { entities: [] }
  }
}

export const loadEntities = createAsyncThunk('entities/all', async () => {
  const { entities } = await fetchEntities();
  return { entities }
})
