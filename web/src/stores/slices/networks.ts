import {
  createSlice,
  createEntityAdapter,
  PayloadAction,
  EntityState
} from '@reduxjs/toolkit'
import { WiFi } from '../types'
import { refreshNetworks } from "../actions/networks"

export interface NetworksState extends EntityState<WiFi> {
  loading: boolean
}

const networksAdapter = createEntityAdapter<WiFi>({
  selectId: (entity) => entity.id,
  sortComparer: (a, b) => a.ssid.localeCompare(b.ssid),
})

const networksSlice = createSlice({
  name: 'networks',
  initialState: networksAdapter.getInitialState<NetworksState>({ loading: false, ids: [], entities: {} }),

  reducers: {
    clear(state) {
      state.loading = true
      networksAdapter.setAll(state, [])
    },
    consumeMessage: (state, { payload: { network } } : PayloadAction<{ network: WiFi }>) => {
      networksAdapter.setOne(state, network)
    }
  },
  extraReducers: (builder) =>
    builder
      .addCase(refreshNetworks.fulfilled, (state, { payload: { entities, ts } }) => {
        state.loading = false
      })
      .addCase(refreshNetworks.pending, (state) => {
        state.loading = true
        networksAdapter.setAll(state, [])
      })
      .addCase(refreshNetworks.rejected, (state) => {
        state.loading = false
      })
})

export const selectors = networksAdapter.getSelectors<{ networks: NetworksState }>((state) => state.networks)

export default networksSlice
