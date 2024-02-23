
export type PingResponse = {
  done: boolean
};

export type TopicsResponse = {
  ts: Topic[]
}

export type Topic = {
  id: string
  tn: string,
  ts: Topic[]
}

export type Color = {
  r: number,
  g: number,
  b: number
}

export type EntityState = "ON" | "OFF"

export type EntityTopicState = {
  state: EntityState,
  obj_id: string,
  color_mode?: "RGB",
  brightness?: number,
  color?: Color
}

export type GetBrokerResponse = {
  broker: {
    connected: boolean
    address: string,
    port: number
    username: string
    password: string
  }
}

export type GetGundamResponse = {
  hostname: string
  name: string
  model: string
}

export type UpdateBrokerRequest = {
  address: string,
  port: number
  username: string
  password: string
}

export type UpdateBrokerResponse = {
  success: boolean,
  errors: string[]
}

export type UpdatePasswordRequest = {
  password: string,
  currentPassword: string
}

export type UpdatePasswordResponse = {
  success: boolean,
  errors: string[]
}

export type SignInRequest = {
  password: string
}

export type SignInResponse = {
  success: boolean,
  errors: string[]
}

export type StatusResponse = {
  firmware: {
    md5: string,
    name: string,
    release: number
  }
  chipInfo: {
    heap: number,
    chipId: number,
    uptime: number
  },
  broker: {
    connected: boolean
    address: string
    port: number | string
  },
  wifi: {
    connected: boolean,
    mac: string,
    ssid: string,
    hostname: string,
    ip: string,
    rssi: number
  },
  gundam: GetGundamResponse
}

export type WiFi = {
  id: number,
  ssid: string,
  rssi: number,
  bssid: string,
  encryption: number
}

export type NetworksList = {
  networks: WiFi[],
  scan: boolean
}

export type ScanNetworkResponse = {
  status: string
}

export type ConnectNetworkRequest = {
  ssid: string,
  password: string,
}

export type EntityTopic = {
  base: string,
  state: string,
  set: string
}

export type SharedEntity = {
  kind: string,
  id: string,
  pin: string,
  name: string,
  topics?: EntityTopic
}

export type SimpleLightEntity = {
  kind: "SIMPLE_LED"
  color: Color
  brightness: number
} & SharedEntity

export type RGBLightEntity = {
  color: Color
  brightness: number
  pixels: number
} & SharedEntity

export type RGBStripEntity = {
  kind: "RGB_LED",
  lights: RGBLightEntity[]
} & SharedEntity

export type Entity = RGBStripEntity | SimpleLightEntity | RGBLightEntity

export type EntitiesResponse = {
  entities: Entity[]
}
