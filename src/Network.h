#ifndef Network_H
#define Network_H

#include <Ticker.h>
#include <ESPNetwork.h>
#include <Configuration.h>
#include <AuthenticationManager.h>

typedef std::function<void()> NetworkConnectedHandler;
typedef std::function<void(int id, const char * ssid, int rssi, int encryptionType)> FoundNetworkHandler;

enum NetworkScan {
  Done = 0, Failed = 1, Running = 2
};

class Network {
  private:
    Ticker resendNetworks;
    Ticker reconfigureNetwork;
    int emittedNetwork = 0;
    // handler function for found network
    AuthenticationManager *auth;
    DNSServer *dns;
    ConfigurationManager *configuration;
    NetworkConnectedHandler _onNetworkConnectHandler;
    FoundNetworkHandler _foundNetworkHandler;

    #ifdef USE_ESP8266
      WiFiEventHandler wifiConnectHandler;
      WiFiEventHandler wifiDisconnectHandler;
      WiFiEventHandler wifiAPConnectHandler;
      WiFiEventHandler wifiAPDisconnectHandler;
    #endif

    void startApConfig();
    void reconfigureMDNS();

    void onWifiConnect();
    void onWifiDisconnect();
    /// @brief Fetch network information and sen it over relay
    void checkNextNetwork();
  public:

    Network();
    /// @brief start async scan
    /// @return
    NetworkScan scan();
    NetworkScan scanState();
    void setup(ConfigurationManager *configuration, AuthenticationManager *auth);
    void onConnect(NetworkConnectedHandler handler);
    void onNetworkFound(FoundNetworkHandler handler);
    void connect();
    void loop();
    bool connectedToRouter();
};

Network::Network() {
  this->dns = new DNSServer();
  // Reset WIFI

  #ifdef USE_ESP32
    WiFi.disconnect(true, true);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  #endif
}

NetworkScan Network::scan() {
  if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
    this->resendNetworks.detach();
    Serial.println(F("Starting scanning for networks"));

    WiFi.scanDelete();
    delay(100);
    int res = WiFi.scanNetworks(true, false);
    if (res == WIFI_SCAN_FAILED) {
      WiFi.disconnect();
      WiFi.scanNetworks(true, false);
    }
    this->emittedNetwork = 0;
    return NetworkScan::Running;
  } else {
    Serial.println(F("Scan in progress, waiting for it to finish"));
    return this->scanState();
  }
}

NetworkScan Network::scanState() {
  switch (WiFi.scanComplete()) {
    case WIFI_SCAN_RUNNING:
      return NetworkScan::Running;
    case WIFI_SCAN_FAILED:
      return NetworkScan::Failed;
    default:
      return NetworkScan::Done;
  }
}

void Network::onNetworkFound(FoundNetworkHandler handler) {
  this->_foundNetworkHandler = handler;
}

void Network::onConnect(NetworkConnectedHandler handler) {
  this->_onNetworkConnectHandler = handler;
}

void Network::setup(ConfigurationManager *configuration, AuthenticationManager *auth) {
  this->configuration = configuration;
  this->auth = auth;

  #ifdef USE_ESP8266
    this->wifiConnectHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& event) {
      this->onWifiConnect();
    });
    this->wifiAPConnectHandler = WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected& event) {
      Serial.println(F("New client connected to Access Point for configuration!"));
    });
    this->wifiAPDisconnectHandler = WiFi.onSoftAPModeStationDisconnected([this](const WiFiEventSoftAPModeStationDisconnected& event) {
      Serial.println(F("Disconnected from WiFi..."));
    });
    this->wifiDisconnectHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event) {
      Serial.println(F("Client configurator disconnected..."));
      this->onWifiDisconnect();
    });
  #endif

  #ifdef USE_ESP32
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
      this->onWifiConnect();
    }, ARDUINO_EVENT_WIFI_STA_GOT_IP);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
      this->onWifiDisconnect();
    }, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  #endif
}

