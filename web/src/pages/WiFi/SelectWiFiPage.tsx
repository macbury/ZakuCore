import React, { useLayoutEffect, useCallback, useState, useEffect } from "react";
import { MdRefresh } from "react-icons/md";
import Actions from "../../components/Actions";
import { refreshNetworks } from '../../stores/actions/networks';
import CenteredContent from "../../components/CenteredContent";
import ActionButton from "../../components/ActionButton";
import Window from "../../components/Window";
import WiFiNetworkButton from "../../components/wifi/WifFiNetworkButton";
import { useAppDispatch, useAppSelector } from "../../stores/store";
import { selectNetworksSelector } from "../../stores/selectors/networks";
import { Animator } from "@arwes/animation";

export default function SelectWiFiPage() {
  const dispatch = useAppDispatch();
  const { networks, loading } = useAppSelector((state) => selectNetworksSelector(state));
  const [visible, setVisible] = useState(false);

  const refreshWifi = useCallback((e) => {
    dispatch(refreshNetworks() as any)
  }, [])

  useLayoutEffect(() => {
    refreshWifi(null);
  }, [])

  useEffect(() => {
    const handler = setTimeout(() => {
      setVisible(networks.length > 0);
    }, 250);

    return () => {
      clearTimeout(handler)
      setVisible(false)
    }
  }, [networks.length, setVisible])

  const items = networks.map((network, i) => (
    <WiFiNetworkButton network={network} key={network.id} />
  ))

  return (
    <CenteredContent>
      <Window title="Select Wi-Fi Network" activate={visible}>
        <Animator animator={{
          activate: visible,
          manager: 'stagger',
          combine: true
        }}>
          {items}
        </Animator>
      </Window>
      <Actions>
        <ActionButton icon={<MdRefresh />} disabled={!visible} active activate={!loading} palette="primary" onClick={refreshWifi}>
          Refresh List
        </ActionButton>
      </Actions>
    </CenteredContent>
  )
}
