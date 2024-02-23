import { FrameCorners } from "@arwes/core";
import React, { useCallback, useEffect, useState } from "react";
import Actions from "../Actions";
import LinkButton from "../LinkButton";

export default function Tabs(props) {
  return (
    <Actions>
      <LinkButton palette="primary" to="/admin" FrameComponent={FrameCorners}>
        Settings
      </LinkButton>

      <LinkButton palette="primary" to="/admin/firmware/" FrameComponent={FrameCorners}>
        Update Firmware
      </LinkButton>
    </Actions>
  )
}