void Network::onWifiConnect() {
  if (WiFi.isConnected()) {
    Serial.print(F("[Connected to Wi-Fi]  "));
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.localIP());
    this->reconfigureNetwork.detach();
    #ifdef USE_ESP8266
      this->reconfigureMDNS();
    #endif
    #ifdef USE_ESP32
      reconfigureNetwork.once(10, +[](Network *instance) {
        instance->reconfigureMDNS();
      }, this);
    #endif
  } else {
    Serial.println(F("[Not connected to wifi...]"));
  }

  this->dns->stop();

  this->_onNetworkConnectHandler();
}

void Network::onWifiDisconnect() {
  MDNS.end();

  switch (WiFi.getMode()) {
    case WIFI_AP_STA:
      Serial.println("Disconnected from AP");
    break;

    case WIFI_STA:
      Serial.println("Disconnected from Router");
      this->startApConfig();
    break;
  }
}

void Network::connect() {
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  if (strlen(this->configuration->wifi.ssid) == 0) {
    Serial.println("Missing configuration for wifi.");
    WiFi.setHostname(this->configuration->gundam.hostname);
    this->startApConfig();
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->configuration->wifi.ssid, this->configuration->wifi.password);
    WiFi.setHostname(this->configuration->gundam.hostname);

    Serial.printf("Connecting to Wi-Fi: %s and password: %s\n", this->configuration->wifi.ssid, this->configuration->wifi.password);
    Serial.println(WiFi.SSID());
  }

  Serial.print(F("Hostname: "));
  Serial.println(WiFi.getHostname());
}

void Network::startApConfig() {
  #ifdef USE_ESP8266
    String ssid = "ZakuCore" + String(ESP.getChipId());
  #endif
  #ifdef USE_ESP32
    uint64_t chip_id = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
    uint16_t chip = (uint16_t)(chip_id >> 32);
    char ssid[23];
    snprintf(ssid, 23, "ZakuCore %04X%08X", chip, (uint32_t)chip_id);
  #endif

  Serial.print(F("Starting configuration access point: "));
  Serial.println(ssid);
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(ssid, this->auth->password, 5);
  Serial.printf("Access point password is: %s\n", this->auth->password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print(F("AP IP address: "));
  Serial.println(myIP);

  this->dns->setErrorReplyCode(DNSReplyCode::NoError);
  this->dns->start(53, "setup.local", myIP);

  this->reconfigureMDNS();
}

void Network::checkNextNetwork() {
  auto numberOfNetworks = WiFi.scanComplete();

  if (numberOfNetworks > 0 && this->emittedNetwork < numberOfNetworks) {
    auto ssid = WiFi.SSID(this->emittedNetwork);
    auto rssi = WiFi.RSSI(this->emittedNetwork);
    auto encryption = WiFi.encryptionType(this->emittedNetwork);

    Serial.printf("Found ssid: %s\n", ssid.c_str());

    this->_foundNetworkHandler(this->emittedNetwork, ssid.c_str(), rssi, encryption);

    this->emittedNetwork++;

    if (numberOfNetworks == emittedNetwork) {
      Serial.println(F("All networks sent!"));

      // resend everything again after 15 seconds
      #ifdef USE_ESP8266
        resendNetworks.once(15, [this]() {
          this->emittedNetwork = 0;
        });
      #endif
      #ifdef USE_ESP32
        resendNetworks.once(15, +[](Network *instance) {
          instance->emittedNetwork = 0;
        }, this);
      #endif
    }
  }
}

void Network::loop() {
  this->checkNextNetwork();

  if (this->connectedToRouter()) {
    return;
  }

  this->dns->processNextRequest();
}

bool Network::connectedToRouter() {
  return WiFi.getMode() == WIFI_STA;
}

void Network::reconfigureMDNS() {
  MDNS.end();
  delay(100);

  Serial.printf("[Start mDNS] hostname: %s for ip: %s\n", this->configuration->gundam.hostname, WiFi.localIP().toString().c_str());
  if (!MDNS.begin(this->configuration->gundam.hostname)) {
    Serial.println("[Error setting up MDNS responder!]");
  }

  Serial.println("[mDNS responder started registering http service]");
  MDNS.addService("http", "tcp", 80);
}

#endif
