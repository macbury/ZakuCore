import { createSelector } from "@reduxjs/toolkit"
import { selectors } from "../slices/entities"
import { RootState } from "../store"
import { Entity, Topic, EntityTopic, EntityTopicState } from "../types"

const selectTopics = (state : RootState) => state.entities.topics
const selectStates = (state : RootState) => state.entities.states

function buildTopics(topic? : Topic) : EntityTopic | null {
  if (!topic) {
    return null
  }

  return {
    base: topic.tn,
    state: `${topic.tn}/state`,
    set: `${topic.tn}/set`,
  }
}

// this is naive solution. In theory current implementation for light entity allows to build multi layer tree with all entities
// but for simplicity reason we are building only up to one child depth
export const selectEntitiesWithTopic = createSelector([selectors.selectAll, selectTopics], (entities, topics) => {
  return entities.map((entity) => {
    switch (entity.kind) {
      case "RGB_LED":
        const masterTopic = topics.find((topic) => topic.id == entity.id);

        if (!masterTopic) {
          break;
        }

        const lights = entity.lights.map((light) => {
          const topic = masterTopic.ts.find((topic) => topic.id == light.id);

          return { ...light, topics: buildTopics(topic) };
        });

        return {
          ...entity,
          lights
        }

      case "SIMPLE_LED":
        const topic = topics.find((topic) => topic.id == entity.id);
        return { ...entity, topics: buildTopics(topic) };
    }

    return entity;
  })
});


export const selectEntityState = createSelector([selectStates, (_state, entity : Entity) => entity], (states, entity) => {
  const defaultState = {
    state: "OFF",
    obj_id: entity.id,
  } as EntityTopicState;

  const key = entity.topics?.state;
  if (key) {
    return states[key] || defaultState;
  } else {
    return defaultState;
  }
});
