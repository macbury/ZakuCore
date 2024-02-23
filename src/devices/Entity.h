#ifndef Entity_h
#define Entity_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Relay.h>
#include <Configuration.h>

/// Class used for configuring entity on the HomeAssistant side
class Entity {
  protected:
    bool pendingPushState = false;
    bool pendingConfigure = false;

    virtual void prepareStatePayload(DynamicJsonDocument &doc) {

    }

    virtual void configureSensor(DynamicJsonDocument &doc) {

    }

    void init(const char * id, const char * name, const char * type) {
      Serial.printf("[Entity] Init entity: %s of type %s and id: %s\n", name, type, id);
      this->type = type;
      strcpy(this->id, config.hostname);
      strcat(this->id, "__");
      strcat(this->id, id);
      strcpy(this->name, name);
    }

    /// Configure entity using HomeAssistant api
    /// https://www.home-assistant.io/docs/mqtt/discovery/#using-abbreviations-and-base-topic
    virtual void configure() {

    }

    virtual void subscribe() {

    }
  public:
    GundamConfig config;
    Relay *relay;
    const char *type;
    char name[64];
    char id[64];
    char entityId[64]; // id in json document

    Entity(GundamConfig config, Relay *relay) {
      this->config = config;
      this->relay = relay;
    }

    /// Publish to discovery topic information about current entity
    virtual void setup() {
      Serial.printf("Delay setup entity: %s\n", this->id);
      this->pendingConfigure = true;
      this->syncState();
    }

    virtual void unsubscribe() {

    }

    virtual void collectTopic() {

    }

    void syncState() {
      this->pendingPushState = true;
    }

    /// Publish current sensor state
    virtual void pushState() {

    }

    virtual bool receive(const char *topic, JsonObject &doc) {
      Serial.println(F("Entity receive!"));
      return false;
    }

    virtual void dump(JsonObject &doc) {

    }

    virtual void loadBlueprint(JsonObject &blueprint) {
      Serial.println(F("Loading blueprint in base"));
    }

    virtual bool push() {
      if (this->pendingConfigure) {
        Serial.printf("-- Setup entity now: %s\n", this->id);
        this->pendingConfigure = false;
        this->subscribe();
        this->configure();
        this->collectTopic();

        return true;
      }

      if (this->pendingPushState) {
        this->pendingPushState = false;
        this->pushState();

        return true;
      }

      return false;
    }

    virtual void act(float delta) {

    }

    virtual ~Entity() {
      Serial.printf("Unloading %s\n", this->id);
      this->relay = NULL;
    }
};
#endif
