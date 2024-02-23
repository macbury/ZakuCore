#ifndef SimpleLightEntity_H
#define SimpleLightEntity_H

#include <pins_board.h>
#include "MqttEntity.h"
#include "effects.h"

class SimpleLightEntity : public MqttEntity {
  private:
    uint8_t pin;
    byte dstBrightness;
    byte srcBrightness;
    byte userBrightness;
    float speed;
    float progress;
  protected:
    SIMPLE_EFFECT_FUNCTION;
    void prepareStatePayload(DynamicJsonDocument &doc);
    void configureSensor(DynamicJsonDocument &doc);
  public:
    float currentBrightness();
    bool on;
    bool receive(const char * topic, JsonObject &doc);
    void loadBlueprint(JsonObject &blueprint);
    void act(float delta);
    SimpleLightEntity(GundamConfig config, Relay *relay);
};

SimpleLightEntity::SimpleLightEntity(GundamConfig config, Relay *relay) : MqttEntity(config, relay) {

}

void SimpleLightEntity::loadBlueprint(JsonObject &blueprint) {
  strcpy(this->entityId, blueprint[EntityConsts::BlueprintKey::ID]);
  this->init(this->entityId, blueprint[EntityConsts::BlueprintKey::NAME], EntityConsts::ENTITY_LIGHT);

  this->pin = pinNameToId(blueprint[EntityConsts::BlueprintKey::PIN] | "");
  if (blueprint.containsKey(EntityConsts::PayloadKey::BRIGHTNESS)) {
    this->userBrightness = blueprint[EntityConsts::PayloadKey::BRIGHTNESS];
  } else {
    this->userBrightness = 255;
  }
  this->on = false;
  this->speed = blueprint[EntityConsts::BlueprintKey::SPEED] | 0.5f;
  this->srcBrightness = 0;
  this->processEffect = SimpleEffects::fade;
  this->progress = 0;

  Serial.printf("Configure pin: %i for Entity %s\n", this->pin, this->entityId);
}

float SimpleLightEntity::currentBrightness() {
  return this->processEffect(this->progress, this->srcBrightness, this->dstBrightness);
}

void SimpleLightEntity::prepareStatePayload(DynamicJsonDocument &doc) {
  doc[EntityConsts::PayloadKey::STATE] = this->on ? EntityConsts::ON : EntityConsts::OFF;
  doc[EntityConsts::PayloadKey::BRIGHTNESS] = this->userBrightness;
}

void SimpleLightEntity::configureSensor(DynamicJsonDocument &doc) {
  doc[EntityConsts::PayloadKey::BRIGHTNESS] = true;
}

bool SimpleLightEntity::receive(const char * topic, JsonObject &doc) {
  if (!this->matchedTopic(topic)) {
    return false;
  }

  this->on = doc[EntityConsts::PayloadKey::STATE] == EntityConsts::ON;

  if (doc.containsKey(EntityConsts::PayloadKey::BRIGHTNESS)) {
    this->userBrightness = doc[EntityConsts::PayloadKey::BRIGHTNESS];
  }

  this->srcBrightness = this->currentBrightness();
  this->dstBrightness = this->on ? this->userBrightness : 0;

  this->progress = 0.0f;

  this->syncState();

  return true;
}

void SimpleLightEntity::act(float delta) {
  MqttEntity::act(delta);

  if (this->progress < 1.0f) {
    this->progress += this->speed * (delta);
  }

  if (this->progress > 1.0f) {
    this->progress = 1.0f;
  }

  digitalWrite(this->pin, this->on ? HIGH : LOW);
}

#endif
