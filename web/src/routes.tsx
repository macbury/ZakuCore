import React from 'react'
import {
  createBrowserRouter,
  createRoutesFromElements,
  Route,
  RouterProvider,
} from "react-router-dom";
import LightsSetupPage from "./pages/Lights/LightsSetupPage";
import MainLayout from './layouts/MainLayout';
import BrokerStatusPage from './pages/Broker/BrokerStatusPage';
import ConnectBrokerPage from './pages/Broker/ConnectBrokerPage';
import ConnectWifiPage from './pages/WiFi/ConnectWiFiPage';
import SelectWiFiPage from './pages/WiFi/SelectWiFiPage';
import AdminPage from './pages/AdminPage';
import WaitForRebootPage from './pages/WaitForRebootPage';
import WiFiStatusPage from './pages/WiFi/WiFiStatusPage';
import ListLightsPage from './pages/Lights/ListLightsPage';
import NewLightPage from './pages/Lights/NewLightPage';
import UpdateFirmwarePage from './pages/Admin/UpdateFirmwarePage';
import BootScreen from './layouts/BootScreen';
import WebsocketListener from './components/WebsocketListener';

const router = createBrowserRouter(
  createRoutesFromElements(
    <Route path="/" element={<MainLayout />}>
      <Route index path="/" element={<ListLightsPage />} />
      <Route element={<LightsSetupPage />} path="/lights/setup" />
      <Route element={<NewLightPage />} path="/lights/new" />
      <Route element={<UpdateFirmwarePage />} path="/admin/firmware" />
      <Route element={<AdminPage />} path="/admin" />
      <Route element={<WaitForRebootPage />} path="/reboot/:hostname" />
      <Route element={<ConnectWifiPage />} path="/wifi/:ssid" />
      <Route element={<SelectWiFiPage />} path="/wifi/setup" />
      <Route element={<WiFiStatusPage />} path="/wifi" />
      <Route element={<ConnectBrokerPage />} path="/broker/setup" />
      <Route element={<BrokerStatusPage />} path="/broker" />
    </Route>
  )
);

export default function Routes() {
  return (
    <BootScreen>
      <WebsocketListener>
        <RouterProvider router={router} />
      </WebsocketListener>
    </BootScreen>
  )
}
