#include "MyAPServer.h"
#include "PreferencesManager.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

bool turnLed = false;
MyAPServer server;
PreferencesManager prefsManager;

void ServerStart(void *pvParamerters);
void LedManage(void *pvParamerters);

void setup() {
  xTaskCreatePinnedToCore(ServerStart, "ServerStart", 4096, NULL, 0, NULL, 1);
  xTaskCreatePinnedToCore(LedManage, "LedManage", 128, NULL, 0, NULL, 0);
}

void loop() {}

void ServerStart(void *pvParamerters) {
  JsonDocument wifiConfig = prefsManager.getWifiConfig();

  server.config(wifiConfig["ssid"], wifiConfig["password"]);

  server.get("/led", []() {
    JsonDocument body;
    body["status"] = turnLed;
    return body;
  });

  server.post("/led", [](JsonDocument body) {
    turnLed = body["status"];
  });

  server.get("/wifi", []() {
    return prefsManager.getWifiConfig();
  });

  server.post("/wifi", [](JsonDocument body) {
    prefsManager.updateWifiConfig(body["ssid"], body["password"]);
  },[](){prefsManager.reboot();});

  server.start();
}

void LedManage(void *pvParamerters) {
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {
    digitalWrite(LED_BUILTIN, turnLed);
    vTaskDelay(500);
  }
}