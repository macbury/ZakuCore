import { createSelector } from "@reduxjs/toolkit"
import { selectors } from "../slices/networks"
import { RootState } from "../store"

const selectLoading = (state : RootState) => state.networks.loading

export const selectNetworksSelector = createSelector([selectLoading, selectors.selectAll], (loading, networks) => {
  return {
    loading,
    networks: networks || []
  }
});
