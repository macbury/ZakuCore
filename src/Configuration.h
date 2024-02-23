#ifndef Configuration_H
#define Configuration_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <consts.h>

struct GundamConfig {
  char name[64];
  char model[128];
  char hostname[64];

  void getAvailabilityTopic(char* output) {
    strcpy(output, EntityConsts::PREFIX);
    strcat(output, hostname);
    strcat(output, EntityConsts::SLASH);
    strcat(output, EntityConsts::AVAILABILITY);
  }
};

struct BrokerConfig {
  char address[64];
  char username[64];
  char password[64];
  int port;
};

struct WiFiCredentials {
  char ssid[64];
  char password[64];
};

class ConfigurationManager {
  public:
    GundamConfig gundam;
    BrokerConfig broker;
    WiFiCredentials wifi;

    ConfigurationManager();
    void load();
    void cleanup();
    void save();
    void updateWifi(String ssid, String password);
  private:
    void resetWifiCredentials();
    void resetBrokerConfig();
    void resetGundamConfig();
    void loadBrokerConfig();
    void loadWifiCredentials();
    void loadGundamConfig();
    void saveWiFiCredentials();
    void saveBrokerConfig();
    void saveGundamConfig();
    void write(StaticJsonDocument<256> &doc, const char * filename);
};

ConfigurationManager::ConfigurationManager() {}

void ConfigurationManager::write(StaticJsonDocument<256> &doc, const char * filename) {
  #ifdef USE_ESP32
    File file = LittleFS.open(filename, "w", true);
  #endif

  #ifdef USE_ESP8266
    File file = LittleFS.open(filename, "w");
  #endif

  if (serializeJson(doc, file) == 0) {
    Serial.print(F("Failed to save:"));
    Serial.println(filename);
  } else {
    Serial.print("Finished persisting: ");
    Serial.println(filename);
  }

  file.close();
}

void ConfigurationManager::cleanup() {
  Serial.println(F("Removing configuration"));
  LittleFS.format();
}

void ConfigurationManager::load() {
  this->loadBrokerConfig();
  this->loadWifiCredentials();
  this->loadGundamConfig();
}

void ConfigurationManager::save() {
  this->saveBrokerConfig();
  this->saveWiFiCredentials();
  this->saveGundamConfig();
}

void ConfigurationManager::resetGundamConfig() {
  #ifdef USE_ESP8266
    String name = "zaku-core" + String(ESP.getChipId());
  #endif
  #ifdef USE_ESP32
    uint64_t chip_id = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
    uint16_t chip = (uint16_t)(chip_id >> 32);
    char name[23];
    snprintf(name, 23, "zaku-core%04X%08X", chip, (uint32_t)chip_id);
  #endif
  String hostname = String(name); //TODO How to build hostname
  String unknown = "unknown";

  strlcpy(
    this->gundam.hostname,
    hostname.c_str(),
    sizeof(this->gundam.hostname)
  );

  strlcpy(
    this->gundam.name,
    hostname.c_str(),
    sizeof(this->gundam.name)
  );

  strlcpy(
    this->gundam.model,
    unknown.c_str(),
    sizeof(this->gundam.model)
  );
}

void ConfigurationManager::loadGundamConfig() {
  File file = LittleFS.open(GUNDAM_CONFIG_FILE, "r");

  if (file) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
      Serial.println(F("Failed to read file, using default configuration"));
      this->resetGundamConfig();
      return;
    }

    Serial.println(F("Loaded gundam configuration file"));
    String hostname = doc["hostname"] | "";
    String name = doc["name"] | "";
    String model = doc["model"] | "";

    strlcpy(
      this->gundam.hostname,
      hostname.c_str(),
      sizeof(this->gundam.hostname)
    );

    strlcpy(
      this->gundam.name,
      name.c_str(),
      sizeof(this->gundam.name)
    );

    strlcpy(
      this->gundam.model,
      model.c_str(),
      sizeof(this->gundam.model)
    );
  } else {
    Serial.print(F("Failed to open file: "));
    Serial.println(GUNDAM_CONFIG_FILE);
    this->resetGundamConfig();
  }
}

void ConfigurationManager::resetWifiCredentials() {
  String empty = "";

  strlcpy(
    this->wifi.password,
    empty.c_str(),
    sizeof(this->wifi.password)
  );

  strlcpy(
    this->wifi.ssid,
    empty.c_str(),
    sizeof(this->wifi.ssid)
  );
}

void ConfigurationManager::resetBrokerConfig() {
  String localIp = "0.0.0.0";
  String empty = "";

  this->broker.port = 1883;

  strlcpy(
    this->broker.address,
    localIp.c_str(),
    sizeof(this->broker.address)
  );

  strlcpy(
    this->broker.username,
    empty.c_str(),
    sizeof(this->broker.username)
  );

  strlcpy(
    this->broker.password,
    empty.c_str(),
    sizeof(this->broker.password)
  );
}

void ConfigurationManager::loadBrokerConfig() {
  File file = LittleFS.open(BROKER_CONFIG_FILE, "r");

  if (file) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
      Serial.println(F("Failed to read file, using default configuration"));
      this->resetBrokerConfig();
      return;
    }

    Serial.println(F("Loaded broker connection configuration file"));
    int port = doc["port"] | 1883;
    String address = doc["address"] | "";
    String username = doc["username"] | "";
    String password = doc["password"] | "";

    strlcpy(
      this->broker.address,
      address.c_str(),
      sizeof(this->broker.address)
    );

    strlcpy(
      this->broker.username,
      username.c_str(),
      sizeof(this->broker.username)
    );

    strlcpy(
      this->broker.password,
      password.c_str(),
      sizeof(this->broker.password)
    );

    this->broker.port = port;
  } else {
    Serial.print(F("Failed to open file: "));
    Serial.println(BROKER_CONFIG_FILE);
    this->resetBrokerConfig();
  }
}

void ConfigurationManager::loadWifiCredentials() {
  File file = LittleFS.open(CONNECTION_CONFIG_FILE, "r");

  if (file) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
      Serial.println(F("Failed to read file, using default configuration"));
      this->resetWifiCredentials();
      return;
    }

    Serial.println(F("Loaded wifi configuration file"));
    String ssid = doc["ssid"] | "";
    String password = doc["password"] | "";

    strlcpy(
      this->wifi.ssid,
      ssid.c_str(),
      sizeof(this->wifi.ssid)
    );

    strlcpy(
      this->wifi.password,
      password.c_str(),
      sizeof(this->wifi.password)
    );
  } else {
    Serial.print(F("Failed to open file: "));
    Serial.println(CONNECTION_CONFIG_FILE);
    this->resetWifiCredentials();
  }
}

void ConfigurationManager::saveWiFiCredentials() {
  StaticJsonDocument<256> doc;

  doc["ssid"] = this->wifi.ssid;
  doc["password"] = this->wifi.password;

  this->write(doc, CONNECTION_CONFIG_FILE);
}

void ConfigurationManager::saveGundamConfig() {
  StaticJsonDocument<256> doc;

  doc["name"] = this->gundam.name;
  doc["hostname"] = this->gundam.hostname;
  doc["model"] = this->gundam.model;
  this->write(doc, GUNDAM_CONFIG_FILE);
}

void ConfigurationManager::saveBrokerConfig() {
  StaticJsonDocument<256> doc;

  doc["address"] = this->broker.address;
  doc["password"] = this->broker.password;
  doc["username"] = this->broker.username;
  doc["port"] = this->broker.port;

  this->write(doc, BROKER_CONFIG_FILE);
}


#endif
