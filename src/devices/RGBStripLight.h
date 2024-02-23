#ifndef RGBStripLight_H
#define RGBStripLight_H

#include <pins_board.h>
#include "MultiEntity.h"
#include "RGBLightEntity.h"

class RGBStripLight : public MultiEntity {
  private:
    Adafruit_NeoPixel *controller_{nullptr};
    uint8_t pin;
  public:
    RGBStripLight(GundamConfig config, Relay *relay);
    ~RGBStripLight() {
      if (this->controller_) {
        delete this->controller_;
      }
    }
    void loadBlueprint(JsonObject &blueprint);
    void act(float delta);
};

RGBStripLight::RGBStripLight(GundamConfig config, Relay *relay) : MultiEntity(config, relay) {}

void RGBStripLight::loadBlueprint(JsonObject &blueprint) {
  strcpy(this->entityId, blueprint[EntityConsts::BlueprintKey::ID]);
  this->init(this->entityId, blueprint[EntityConsts::BlueprintKey::NAME], EntityConsts::ENTITY_LIGHT);

  Serial.println(F("Loading blueprint for RGB strip light"));
  if (this->controller_) {
    delete this->controller_;
  }

  String pinName = blueprint[EntityConsts::BlueprintKey::PIN];
  int pin = pinNameToId(pinName.c_str());
  Serial.printf("Pin name is: %s and id is: %i\n", pinName.c_str(), pin);
  JsonArray lights = blueprint[EntityConsts::BlueprintKey::LIGHTS].as<JsonArray>();
  Serial.println(F("Initializing light strip"));
  int pixelCount = 0;

  String prefixId = blueprint[EntityConsts::BlueprintKey::ID];
  Serial.println(F("Initializing each pixel"));

  for (size_t i = 0; i < lights.size(); i++) {
    JsonObject lightBlueprint = lights[i];
    Serial.printf("Pixel no: %i\n", i);

    RGBLightEntity *light = new RGBLightEntity(this->config, this->relay);
    light->loadBlueprint(prefixId.c_str(), lightBlueprint);
    pixelCount += light->pixels;
    this->add(light);
  }

  Serial.printf("Initialize light strip with size: %i\n", pixelCount);
  this->controller_ = new Adafruit_NeoPixel(pixelCount, pin, NEO_GRB + NEO_KHZ800);
  this->controller_->begin();
  Serial.println(F("DONE:"));
}

void RGBStripLight::act(float delta) {
  int offset = 0;
  for (size_t i = 0; i < this->size; i++) {
    auto light = (RGBLightEntity*)this->entities[i];
    light->act(delta);

    auto color = light->currentColor();
    for (size_t pi = 0; pi < light->pixels; pi++) {
      int pix = offset = pi;
      int hexColor = color.toInt();

      this->controller_->setPixelColor(pix, hexColor);
    }
    offset += light->pixels;
  }

  this->controller_->show();
}

#endif
