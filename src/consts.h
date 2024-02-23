#ifndef consts_H
#define consts_H

#include <Arduino.h>

const byte TOPIC_SIZE = 128;

namespace Version {
  const int Release = 34;
  const char * Name = "2023.07.17";
}

namespace EntityConsts {
  const byte RECONNECT_EVERY = 5;
  const byte SYNC_BOARD_STATE_EVERY = 5;
  const char * TOPIC_HOME_ASSISTANT_STATUS = "homeassistant/status";

  const char * ONLINE = "online";
  const char * OFFLINE = "offline";
  const char * ENTITY_SENSOR = "sensor";
  const char * ENTITY_BINARY_SENSOR = "binary_sensor";
  const char * ENTITY_LIGHT = "light";
  const char * PREFIX = "homeassistant/";
  const char * AVAILABILITY = "availability";
  const char * SCHEMA = "json";
  const char * SLASH = "/";
  const char * STATE = "~/state";
  const char * SET = "~/set";
  const char * VAL_TPL = "{{ value_json.state | default(OFF) }}";

  const char * ON = "ON";
  const char * OFF = "OFF";

  namespace PayloadKey {
    const char * STATE = "state";
    const char * RGB = "rgb";
    const char * BRIGHTNESS = "brightness";
  }

  namespace BlueprintKey {
    const char * NAME = "name";
    const char * ID = "id";
    const char * PIXELS = "pixels";
    const char * LIGHTS = "lights";
    const char * PIN = "pin";
    const char * SPEED = "speed";
  }
}

const char * CONNECTION_CONFIG_FILE = "/config/wifi.json";
const char * BROKER_CONFIG_FILE = "/config/broker.json";
const char * GUNDAM_CONFIG_FILE = "/config/gundam.json";

#endif
