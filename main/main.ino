#include "MyAPServer.h"
#include "secrets.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

bool turnLed = false;
MyAPServer server;

void ServerStart(void *pvParamerters);
void LedManage(void *pvParamerters);

void setup() {
  xTaskCreate(ServerStart, "ServerStart", 4096, NULL, 0, NULL);
  xTaskCreate(LedManage, "LedManage", 2048, NULL, 0, NULL);
}

void loop() {}

void ServerStart(void *pvParamerters) {
  server.config(SSID, PASSWORD, AP_LOCAL_IP);

  server.get("/1", []() {
    turnLed = true;
    return "ON";
  });

  server.get("/0", []() {
    turnLed = false;
  });

  server.start();
}

void LedManage(void *pvParamerters) {
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {
    digitalWrite(LED_BUILTIN, turnLed);
    vTaskDelay(500);
  }
}