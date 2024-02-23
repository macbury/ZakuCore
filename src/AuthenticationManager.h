#ifndef AuthenticationManager_H
#define AuthenticationManager_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

namespace Authentication {
  const char * COOKIE_NAME = "_ZakuCore=";
  const char * FILE_PATH = "/config/session.json";
  const char * DEFAULT_PASSWORD = "archangel";
  const int BUFFER_SIZE = 1024;
  const byte MAX_SESSIONS = 4;
  const byte PASSWORD_LENGTH = 32;
  const byte SID_LENGTH = 24;

  const char * PASSWORD_KEY = "p";
  const char * SIDS_KEY = "s";
}

/// @brief All logic required for user management
class AuthenticationManager {
  private:
    char sids[Authentication::MAX_SESSIONS][Authentication::SID_LENGTH + 1]; // REMEMBER! make array of bytes larger, C requires null terminators!
    void reset();
    bool getSidFromCookie(String &cookie, String &out);
  public:
    char password[Authentication::PASSWORD_LENGTH + 1];

    AuthenticationManager();
    /// @brief Load sessions from file
    void load();
    /// @brief Reset sessions to default state
    void cleanup();
    /// @brief Save all sessions to flash memory
    void persist();

    /// @brief Check if password is valid
    /// @param passwordToValidate
    /// @return
    bool signIn(const char * passwordToValidate);
    bool updatePassword(String currentPassword, String nextPassword);
    /// @brief Load session id from request.
    /// @param request
    /// @param out
    /// @return
    bool getSidFromRequest(AsyncWebServerRequest *request, String &out);
    /// @brief Destroy session
    /// @param sid
    void signOut(const char * sid);
    bool authenticate(const char * sid);
    /// @brief Create new session, and persist it in FLASH memory
    /// @return new session hash
    String create();
    String buildCookie(String sid);
};

AuthenticationManager::AuthenticationManager() {
  strcpy(this->password, Authentication::DEFAULT_PASSWORD);

  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    strcpy(this->sids[i], "");
  }
}

String AuthenticationManager::create() {
  randomSeed(millis());

  //TODO: it would be nicer to generate a fingerprint using eliptic curve
  String sid = "";

  for (size_t i = 0; i < Authentication::SID_LENGTH; i++) {
    sid += String(random(0xf), HEX);
  }

  //TODO: This is ugly solution, needs rewrite
  char oldSids[Authentication::MAX_SESSIONS][Authentication::SID_LENGTH + 1];
  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    strcpy(oldSids[i], "");
  }

  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    strcpy(oldSids[i], this->sids[i]);
  }

  strcpy(this->sids[0], "");
  strcpy(this->sids[0], sid.c_str());

  for (size_t i = 1; i < Authentication::MAX_SESSIONS - 1; i++) {
    strcpy(this->sids[i], oldSids[i-1]);
  }

  this->persist();
  return sid;
}

String AuthenticationManager::buildCookie(String sid) {
  String cookie = String(Authentication::COOKIE_NAME) + sid +";HttpOnly;";
  return cookie;
}

bool AuthenticationManager::authenticate(const char * currentSid) {
  if (strlen(currentSid) < Authentication::SID_LENGTH) {
    Serial.printf("Invalid size of sid: %s\n", currentSid);
    return false;
  }

  // char sidToCheck[Authentication::SID_LENGTH];
  // strcpy(sidToCheck, currentSid);
  // Serial.printf("sidToCheck size: %i\n", strlen(sidToCheck));

  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    if (strlen(this->sids[i]) == 0) {
      continue;
    }

    // Serial.printf("Checking sid: %s len: %i \n", this->sids[i], strlen(this->sids[i]));

    if (strcmp(currentSid, this->sids[i]) == 0) {
      Serial.printf("Found session: %s\n", this->sids[i]);
      return true;
    }
  }

  Serial.printf("Missing sid: %s\n", currentSid);

  return false;
}

void AuthenticationManager::signOut(const char * sidToDestroy) {
  Serial.printf("Cleaning up session: %s\n", sidToDestroy);
  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    auto sid = this->sids[i];
    if (strcmp(sid, sidToDestroy) == 0) {
      strcpy(this->sids[i], "");
    }
  }

  this->persist();
}

