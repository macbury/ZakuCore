import React, { useLayoutEffect } from "react"
import styled from '@emotion/styled';
import { Animator } from "@arwes/animation";
import flashSlice from "../stores/slices/flash";
import { useAppDispatch, useAppSelector } from "../stores/store";
import Toast from "./Toast";

const FlashContainer = styled.div`
  z-index: 9001;
  bottom: 10px;
  position: fixed;
  right: 20px;
  left: 20px;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-content: center;
  align-items: center;
`;

const FlashInnerContainer = styled.div`
  min-width: 320px;
  flex: 1;
`;

export default function FlashMessage() {
  const dispatch = useAppDispatch()
  const { message, type, visible } = useAppSelector((state) => state.flash);

  useLayoutEffect(() => {
    const handler = setTimeout(() => {
      dispatch(flashSlice.actions.hide())
    }, 5000)

    return () => clearTimeout(handler)
  }, [visible])

  if (!visible) {
    return null;
  }

  return (
    <Animator animator={{ activate: visible }}>
      <FlashContainer>
        <FlashInnerContainer>
          <Toast palette={type}>{message}</Toast>
        </FlashInnerContainer>
      </FlashContainer>
    </Animator>
  )
}
