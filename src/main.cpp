#include <Arduino.h>
#include <GundamManager.h>
#include <Web.h>
#include <consts.h>
#include <pins_board.h>

GundamManager gundam;
Web web;

void setup() {
  Serial.begin(115200);
  Serial.println(F("[Booted]"));
  Serial.printf("Version: %s\n", Version::Name);

  LittleFS.begin(
    #ifdef USE_ESP32
      true
    #endif
  );

  enablePins();
  randomSeed(millis());

  gundam.setup();
  web.setup(&gundam);
}

void loop() {
  gundam.loop();
  delay(1); // IMPORTANT! give some time for cpu to run all async shit
}
