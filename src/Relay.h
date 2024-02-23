#ifndef Relay_H
#define Relay_H

#include <ESPAsyncWebServer.h>
#include <ESPNetwork.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include <consts.h>

const int PAYLOAD_BUFFER_SIZE = 1024;

typedef std::function<void(const char * topic, JsonObject &doc)> RelayMessageHandler;
typedef std::function<void()> RelayReadyHandler;

class Relay {
  private:
    RelayMessageHandler _messageHandler;
    RelayReadyHandler _relayReadyHandler;
    RelayReadyHandler _relayDisconnectHandler;
    AsyncWebSocket *ws;
    AsyncMqttClient *mqtt;
    int messagesSent;

    void onWsEvent(AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void flush();
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
  public:
    Relay();

    void setup(ConfigurationManager *config);
    void connect();
    void disconnect();
    void setWill(char * topic);
    bool connected();
    bool uiConnected();
    void onMessage(RelayMessageHandler handler);
    void onReady(RelayReadyHandler handler);
    void onDisconnect(RelayReadyHandler handler);
    void registerHandler(AsyncWebServer * server);
    void publish(const char* topic, const char* payload);
    void publish(const char* topic, DynamicJsonDocument &doc);
    void publish(DynamicJsonDocument &doc);
    void subscribe(const char* topic);
    void unsubscribe(const char* topic);
    void loop();
};

Relay::Relay() {
  this->ws = new AsyncWebSocket("/api/ws");
  this->mqtt = new AsyncMqttClient();
  this->ws->onEvent([this](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
    this->onWsEvent(client, type, arg, data, len);
  });

  this->mqtt->onConnect([this](bool sessionPresent) {
    Serial.println("------- MQTT connected!");
    Serial.print(F("Subscribing to home assistant status: "));
    Serial.println(EntityConsts::TOPIC_HOME_ASSISTANT_STATUS);
    this->subscribe(EntityConsts::TOPIC_HOME_ASSISTANT_STATUS);
    this->_relayReadyHandler();
  });

  this->mqtt->onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
    this->onMqttDisconnect(reason);
  });

  this->mqtt->onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    this->onMqttMessage(topic, payload, properties, len, index, total);
  });
}

void Relay::setup(ConfigurationManager *configuration) {
  Serial.println("Relay setup");
  Serial.printf("Connecting to MQTT: %s:%i\n", configuration->broker.address, configuration->broker.port);
  this->mqtt->setCleanSession(true);
  this->mqtt->setClientId(WiFi.getHostname());
  this->mqtt->setServer(configuration->broker.address, configuration->broker.port);
  Serial.printf("Credentials: %s:%s\n", configuration->broker.username, configuration->broker.password);
  this->mqtt->setCredentials(configuration->broker.username, configuration->broker.password);
}

void Relay::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.print(F("Disconnected from MQTT: "));
  //TODO: create status method which casts enum to string and append it to broker information
  switch (reason) {
    case AsyncMqttClientDisconnectReason::TCP_DISCONNECTED:
      Serial.println(F(" TCP_DISCONNECTED"));
    break;
    case AsyncMqttClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
      Serial.println(F(" MQTT_UNACCEPTABLE_PROTOCOL_VERSION"));
    break;
    case AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED:
      Serial.println(F(" MQTT_IDENTIFIER_REJECTED"));
    break;
    case AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE:
      Serial.println(F(" MQTT_SERVER_UNAVAILABLE"));
    break;
    case AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS:
      Serial.println(F(" MQTT_MALFORMED_CREDENTIALS"));
    break;
    case AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED:
      Serial.println(F(" MQTT_NOT_AUTHORIZED"));
    break;
    case AsyncMqttClientDisconnectReason::ESP8266_NOT_ENOUGH_SPACE:
      Serial.println(F(" ESP8266_NOT_ENOUGH_SPACE"));
    break;
    default:
      uint8_t r = (uint8_t)reason;
      Serial.println(r);
  }

  this->_relayDisconnectHandler();
}

void Relay::setWill(char * topic) {
  Serial.printf("Last will: %s\n", topic);
  this->mqtt->setWill(topic, 1, false, EntityConsts::OFFLINE);
}

bool Relay::connected() {
  return this->mqtt->connected();
}

bool Relay::uiConnected() {
  return this->ws->count() > 0;
}

void Relay::connect() {
  this->mqtt->connect();
}

void Relay::disconnect() {
  this->mqtt->disconnect();
  this->ws->closeAll();
  this->ws->cleanupClients();
}

void Relay::onMessage(RelayMessageHandler handler) {
  this->_messageHandler = handler;
}

void Relay::onReady(RelayReadyHandler handler) {
  this->_relayReadyHandler = handler;
}

void Relay::onDisconnect(RelayReadyHandler handler) {
  this->_relayDisconnectHandler = handler;
}

