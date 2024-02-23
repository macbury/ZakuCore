import { createAsyncThunk, isAnyOf } from '@reduxjs/toolkit';
import { ScanNetworkResponse } from '../types';

export const refreshNetworks = createAsyncThunk('networks/refresh', async () => {
  const response = await fetch("/api/wifi/scan")
  return await response.json();
})
