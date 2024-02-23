import {
  createSlice,
  createEntityAdapter,
  PayloadAction,
  EntityState
} from '@reduxjs/toolkit'
import { Entity, Topic, EntityTopicState } from '../types'
import { loadEntities } from '../actions/entities'

export interface EntitiesState extends EntityState<Entity> {
  loading: boolean,
  topics: Topic[],
  states: {
    [topic : string]: EntityTopicState
  }
}

const entitiesAdapter = createEntityAdapter<Entity>({
  selectId: (entity) => entity.id,
  sortComparer: (a, b) => a.name.localeCompare(b.name),
})

const entitiesSlice = createSlice({
  name: 'entities',
  initialState: entitiesAdapter.getInitialState<EntitiesState>({
    topics: [],
    loading: false,
    entities: {},
    ids: [],
    states: {}
  }),

  reducers: {
    clear(state) {
      state.loading = true
      entitiesAdapter.setAll(state, [])
    },
    consumeMessage: (state, { payload: { topic, payload, topicMapping } } : PayloadAction<{ topic?: string, payload: any, topicMapping?: Topic }>) => {
      if (topic?.endsWith("/state")) {
        state.states[topic] = payload
      }

      if (topicMapping) {
        let updated = false;
        state.topics = state.topics.map((topic) => {
          if (topic.id == topicMapping.id) {
            updated = true;
            return topicMapping;
          } else {
            return topic;
          }
        })

        if (!updated) {
          state.topics.push(topicMapping);
        }
      }
    }
  },
  extraReducers: (builder) =>
    builder
      .addCase(loadEntities.fulfilled, (state, { payload: { entities } }) => {
        entitiesAdapter.setAll(state, entities)
        state.loading = false
      })
      .addCase(loadEntities.pending, (state) => {
        state.loading = true
      })
      .addCase(loadEntities.rejected, (state) => {
        state.loading = false
      })
})

export const selectors = entitiesAdapter.getSelectors<{ entities: EntitiesState }>((state) => state.entities)

export default entitiesSlice
