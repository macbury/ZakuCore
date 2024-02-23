import React, { useCallback } from "react";
import { Link, useNavigate } from "react-router-dom";
import { Button, Text, ButtonProps } from "@arwes/core";
import styled from "@emotion/styled";

export interface LinkButtonProps extends ButtonProps {
  to: string
  activate?: boolean
  icon?: any
  children?: any
  title?: string
}

function ignoreLinkClick(e) {
  e.preventDefault()
}

const ButtonContainer = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
`

const IconContainer = styled.div`
  margin-right: 10px;
`

export default function LinkButton({ to, className, activate, children, style, icon, onClick, ...props } : LinkButtonProps) {
  const navigate = useNavigate();

  const goToPage = useCallback((e) => {
    setTimeout(() => {
      if (onClick) {
        onClick(e);
      }
      navigate(to)
    }, 100)
  }, [navigate, to])

  return (
    <Link to={to} className={className} style={style} onClick={ignoreLinkClick}>
      <Button {...props} animator={{ activate }} onClick={goToPage}>
        <ButtonContainer>
          {activate && icon && <IconContainer>{icon}</IconContainer>}
          <Text>{children}</Text>
        </ButtonContainer>
      </Button>
    </Link>
  )
}
