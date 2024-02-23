import { useAppSelector } from "../../stores/store";
import { selectEntitiesWithTopic } from "../../stores/selectors/entities";

export function useEntities() {
  const { entities, loading } = useAppSelector((state) => ({
    loading: state.entities.loading,
    entities: selectEntitiesWithTopic(state)
  }));

  return { entities, loading }
}
