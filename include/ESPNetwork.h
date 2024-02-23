#ifdef USE_ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <DNSServer.h>
  #include <ESPAsyncTCP.h>
  #include <AsyncMqttClient.h>
#endif

#ifdef USE_ESP32
  #include <WiFi.h>
  #include <DNSServer.h>
  #include <ESPmDNS.h>
#endif
