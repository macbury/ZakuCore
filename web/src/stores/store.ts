import { TypedUseSelectorHook, useDispatch, useSelector } from 'react-redux'
import { configureStore } from '@reduxjs/toolkit'
import { setupListeners } from '@reduxjs/toolkit/query'
import flashSlice from './slices/flash'
import entitiesSlice from './slices/entities'
import networksSlice from './slices/networks'
import { zakuCoreApi } from './api'
import { websocketMiddleware } from "./middleware/websocket"

export const store = configureStore({
  reducer: {
    [flashSlice.name]: flashSlice.reducer,
    [entitiesSlice.name]: entitiesSlice.reducer,
    [zakuCoreApi.reducerPath]: zakuCoreApi.reducer,
    [networksSlice.name]: networksSlice.reducer
  },

  middleware: (getDefaultMiddleware) => ([
    ...getDefaultMiddleware(),
    zakuCoreApi.middleware,
    websocketMiddleware
  ])
})

setupListeners(store.dispatch)

export type RootState = ReturnType<typeof store.getState>
export type AppDispatch = typeof store.dispatch
export const useAppDispatch: () => AppDispatch = useDispatch
export const useAppSelector: TypedUseSelectorHook<RootState> = useSelector
