import React, { useEffect } from "react";
import { LoadingBars } from "@arwes/core";
import styled from '@emotion/styled'
import { useGetStatusQuery } from "../stores";
import SingInPage from "../pages/SignInPage";

const Loader = styled(LoadingBars)`
  z-index: 0;
`;

const Footer = styled.div`
  position: fixed;
  bottom: 10px;
  left: 10px;
  opacity: 0.5;
  font-size: 11px;
`;

const Content = styled.div`
  display: flex;
  flex-direction: column;
  flex: 1;
  height: 100%;
  overflow-y: auto;
`;

const ContentWithHeader = styled(Content)`
  padding-top: 70px;
  padding-bottom: 42px;
`;

export default function BootScreen({ children }) {
  const {
    isLoading,
    isUninitialized,
    isError,
    data,
    error,
    refetch
  } = useGetStatusQuery();

  useEffect(() => {
    if (data) {
      window.document.title = `Zaku Core - ${data?.gundam.name}`
    }
  }, [data])

  if (isError && error.status == 403) {
    return (
      <Content>
        <SingInPage refetchSession={refetch} />
      </Content>
    )
  }

  const showLoader = isUninitialized || isLoading;

  if (showLoader) {
    return (
      <ContentWithHeader>
        <Loader
          size={2}
          speed={4}
          full
        />
      </ContentWithHeader>
    )
  }

  return (
    <ContentWithHeader>
      {children}
      <Footer>ZakuCore v{data.firmware.release} <b>{data.firmware.name}</b></Footer>
    </ContentWithHeader>
  )
}
