import React, { useLayoutEffect, useState } from "react";
import csx from "classnames";
import ReactDOM from "react-dom";
import { Animator } from "@arwes/animation";
import styled from "@emotion/styled";
import CenteredContent from "./CenteredContent";

const ModalContainer = styled.div`
  display: none;
  position: fixed;
  top: 0px;
  bottom: 0px;
  left: 0px;
  right: 0px;
  backdrop-filter: blur(12px);
  background: rgba(0, 0, 0, 0.4);
  z-index: 10000;

  &.visible {
    display: flex;
  }
`;

export interface IModalProps {
  visible: boolean
  children: any
}

export default function Modal({ children, visible, ...props } : IModalProps) {
  const [el, setElement] = useState<HTMLDivElement>();

  useLayoutEffect(() => {
    const div = document.createElement('div')
    document.body.appendChild(div);
    setElement(div)

    return () => {
      document.body.removeChild(div)
    }
  }, [visible])

  return (
    <React.Fragment>
      {
        el && ReactDOM.createPortal(
          <ModalContainer className={csx({ visible })}>
            <Animator animator={{ activate: visible, root: true }}>
              <CenteredContent {...props}>
                {children}
              </CenteredContent>
            </Animator>
          </ModalContainer>,
          el
        )
      }
    </React.Fragment>
  )
}
