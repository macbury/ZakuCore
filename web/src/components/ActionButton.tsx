import React from "react";
import { Button, Text, ButtonProps } from "@arwes/core";
import styled from "@emotion/styled";

const IconContainer = styled.div`
  margin-right: 10px;
`

export interface ActionButton extends ButtonProps {
  children: any
  activate?: boolean
  icon?: any
}

const ButtonContainer = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
`

export default function ActionButton({ children, icon, activate, ...props } : ActionButton) {
  return (
    <Button {...props} animator={{ activate }}>
      <ButtonContainer>
        {icon && <IconContainer>{icon}</IconContainer>}
        <Text>{children}</Text>
      </ButtonContainer>
    </Button>
  )
}
