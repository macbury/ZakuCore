#ifndef RGBLightEntity_H
#define RGBLightEntity_H

#include "MqttEntity.h"
#include "effects.h"

class RGBLightEntity : public MqttEntity {
  private:
    RGBW dst;
    RGBW src;
    float speed;
    float progress;
  protected:
    EFFECT_FUNCTION;

    void prepareStatePayload(DynamicJsonDocument &doc);
    void configureSensor(DynamicJsonDocument &doc);
    void setEffect(EFFECT_FUNCTION);

  public:
    bool on;
    byte pixels;

    RGBLightEntity(GundamConfig config, Relay *relay);
    void loadBlueprint(const char * prefixId, JsonObject &blueprint);
    bool off();
    void setColor(RGBW color) ;
    RGBW currentColor();
    RGBW dstStateColor();
    bool receive(const char * topic, JsonObject &doc);
    void act(float delta);
};

RGBLightEntity::RGBLightEntity(GundamConfig config, Relay *relay) : MqttEntity(config, relay) {}

void RGBLightEntity::loadBlueprint(const char * prefixId, JsonObject &blueprint) {
  strcpy(this->entityId, blueprint[EntityConsts::BlueprintKey::ID]);

  char prefixedId[64];
  strcpy(prefixedId, prefixId);
  strcat(prefixedId, "___");
  strcat(prefixedId, this->entityId);
  this->init(prefixedId, blueprint[EntityConsts::BlueprintKey::NAME], EntityConsts::ENTITY_LIGHT);

  if (blueprint.containsKey(EntityConsts::BlueprintKey::PIXELS)) {
    this->pixels = blueprint[EntityConsts::BlueprintKey::PIXELS];
  } else {
    this->pixels = 1;
  }

  this->on = false;
  this->speed = blueprint["speed"] | 0.5f;
  this->setEffect(Effects::fade);
  byte brightness = blueprint["brightness"] | 255;

  if (blueprint.containsKey("color")) {
    auto color = blueprint["color"];
    byte red = color["r"];
    byte green = color["g"];
    byte blue = color["b"];

    this->dst = RGBW { red, green, blue, brightness };
  } else {
    this->dst = RGBW { 255, 255, 255, brightness };
  }

  this->src = RGBW(dst);
}

bool RGBLightEntity::off() {
  return !this->on;
}

void RGBLightEntity::setColor(RGBW color) {
  this->dst = color;
}

RGBW RGBLightEntity::currentColor() {
  return this->processEffect(this->progress, this->src, this->dstStateColor());
}

RGBW RGBLightEntity::dstStateColor() {
  return this->on ? this->dst : RGBW::zero();
}

bool RGBLightEntity::receive(const char * topic, JsonObject &doc) {
  if (!this->matchedTopic(topic)) {
    return false;
  }

  Serial.print(F("Light received payload: "));
  Serial.println(this->name);

  this->on = doc[F("state")] == F("ON");

  //TODO: if this is before off and this is on set Effect to power on if part is boot
  //TODO: otherwise use just fade
  // if it was on and changed to off then change to fade

  RGBW next_color = RGBW(this->dst);
  if (doc.containsKey(F("brightness"))) {
    next_color.brightness = doc[F("brightness")];
  }

  const char * COLOR_KEY = "color";
  if (doc.containsKey(COLOR_KEY)) {
    next_color.red = doc[COLOR_KEY][F("r")];
    next_color.green = doc[COLOR_KEY][F("g")];
    next_color.blue = doc[COLOR_KEY][F("b")];
  }

  this->src = this->currentColor();
  this->dst = next_color;
  this->progress = 0.0f;

  this->syncState();

  return true;
}

void RGBLightEntity::act(float delta) {
  MqttEntity::act(delta);

  if (this->progress < 1.0f) {
    this->progress += this->speed * (delta);
  }

  if (this->progress > 1.0f) {
    this->progress = 1.0f;
  }
}

void RGBLightEntity::prepareStatePayload(DynamicJsonDocument &doc) {
  doc[F("state")] = this->on ? EntityConsts::ON : EntityConsts::OFF;
  doc[F("color_mode")] = F("rgb");
  doc[F("brightness")] = this->dst.brightness;

  JsonObject color = doc.createNestedObject(F("color"));
  color[F("r")] = this->dst.red;
  color[F("g")] = this->dst.green;
  color[F("b")] = this->dst.blue;
}

void RGBLightEntity::configureSensor(DynamicJsonDocument &doc) {
  doc[F("rgb")] = true;
  doc[F("brightness")] = true;
}

void RGBLightEntity::setEffect(EFFECT_FUNCTION) {
  this->processEffect = processEffect;
}

#endif
