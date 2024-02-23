import React, { useCallback, useEffect, useState } from "react";
import styled from "@emotion/styled";
import { SiHomeassistant } from "react-icons/si";
import { FrameHexagon, FrameUnderline, LoadingBars } from '@arwes/core'
import { MdLightbulb, MdSignalWifiStatusbar4Bar, MdLogout, MdSettings, MdMenu, MdClose } from "react-icons/md";
import { useGetStatusQuery, useLogoutMutation } from "../stores";
import LinkButton from "../components/LinkButton";
import ActionButton from "../components/ActionButton";
import Toast from "../components/Toast";
import useMediaQuery from "../hooks/useMediaQuery";
import SignalStrengthsIcon from "../components/wifi/SignalStrengthsIcon";

const HeaderContent = styled.div`
  display: flex;
  flex-direction: row;
  position: fixed;
  top: 0px;
  left: 0px;
  right: 0px;
  z-index: 10000;
  box-shadow: 0px 0px 10px rgb(0 0 0 / 40%);
  backdrop-filter: blur(12px);
  background: linear-gradient(180deg, rgba(0,9,6,0.6) 0%, rgba(0,212,255,0.0) 100%);
`;

const Drawer = styled.div`
  display: flex;
  flex-direction: column;
  position: fixed;
  top: 0px;
  left: 0px;
  right: 0px;
  bottom: 0px;
  z-index: 10000;
  backdrop-filter: blur(12px);
  background: rgba(0,0,0, 0.1) linear-gradient(180deg, rgba(0,9,6,0.6) 0%, rgba(0,212,255,0.0) 100%);
`

const DrawerActions = styled.div`
  flex-direction: column;
  display: flex;
  align-items: end;
  padding: 10px 20px;
`

const DrawerOptions = styled.div`
  display: flex;
  flex-direction: column;
  flex: 1;
  padding: 0px 20px 40px 20px;
  a {
    display: block;
    width: 100%;
    margin-top: 15px;
  }

  a button {
    width: 100%;
  }
`

const Gap = styled.div`
  align-self: flex-end;
  flex: 1;
`;

const Link = styled(LinkButton)`
  display: flex;
  margin-right: 15px;
`;

export default function Header() {
  const [drawerVisible, setDrawerVisible] = useState(false);
  const desktop = useMediaQuery('(min-width: 768px)')

  const {
    data: status,
    isFetching,
    isSuccess,
    isError,
  } = useGetStatusQuery();

  const [logoutRequest, {}] = useLogoutMutation();

  const logout = useCallback(async () => {
    await logoutRequest({})
    window.location.reload()
  }, [logoutRequest])

  const toggleDrawer = useCallback(() => {
    setDrawerVisible((d) => !d)
  }, [setDrawerVisible])

  const closeDrawer = useCallback(() => {
    setDrawerVisible(false)
  }, [setDrawerVisible])

  useEffect(() => {
    if (desktop) {
      closeDrawer()
    }
  }, [desktop])

  if (isError) {
    return (
      <HeaderContent>
        <Toast palette="error">
          There is problem with connection...
        </Toast>
      </HeaderContent>
    )
  }

  if (!status) {
    return null
  }

  const frameComponent = drawerVisible ? FrameHexagon : FrameUnderline;

  const menuButtons = (
    <React.Fragment>
      <Link FrameComponent={frameComponent} to="/wifi" activate={isSuccess} icon={<SignalStrengthsIcon rssi={status.wifi.rssi} />} onClick={closeDrawer}>
        Wi-Fi: {status.wifi.ssid || "None"}
      </Link>
      <Link FrameComponent={frameComponent} to="/broker" activate={isSuccess} icon={<SiHomeassistant />} onClick={closeDrawer}>
        HomeAssistant: {status.broker.connected ? "Connected" : "Disconnected"}
      </Link>
      <Link FrameComponent={frameComponent} to="/" activate={isSuccess} icon={<MdLightbulb />} onClick={closeDrawer}>
        Lights
      </Link>
      {desktop && <Gap />}
      <Link FrameComponent={frameComponent} to="/admin" activate={isSuccess} icon={<MdSettings />} onClick={closeDrawer}>
        {!desktop && "Settings"}
      </Link>
      {!desktop && <Gap />}
      <ActionButton FrameComponent={frameComponent} onClick={logout} activate={isSuccess} icon={<MdLogout />}>
        Logout
      </ActionButton>
    </React.Fragment>
  )

  if (desktop) {
    return (
      <HeaderContent>
        {menuButtons}
      </HeaderContent>
    )
  }

  if (drawerVisible) {
    return (
      <Drawer className="drawer">
        <DrawerActions className="actions">
          <ActionButton FrameComponent={frameComponent} onClick={toggleDrawer} icon={<MdClose />}>Close</ActionButton>
        </DrawerActions>

        <DrawerOptions className="options">
          {menuButtons}
        </DrawerOptions>
      </Drawer>
    )
  }

  return (
    <HeaderContent>
      <ActionButton onClick={toggleDrawer} activate={isSuccess} icon={<MdMenu />}>
        Menu
      </ActionButton>
    </HeaderContent>
  )
}
