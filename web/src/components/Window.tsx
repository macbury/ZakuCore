import React from "react";
import { FrameHexagon, Text } from "@arwes/core";
import { Animator } from "@arwes/animation";
import styled from '@emotion/styled';

const Content = styled(FrameHexagon)`

`

const InnerContent = styled.div`
  width: 100%;
  @media only screen and (min-width: 992px) {
    width: 640px;
  }
`;

export interface IWindowProps {
  children: any
  activate?: boolean
  title?: string
}

export default function Window({ children, activate, title, ...props } : IWindowProps) {
  return (
    <Animator animator={{ activate }}>
      <Content {...props}>
        <InnerContent>
          <Text as="h1">
            {title}
          </Text>

          {children}
        </InnerContent>
      </Content>
    </Animator>
  )
}
