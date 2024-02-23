#ifndef GundamManager_H
#define GundamManager_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPNetwork.h>

#include <Ticker.h>
#include <AuthenticationManager.h>
#include <AsyncMqttClient.h>
#include <Configuration.h>
#include <EntityManager.h>
#include <devices/Board.h>
#include <pins_board.h>
#include <Network.h>
#include <consts.h>

#include "Relay.h"

class GundamManager {
  private:
    unsigned short version;
    unsigned short fps;
    bool pendingEntityReload;
    char availabilityTopic[128];

    BoardEntity *board;

    Ticker boardInfoTicker;
    Ticker mqttReconnectTimer;
    Ticker fpsTicker;
    Ticker restartEspTicker;

    void reloadEntities();
    void setupBoard();
    void setLastWill();
    void pushOnline();
    void pushReload();
    void syncBoard();
    void trackFPS();

    void onRelayReady();
    void onMessage(const char * topic, JsonObject &doc);
  public:
    Network *network;
    AuthenticationManager *auth;
    AsyncWebServer *server;
    Relay *relay;
    EntityManager *entities;
    ConfigurationManager *configuration;

    GundamManager();
    void pushStatus();
    void requestEntityReload();
    void setup();
    void loop();
    void restart();
};

GundamManager::GundamManager() {
  this->auth = new AuthenticationManager();
  this->version = 1;
  this->network = new Network();
  this->server = new AsyncWebServer(80);
  this->configuration = new ConfigurationManager();
  this->entities = new EntityManager();
  this->relay = new Relay();
}

void GundamManager::pushOnline() {
  // Serial.printf("Sending online status: %s\n", this->availabilityTopic);
  this->relay->publish(this->availabilityTopic, EntityConsts::ONLINE);
}

void GundamManager::pushReload() {
  DynamicJsonDocument message(PAYLOAD_BUFFER_SIZE);
  message["reload"] = version;
  this->relay->publish(message);
}

void GundamManager::restart() {
  Serial.println(F("Restarting ESP in 5 seconds..."));
  restartEspTicker.detach();

  restartEspTicker.once(5, []() {
    Serial.println(F("Restarting..."));
    WiFi.disconnect(true);
    ESP.restart();
  });
}

void GundamManager::setLastWill() {
  this->relay->setWill(this->availabilityTopic);
}

void GundamManager::trackFPS() {
  // Serial.printf("FPS: %i\n", this->fps);
  DynamicJsonDocument message(512);
  message["fps"] = this->fps;
  this->relay->publish(message);
  this->fps = 0;
}

void GundamManager::syncBoard() {
  this->board->syncState();
  this->pushOnline();
  this->pushStatus();
}

void GundamManager::setup() {
  this->auth->load();
  this->configuration->load();
  this->network->setup(this->configuration, this->auth);
  this->relay->setup(this->configuration);
  this->entities->setup(this->configuration->gundam, this->relay);
  this->pendingEntityReload = true;

  this->configuration->gundam.getAvailabilityTopic(this->availabilityTopic);

  #ifdef USE_ESP8266
    boardInfoTicker.attach(EntityConsts::SYNC_BOARD_STATE_EVERY, [this]() {
      this->syncBoard();
    });
  #endif
  #ifdef USE_ESP32
    boardInfoTicker.attach(EntityConsts::SYNC_BOARD_STATE_EVERY, +[](GundamManager *instance) {
      instance->syncBoard();
    }, this);
  #endif

  this->network->onConnect([this]() {
    this->relay->connect();
  });

  this->network->onNetworkFound([this](int id, const char * ssid, int rssi, int encryption) {
    DynamicJsonDocument message(PAYLOAD_BUFFER_SIZE);
    auto network = message.createNestedObject("network");
    network["id"] = id;
    network["ssid"] = ssid;
    network["rssi"] = rssi;
    network["encryption"] = encryption;

    this->relay->publish(message);
  });

  Serial.println(F("[Starting http server at port 80]"));

  this->setLastWill();
  this->relay->onDisconnect([this]() {
    if (WiFi.isConnected()) {
      #ifdef USE_ESP8266
        this->mqttReconnectTimer.once(EntityConsts::RECONNECT_EVERY, [this]() {
          this->relay->connect();
        });
      #endif

      #ifdef USE_ESP32
        this->mqttReconnectTimer.once(EntityConsts::RECONNECT_EVERY, +[](GundamManager *instance) {
          instance->relay->connect();
        }, this);
      #endif
    }
  });

  this->relay->onReady([this]() { this->onRelayReady(); });
  this->relay->onMessage([this](const char * topic, JsonObject &object) {
    this->onMessage(topic, object);
  });

  this->network->connect();
  this->server->begin();

  #ifdef USE_ESP8266
    fpsTicker.attach(1, [this]() {
      this->trackFPS();
    });
  #endif
  #ifdef USE_ESP32
    fpsTicker.attach(1, +[](GundamManager *instance) {
      instance->trackFPS();
    }, this);
  #endif
}

void GundamManager::loop() {
  this->relay->loop();

  if (this->pendingEntityReload) {
    this->pendingEntityReload = false;
    this->reloadEntities();
  }

  if (this->entities->act()) {
    this->fps++;
  }
  this->network->loop();
}

void GundamManager::onRelayReady() {
  Serial.println(F("--- New connection ---"));
  this->pushStatus();
  this->entities->setup();
  this->pushOnline();
  this->pushReload();
}

void GundamManager::setupBoard() {
  this->board = new BoardEntity(this->configuration->gundam, this->relay);
  this->entities->add(this->board);
}

void GundamManager::requestEntityReload() {
  this->pendingEntityReload = true;
}

void GundamManager::reloadEntities() {
  Serial.println(F("[Reloading entities]"));
  this->entities->clear();
  delay(100); // wait 1 second, in meantime cpu will execute mqtt async logic
  Serial.println("Setup board");
  this->setupBoard();
  this->entities->load();

  this->entities->setup();
  if (this->relay->connected()) {
    this->pushOnline();
  } else {
    Serial.println(F("Skipping setup, mqtt disconnected"));
  }
  this->version++;
  this->pushReload();
  Serial.println(F("[DONE]"));
}

void GundamManager::onMessage(const char* topic, JsonObject &doc) {
  if (this->entities->receive(topic, doc)) {
    Serial.println(F("    [Success]"));
  } else {
    Serial.println(F("    [Could not match topic]"));
  }
}

#endif
