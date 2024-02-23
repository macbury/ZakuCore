import React from "react"
import {
  MdSignalWifiStatusbar4Bar,
  MdSignalWifiStatusbar3Bar,
  MdSignalWifiStatusbar2Bar,
  MdSignalWifiStatusbar1Bar,
  MdSignalWifiStatusbarConnectedNoInternet1,
  MdSignalWifiStatusbarNotConnected
} from "react-icons/md";

export interface ISignalStrengthsIconProps {
  rssi: number
}

export default function SignalStrengthsIcon({ rssi, ...props } : ISignalStrengthsIconProps) {
  if (!rssi) {
    return <MdSignalWifiStatusbarNotConnected {...props} />
  } else if (rssi < -90) {
    return <MdSignalWifiStatusbarConnectedNoInternet1 {...props} />
  } else if (rssi < -80) {
    return <MdSignalWifiStatusbar1Bar {...props} />
  } else if (rssi < -70) {
    return <MdSignalWifiStatusbar2Bar {...props} />
  } else if (rssi < -67) {
    return <MdSignalWifiStatusbar3Bar {...props} />
  } else if (rssi < -30) {
    return <MdSignalWifiStatusbar4Bar {...props} />
  }

  return <MdSignalWifiStatusbar4Bar {...props} />
}
