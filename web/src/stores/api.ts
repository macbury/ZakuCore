import { createApi, fetchBaseQuery } from '@reduxjs/toolkit/query/react'
import { decode } from "@msgpack/msgpack";
import {
  PingResponse,
  StatusResponse,
  SignInRequest,
  SignInResponse,
  UpdatePasswordResponse,
  UpdatePasswordRequest,
  GetBrokerResponse,
} from './types'

export const zakuCoreApi = createApi({
  reducerPath: 'zakuCoreApi',
  tagTypes: ['Status'],
  baseQuery: fetchBaseQuery({ baseUrl: '/api/' }),
  endpoints: (builder) => ({
    ping: builder.query<PingResponse, void | null>({ query: () => '/ping' }),
    logout: builder.mutation({ query: () => '/logout' }),
    reset: builder.mutation({ query: () => '/reset' }),
    restart: builder.mutation({ query: () => '/restart' }),
    getConfig: builder.query<any, { key: string }>({
      query: ({ key }) => ({
        url: `/config?key=${key}`,
        responseHandler: async (response) => {
          if (key.endsWith(".json")) {
            return await response.json()
          }
          const blob = await response.blob();
          const msgpack = decode(await blob.arrayBuffer());
          return msgpack
        }
      }),
    }),
    getBroker: builder.query<GetBrokerResponse, void | null>({ query: () => '/broker' }),
    getStatus: builder.query<StatusResponse, void | null>({ query: () => '/status' }),
    updateConfig: builder.mutation<{ write: boolean, size: number }, { key : string , data: any }>({
      query: ({ key, data }) => ({
        url: `/config/write?key=${key}`,
        method: 'PUT',
        body: data
      }),
    }),
    reloadEntities: builder.mutation<{ reload: boolean }, void | null>({
      query: () => `/entities/reload`,
    }),
    signIn: builder.mutation<SignInResponse, SignInRequest>({
      invalidatesTags: ['Status'],
      query: (data) => ({
        url: '/sign_in',
        method: 'POST',
        body: data
      }),
    }),
    updatePassword: builder.mutation<UpdatePasswordResponse, UpdatePasswordRequest>({
      query: (data) => ({
        url: '/password',
        method: 'POST',
        body: data
      }),
    })
  }),
})

export const {
  useReloadEntitiesMutation,
  useGetConfigQuery,
  usePingQuery,
  useGetBrokerQuery,
  useGetStatusQuery,
  useSignInMutation,
  useLogoutMutation,
  useUpdatePasswordMutation,
  useResetMutation,
  useRestartMutation,
  useUpdateConfigMutation
} = zakuCoreApi