bool AuthenticationManager::updatePassword(String currentPassword, String nextPassword) {
  if (!currentPassword.equals(this->password)) {
    return false;
  }

  this->reset();
  strcpy(this->password, nextPassword.c_str());
  this->persist();

  return true;
}

bool AuthenticationManager::getSidFromCookie(String &cookie, String &out) {
  int start = cookie.indexOf(Authentication::COOKIE_NAME);
  if (start < 0) {
    return false;
  }

  start += strlen(Authentication::COOKIE_NAME);
  int end = start;
  for (unsigned int i = start; i < cookie.length(); i++) {
    end = i;
    if (cookie.charAt(i) == ';') {
      break;
    }
  }

  String sid = cookie.substring(start, end-1);
  out.clear();
  out.concat(sid);
  out.replace("\n", "");

  // Serial.println("Side size from cookie:");
  // Serial.println(out.length());

  return true;
}

bool AuthenticationManager::getSidFromRequest(AsyncWebServerRequest *request, String &out) {
  if (!request->hasHeader("Cookie")) {
    Serial.println("Missing cookie");
    return false;
  }

  String cookie = request->getHeader("Cookie")->toString();

  return this->getSidFromCookie(cookie, out);
}

bool AuthenticationManager::signIn(const char * passwordToValidate) {
  Serial.printf("Checking password: %s == %s\n", this->password, passwordToValidate);
  return strcmp(this->password, passwordToValidate) == 0;
}

void AuthenticationManager::load() {
  Serial.printf("Loading sids: %s\n", Authentication::FILE_PATH);
  File file = LittleFS.open(
    Authentication::FILE_PATH,
    "r"
    #ifdef USE_ESP32
      ,true
    #endif
  );

  if (file) {
    StaticJsonDocument<Authentication::BUFFER_SIZE> config;
    DeserializationError error = deserializeJson(config, file);

    if (error) {
      Serial.println(F("Failed to read authentication file, using default configuration"));
      this->cleanup();
      return;
    }

    strcpy(this->password, config[Authentication::PASSWORD_KEY].as<String>().c_str());
    Serial.printf("Loaded password: %s\n", this->password);

    JsonArray sids = config[Authentication::SIDS_KEY];
    byte totalSids = sids.size() < Authentication::MAX_SESSIONS ? sids.size() : Authentication::MAX_SESSIONS;
    Serial.printf("Total sids: %i\n", totalSids);

    for (byte i = 0; i < totalSids; i++) {
      String sid = sids[i];
      if (sid.length() > 0) {
        strcpy(this->sids[i], sid.c_str());

        Serial.printf("Loaded sid: %i with content %s\n", i, this->sids[i]);
      } else {
        Serial.printf("Skipped sid: %i\n", i);
      }
    }
  } else {
    Serial.printf("Could not load file: %s\n", Authentication::FILE_PATH);
    this->cleanup();
  }
}

void AuthenticationManager::persist() {
  Serial.println(F("Saving auth settings"));

  DynamicJsonDocument config(Authentication::BUFFER_SIZE);
  config[Authentication::PASSWORD_KEY] = String(this->password);

  auto sids = config.createNestedArray(Authentication::SIDS_KEY);
  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    const char * sid = this->sids[i];
    if (strlen(sid) > 0) {
      sids.add(String(sid));
    }
  }

  File file = LittleFS.open(
    Authentication::FILE_PATH,
    "w"
    #ifdef USE_ESP32
      ,true
    #endif
  );

  // char buffer[Authentication::BUFFER_SIZE];
  if (serializeJson(config, file) == 0) {
    Serial.print(F("Failed to save:"));
    Serial.println(Authentication::FILE_PATH);
  } else {
    Serial.print("Finished persisting: ");
    Serial.println(Authentication::FILE_PATH);
  }

  // Serial.println(buffer);

  file.close();
}

void AuthenticationManager::reset() {
  Serial.println(F("Cleaning up the authentication"));
  strcpy(this->password, Authentication::DEFAULT_PASSWORD);
  for (size_t i = 0; i < Authentication::MAX_SESSIONS; i++) {
    strcpy(this->sids[i], "");
  }
}

void AuthenticationManager::cleanup() {
  this->reset();
  this->persist();
}

#endif
