import React from 'react'
import { FrameLines, LoadingBars } from '@arwes/core'
import styled from "@emotion/styled";
import { MdAdd } from "react-icons/md";

import LinkButton from '../../components/LinkButton';
import CenteredContent from '../../components/CenteredContent';
import BaseLightEntity from '../../components/lights/BaseLightEntity';
import { useEntities } from './useEntities';

const Items = styled.div`
  max-width: 640px;
  width: 100%;
  padding-bottom: 120px;

  .arwes-loading-bars {
    margin: 0 auto;
  }
`;

const FabContainer = styled.div`
  position: fixed;
  bottom: 15px;
  right: 15px;
  z-index: 9000;

  @media only screen and (min-width: 992px) {
    bottom: 40px;
    right: 40px;
  }

  button {
    backdrop-filter: blur(12px);
    background: rgba(0, 0, 0, 0.3);
    font-size: 18px;
  }

  svg.icon {
    width: 24px;
    height: 24px;
  }
`

export default function ListLightsPage() {
  const { entities, loading } = useEntities();
  const items = entities.map((entity) => (<BaseLightEntity entity={entity} key={entity.id} />));

  if (loading) {
    return (
      <CenteredContent>
        <LoadingBars size={2} animator={{ activate: true }} />
      </CenteredContent>
    )
  }

  return (
    <React.Fragment>
      <CenteredContent>
        <Items>
          {items}
        </Items>
      </CenteredContent>
      <FabContainer>
        <LinkButton FrameComponent={FrameLines} to="/lights/new" icon={<MdAdd className="icon" />}>Add light</LinkButton>
      </FabContainer>
    </React.Fragment>
  )
}
