#ifndef BoardEntity_H
#define BoardEntity_H

#include <ESPNetwork.h>
#include "MqttEntity.h"
#include <consts.h>

/// This entity sends information about current ESP8266 board, IP address, RSSI
class BoardEntity : public MqttEntity {
  protected:
    void prepareStatePayload(DynamicJsonDocument &doc) {
      doc[F("state")] = EntityConsts::ON;
      doc[F("uptime")] = millis() / 1000;
      doc[F("Local IP")] = WiFi.localIP();
      doc[F("MAC")] = WiFi.macAddress();
      doc[F("RSSI")] = WiFi.RSSI();
      doc[F("DNS")] = WiFi.dnsIP(0);
      doc[F("SSID")] = WiFi.SSID();

      doc[F("Free heap")] = ESP.getFreeHeap();
      #ifdef USE_ESP8266
        doc[F("Host")] = WiFi.hostname();
        doc[F("Heap frag")] = ESP.getHeapFragmentation();
      #endif
      doc[F("MD5")] = ESP.getSketchMD5();
      doc[F("Version")] = Version::Name;
    }

    virtual void configureSensor(DynamicJsonDocument &doc) {
      doc[F("json_attributes_topic")] = F("~/state");
      doc[F("val_tpl")] = F("{{ value_json.state | default(0) }}");
    }
  public:
    BoardEntity(GundamConfig config, Relay *relay) : MqttEntity(config, relay) {
      this->init("board", config.name, EntityConsts::ENTITY_BINARY_SENSOR);
      String board = "board";

      strlcpy(
        this->entityId,
        board.c_str(),
        sizeof(this->entityId)
      );
    }
};

#endif
