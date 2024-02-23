#ifndef EntityManager_H
#define EntityManager_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Configuration.h>
#include <devices/Entity.h>
#include <devices/SimpleLight.h>
#include <devices/RGBStripLight.h>

const char * ENTITIES_CONFIG = "/config/entities.db";
const int MAX_LIGHTS = 32;
const int DELTA_TICK_MS = 1000/48;

class EntityManager {
private:
  Relay *relay;
  GundamConfig config;
  Entity *entities[MAX_LIGHTS];
  unsigned int size;
  unsigned long deltaAcc; // 1.0f == 1 second, 0.5f == 500 ms
  unsigned long lastTick;
public:
  EntityManager();
  void setup(GundamConfig config, Relay *relay);
  void clear();
  // Save current entities
  void persist();

  // Load configuration
  void load();
  bool add(Entity *entity);
  void setup();
  bool receive(const char * topic, JsonObject &doc);
  bool tick();
  bool act();
  void collectTopics();
};

EntityManager::EntityManager() {
  this->size = 0;
  this->lastTick = millis();
}

void EntityManager::setup(GundamConfig config, Relay *relay) {
  this->config = config;
  this->relay = relay;
  this->lastTick = millis();
}

void EntityManager::collectTopics() {
  Serial.println("Entity manager, collect topics");
  for (unsigned int i = 0; i < this->size; i++) {
    this->entities[i]->collectTopic();
  }
}

void EntityManager::clear() {
  for (unsigned int i = 0; i < this->size; i++) {
    delete this->entities[i];
    this->entities[i] = NULL;
  }
  this->size = 0;
}

void EntityManager::persist() {

}

// Load configuration
void EntityManager::load() {
  File file = LittleFS.open(ENTITIES_CONFIG, "r");

  if (file) {
    Serial.printf("Loaded entities configuration: %s\n", ENTITIES_CONFIG);
    StaticJsonDocument<4096> doc;
    auto error = deserializeMsgPack(doc, file);

    if (error) {
      Serial.printf("Could not open entities config file: %s\n", error.c_str());
      return;
    }

    JsonArray entities = doc["entities"];

    Serial.printf("Found array with: %i entities\n", entities.size());
    for (size_t i = 0; i < entities.size(); i++) {
      JsonObject blueprint = entities[i];

      const char * kind = blueprint["kind"];
      Serial.printf("Configuring entity: %s\n", kind);
      Entity *entity;

      if (strcmp("SIMPLE_LED", kind) == 0) {
        Serial.println("Initialize SIMPLE_LED");
        entity = new SimpleLightEntity(this->config, this->relay);
      } else if (strcmp("RGB_LED", kind) == 0) {
        Serial.println("Initialize RGB_LED");
        entity = new RGBStripLight(this->config, this->relay);
      } else {
        Serial.printf("Skipping unsupported entity type: %s\n", kind);
        continue;
      }

      entity->loadBlueprint(blueprint);
      this->add(entity);
    }
  } else {
    Serial.printf("Missing entities configuration: %s\n", ENTITIES_CONFIG);
  }
}

bool EntityManager::add(Entity *entity) {
  if (this->size >= MAX_LIGHTS) {
    Serial.println(F("Exceeded max entities"));
    return false;
  }

  Serial.printf("Added entity: %s of type %s\n", entity->name, entity->type);
  this->entities[this->size++] = entity;
  return true;
}

void EntityManager::setup() {
  Serial.println(F("Configuring entities"));

  for (unsigned int i = 0; i < this->size; i++) {
    Entity *entity = this->entities[i];
    entity->setup();
  }

  Serial.println(F(" OK"));
}

bool EntityManager::receive(const char * topic, JsonObject &doc) {
  for (unsigned int i = 0; i < this->size; i++) {
    Entity *entity = this->entities[i];

    if (entity->receive(topic, doc)) {
      Serial.printf("Matched %s with entity id: %s \n", topic, entity->id);
      return true;
    }
  }

  return false;
}

bool EntityManager::tick() {
  unsigned long now = millis();

  this->deltaAcc += (now - this->lastTick);
  this->lastTick = now;

  // Serial.println(this->deltaAcc);

  return this->deltaAcc < DELTA_TICK_MS;
}

bool EntityManager::act() {
  for (unsigned int i = 0; i < this->size; i++) {
    Entity *entity = this->entities[i];
    if (entity->push()) {
      break;
    }
  }

  if (this->tick()) {
    // Serial.printf("Entities: %i\n", this->size);
    return false;
  }

  for (unsigned int i = 0; i < this->size; i++) {
    Entity *entity = this->entities[i];
    entity->act(this->deltaAcc / 1000.0f);
  }

  this->deltaAcc = 0;

  return true;
}

#endif
