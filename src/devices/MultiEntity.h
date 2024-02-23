#ifndef LightEntity_H
#define LightEntity_H

#include <Adafruit_NeoPixel.h>
#include "Entity.h"
#include "MqttEntity.h"

const int MAX_ENTITIES = 10;

/**
 * This class wraps multiple entities as one. Mainly used for RGB led strips
*/
class MultiEntity : public Entity {
  protected:
    Entity *entities[MAX_ENTITIES];
    size_t size;

    void clear() {
      Serial.printf("Clear sub entities: %i\n", this->size);
      for (size_t i = 0; i < this->size; i++) {
        delete this->entities[i];
        this->entities[i] = NULL;
      }
      this->size = 0;
    }

    bool add(Entity *entity) {
      if (this->size >= MAX_ENTITIES) {
        Serial.println(F("Exceeded max entities"));
        return false;
      }

      this->entities[this->size++] = entity;
      return true;
    }

  public:
    MultiEntity(GundamConfig config, Relay *relay) : Entity(config, relay) {
      this->size = 0;
    }

    void collectTopic() {
      Serial.println(F("Collect topic for Multi entity!"));
      for (size_t i = 0; i < this->size; i++) {
        this->entities[i]->collectTopic();
      }
    }

    void setup() {
      Serial.println(F("Multi Entity delay setup"));

      for (size_t i = 0; i < this->size; i++) {
        this->entities[i]->setup();
      }
    }

    bool push() {
      for (size_t i = 0; i < this->size; i++) {
        if (this->entities[i]->push()) {
          return true;
        }
      }

      return false;
    }

    void syncState() {
      for (size_t i = 0; i < this->size; i++) {
        this->entities[i]->syncState();
      }
    }

    bool receive(const char * topic, JsonObject &doc) {
      for (size_t i = 0; i < this->size; i++) {
        auto entity = this->entities[i];
        if (entity->receive(topic, doc)) {
          Serial.printf("Matched %s with sub entity id: %s \n", topic, entity->id);
          return true;
        }
      }

      return false;
    }

    virtual void act(float delta) {
      for (size_t i = 0; i < this->size; i++) {
        this->entities[i]->act(delta);
      }
    }

    virtual void unsubscribe() {
      for (size_t i = 0; i < this->size; i++) {
        this->entities[i]->unsubscribe();
      }
    }

    ~MultiEntity() {
      this->clear();
    }
};

#endif
