import React, { useCallback, useState } from "react";
import ChangeName from "../components/admin/ChangeName";
import ChangePassword from "../components/admin/ChangePassword";
import Tabs from "../components/admin/Tabs";
import CenteredContent from "../components/CenteredContent";

export default function AdminPage() {
  return (
    <CenteredContent>
      <Tabs />
      <ChangeName />
      <ChangePassword />
    </CenteredContent>
  )
}
