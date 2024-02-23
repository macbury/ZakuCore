import React from "react";
import styled from '@emotion/styled';

const Actions = styled.div`
  display: flex;
  margin-top: 19px;
  margin-bottom: 19px;
  flex-direction: row;
  justify-content: end;
  align-items: center;
  width: 100%;
  gap: 10px;
  padding-right: 20px;
  @media only screen and (min-width: 992px) {
    width: 640px;
    padding-right: 0px;
  }
`

export default Actions