void Relay::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.print(F("  [Topic]: "));
  Serial.println(topic);

  if (strcmp(topic, EntityConsts::TOPIC_HOME_ASSISTANT_STATUS) == 0) {
    Serial.println(F("[Status changed]"));
    Serial.println(F("Reconfiguring ZakuCore"));
    this->_relayReadyHandler();
    return;
  }

  Serial.print(F("  [Payload]: "));
  for (size_t i = 0; i < len; i++) {
    Serial.print(payload[i]);
  }
  Serial.println();

  StaticJsonDocument<PAYLOAD_BUFFER_SIZE> doc;
  if (DeserializationError::Ok == deserializeJson(doc, payload, len)) {
    JsonObject json = doc.as<JsonObject>();
    this->_messageHandler(topic, json);
  } else {
    Serial.println("Invalid json");
  }
}

void Relay::onWsEvent(AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  StaticJsonDocument<PAYLOAD_BUFFER_SIZE> doc;

  switch(type) {
    case AwsEventType::WS_EVT_DATA:
      if (DeserializationError::Ok == deserializeJson(doc, data, len)) {
        const char * topic = doc["topic"];
        JsonObject payload = doc["payload"].as<JsonObject>();

        // relay topic to mqtt home assistant
        this->_messageHandler(topic, payload);

        // if (this->mqtt->connected()) { // relay message to mqtt for home assistant
        //   char buffer[PAYLOAD_BUFFER_SIZE];

        //   size_t json_size = serializeJson(payload, buffer);
        //   Serial.printf("Publishing to mqtt: %s\n", topic);
        //   Serial.print(buffer);
        //   this->mqtt->publish(topic, 0, false, buffer, json_size);
        // }
      } else {
        Serial.println("Invalid json");
      }
    break;

    case AwsEventType::WS_EVT_CONNECT:
      Serial.printf("Websocket client connected: %s\n", client->remoteIP().toString().c_str());
      this->_relayReadyHandler();
    break;

    case AwsEventType::WS_EVT_DISCONNECT:
      Serial.printf("Websocket client disconnected: %s\n", client->remoteIP().toString().c_str());
    break;

    default:
    break;
  }
}

void Relay::registerHandler(AsyncWebServer * server) {
  server->addHandler(this->ws);
}

void Relay::flush() {
  if (this->ws->availableForWriteAll()) {
    return;
  }

  Serial.print("Wait for messages to be flushed: ");
  while (!this->ws->availableForWriteAll()) {
    Serial.print(".");
    delay(33);
  }
  delay(150);
  Serial.println(" DONE!");
}

void Relay::publish(const char* topic, const char* payload) {
  if (this->mqtt->connected()) {
    this->mqtt->publish(topic, 1, false, payload);
  }

  if (this->uiConnected()) {
    // send message over websocket
    char buffer[PAYLOAD_BUFFER_SIZE];
    DynamicJsonDocument message(PAYLOAD_BUFFER_SIZE);
    message["payload"] = payload;
    message["topic"] = topic;

    serializeJson(message, buffer);
    this->ws->printfAll(buffer);
  }

  this->flush();
}

void Relay::publish(const char* topic, DynamicJsonDocument &doc) {
  char buffer[PAYLOAD_BUFFER_SIZE];
  if (this->mqtt->connected()) {
    size_t json_size = serializeJson(doc, buffer);
    // Serial.printf("Publishing to mqtt: %s\n", topic);
    // Serial.print(buffer);
    this->mqtt->publish(topic, 0, false, buffer, json_size);
  }

  if (this->uiConnected()) {
    // send message over websocket formatted in topic, payload struct
    // Serial.println("Publishing to WS: ");
    auto payload = doc.as<JsonObject>();
    DynamicJsonDocument message(PAYLOAD_BUFFER_SIZE);
    message["payload"] = payload;
    message["topic"] = topic;

    serializeJson(message, buffer);
    // Serial.println(buffer);
    this->ws->printfAll(buffer);
  }

  this->flush();
}

void Relay::publish(DynamicJsonDocument &doc) {
  if (this->uiConnected()) {
    // send message over websocket formatted in topic, payload struct
    // Serial.println("Publishing to WS: ");
    char buffer[PAYLOAD_BUFFER_SIZE];
    serializeJson(doc, buffer);
    // Serial.println(buffer);
    this->ws->printfAll(buffer);
  }

  this->flush();
}

void Relay::subscribe(const char* topic) {
  if (this->mqtt->connected()) {
    this->mqtt->subscribe(topic, 0);
  }

  this->flush();
}

void Relay::unsubscribe(const char* topic) {
  if (this->mqtt->connected()) {
    this->mqtt->unsubscribe(topic);
  }

  this->flush();
}

void Relay::loop() {
  this->ws->cleanupClients();
  this->flush();
}

#endif
