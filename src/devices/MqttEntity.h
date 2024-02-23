#ifndef MqttEntity_h
#define MqttEntity_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Relay.h>
#include <ESPNetwork.h>
#include "Entity.h"
#include <consts.h>

/// Class used for configuring entity on the HomeAssistant side
class MqttEntity : public Entity {
  protected:
    /// Configure entity using HomeAssistant api
    /// https://www.home-assistant.io/docs/mqtt/discovery/#using-abbreviations-and-base-topic
    void configure() {
      DynamicJsonDocument doc(1024);
      char topic[TOPIC_SIZE];

      this->generateTopic(topic);
      doc[F("uniq_id")] = this->id;
      doc[F("obj_id")] = this->id;
      doc[F("name")] = this->name;

      doc[F("~")] = topic;
      doc[F("schema")] = EntityConsts::SCHEMA;
      doc[F("stat_t")] = EntityConsts::STATE;
      doc[F("cmd_t")] = EntityConsts::SET;
      doc[F("val_tpl")] = EntityConsts::VAL_TPL;

      char availabilityTopic[TOPIC_SIZE];
      this->config.getAvailabilityTopic(availabilityTopic);
      doc[F("avty_t")] = availabilityTopic; // https://www.home-assistant.io/integrations/sensor.mqtt/#availability_topic
      // doc[F("avty_mode")] = "latest";

      auto dev = doc.createNestedObject(F("dev"));
      dev[F("name")] = this->config.name;
      dev[F("identifiers")] = WiFi.macAddress();
      dev[F("cu")] = "http://" + WiFi.localIP().toString();

      this->configureSensor(doc);

      char discoveryTopic[TOPIC_SIZE];
      this->generateSubTopic(discoveryTopic, "/config");
      this->relay->publish(discoveryTopic, doc);
    }

    void subscribe() {
      char setTopic[TOPIC_SIZE];
      this->generateSubTopic(setTopic, "/set");

      Serial.print(F("Subscribe to: "));
      Serial.println(setTopic);
      this->relay->subscribe(setTopic);
    }

    bool matchedTopic(const char *topic) {
      char setTopic[64];
      this->generateSubTopic(setTopic, "/set");

      return strcmp(setTopic, topic) == 0;
    }
  public:
    MqttEntity(GundamConfig config, Relay *relay) : Entity(config, relay) {

    }

    void collectTopic() {
      if (this->relay->uiConnected()) {
        DynamicJsonDocument doc(1024);
        auto topic = doc.createNestedObject("topicMapping");
        topic["id"] = this->entityId;

        char topicName[TOPIC_SIZE];
        this->generateTopic(topicName);
        topic["tn"] = topicName;
        this->relay->publish(doc);
      }
    }

    void unsubscribe() {
      char setTopic[TOPIC_SIZE];
      this->generateSubTopic(setTopic, "/set");

      Serial.print(F("Unsubscribed from: "));
      Serial.println(setTopic);
      this->relay->unsubscribe(setTopic);
    }

    void generateTopic(char* output) {
      strcpy(output, EntityConsts::PREFIX);
      strcat(output, this->type);
      strcat(output, EntityConsts::SLASH);
      strcat(output, this->id);
    }

    void generateSubTopic(char* output, const char * append) {
      this->generateTopic(output);
      strcat(output, append);
    }

    /// Publish current sensor state
    void pushState() {
      DynamicJsonDocument doc(1024);
      doc[F("obj_id")] = this->id;

      this->prepareStatePayload(doc);

      char stateTopic[TOPIC_SIZE];
      this->generateSubTopic(stateTopic, "/state");
      this->relay->publish(stateTopic, doc);
    }

    ~MqttEntity() {
      this->unsubscribe();
    }
};

#endif
