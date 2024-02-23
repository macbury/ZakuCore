import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import { loadEntities } from '../actions/entities'

export type FlashType = "success" | "error" | "info"

export interface FlashState {
  message: string,
  type: FlashType,
  visible: boolean
}

export type FlashMessage = {
  message: string
  type: FlashType
}

const initialState : FlashState = {
  message: "",
  type: "success",
  visible: false
}

const flashSlice = createSlice({
  name: 'flash',
  initialState,
  reducers: {
    show: (state, { payload } : PayloadAction<FlashMessage>) => {
      state.message = payload.message
      state.type = payload.type
      state.visible = true
    },

    hide: (state)  => {
      state.visible = false
    }
  },

  extraReducers: (builder) =>
    //TODO: create global matcher
    builder.addCase(loadEntities.rejected, (state, { error }) => {
      state.message = error.toString()
      state.type = "error"
      state.visible = true
    })
})

export default flashSlice
