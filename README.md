# Zaku Core

## About

This is code is used for controlling led on my Gundam models using automation's in [HomeAssistant](https://www.home-assistant.io/). First I wanted to use ESPHome for that but I wanted to have power on effects and different animations for ex. GN Drives.

## Requirements

* [platformio](https://platformio.org/)
* python 3
* HomeAssistant
* MQTT
* WeMos D1 R2 - ESP8266
* ESP32 board

## Setup mode

Search for WiFi network starting with name ZakuCore and connect to it, password is `archangel`. After connection visit `http://192.168.4.1`

# Add new board
You can define each new board in `platformio.ini`. Each platform should run `yarn release` before and script that generates pin layout. Pin layout is defined in `config/pinouts/*.json` files and is used by
webui light editor.

## Release

You can use docker to build releases:

```bash
docker-compose --file docker-compose.build.yaml --project-name zaku-core-release build
docker-compose --file docker-compose.build.yaml  --project-name zaku-core-release up
```

Or just by using platformio tool:

```bash
platformio run -e release:zaku-core:rev1
platformio run -e release:d1_mini
```

### Mosquitto development passwords

```yaml
admin: admin1234
client: client1234
```

## References

* https://pinout.xyz/pinout/3v3_power
* https://github.com/GitSquared/edex-ui
* https://blog.espressif.com/dfu-using-the-native-usb-on-esp32-s2-for-flashing-the-firmware-b2c4af3335f1
* https://all-free-download.com/free-vector/download/zaku_26617.html
* https://github.com/esphome/esphome/tree/dev/esphome/core
* https://www.home-assistant.io/docs/mqtt/discovery/
* https://roelofjanelsinga.com/articles/mqtt-discovery-with-an-arduino/
* https://github.com/bblanchon/ArduinoJson
* https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html#client
* https://github.com/Makuna/NeoPixelBus
* https://github.com/marvinroger/async-mqtt-client
* https://easings.net/
* https://github.com/nicolausYes/easing-functions/blob/master/src/easing.cpp
* https://jinja.palletsprojects.com/en/latest/templates/
* https://randomnerdtutorials.com/esp8266-nodemcu-ota-over-the-air-vs-code/
* https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
* https://github.com/Tvde1/WiFiPicker/blob/master/src/WiFiPicker.cpp
* https://version1-breakpoint1.arwes.dev/api
* https://arwes.dev/
* https://arwes.dev/develop/core
* https://tinkerman.cat/post/embed-your-website-in-your-esp8266-firmware-image
* https://www.npmjs.com/package/gulp-esbuild
* https://reactrouter.com/en/main/components/form
* https://github.com/mcauser/Fritzing-Part-WeMos-D1-Mini/tree/master/src
* https://esphome.github.io/esp-web-tools/
* https://webauthn.io/
* https://github.com/esp8266/ESPWebServer/blob/master/examples/SimpleAuthentification/SimpleAuthentification.ino
* https://www.onetransistor.eu/2019/04/https-server-on-esp8266-nodemcu.html
* https://github.com/me-no-dev/ESPAsyncWebServer/blob/2f784268f0a358741ee6384480d48656e159d726/src/WebServer.cpp#L96
* https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/examples/SimpleAuthentification/SimpleAuthentification.ino#L35
* https://www.freecodecamp.org/news/session-hijacking-and-how-to-stop-it-711e3683d1ac/
* https://github.com/ayushsharma82/ElegantOTA/blob/master/ui/src/App.vue
* https://github.com/2SmartCloud/2smart-cloud-esp32-led/blob/fbee1a51e0775f8a87807fc49593df64f73626b2/Firmware/lib/web_server/src/web_server.cpp#L350
* https://towardsdatascience.com/how-to-validate-your-json-using-json-schema-f55f4b162dce
* https://json-schema.org/learn/getting-started-step-by-step
* https://code.tutsplus.com/tutorials/validating-data-with-json-schema-part-1--cms-25343
* https://ajv.js.org/guide/getting-started.html#parsing-and-serializing-json
* https://primalcortex.wordpress.com/2020/05/05/esp8266nodejs-ecdh/
