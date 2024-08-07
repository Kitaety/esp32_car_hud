#include <Arduino.h>
#include <BatteryVoltageWidget.h>
#include <ELMduino.h>
#include <LoadingIndicator.h>
#include <OBDIIManager.h>
#include <SpeedometerWidget.h>
#include <TFT_eSPI.h>

#include "MyAPServer.h"
#include "PreferencesManager.h"

#define SPEEDOMETER_POSITION_X 10
#define SPEEDOMETER_POSITION_Y 10
#define SPEEDOMETER_RADIUS 90

const char *mac = "41:50:05:05:88:2A";

MyAPServer server;
PreferencesManager prefsManager;

TFT_eSPI tft = TFT_eSPI();
SpeedometerWidget speedometerWidget = SpeedometerWidget(&tft);
BatteryVoltageWidget batteryVoltageWidget = BatteryVoltageWidget(&tft);

void loading(void *pvParamerters);
void serverStart(void *pvParamerters);
void displayManage(void *pvParamerters);
void updateOBDIIData(void *pvParamerters);

ELM327 elm327;
BluetoothSerial bt;
OBDIIManager obd2Manger = OBDIIManager(elm327, bt);

TaskHandle_t updateOBDIIDataTask;

void startUpdateOBDIIDataTask() {
    xTaskCreatePinnedToCore(updateOBDIIData, "updateOBDIIData", 4096, NULL, 0, &updateOBDIIDataTask, 0);
}

void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    TaskHandle_t loadingTask;
    xTaskCreatePinnedToCore(loading, "loading", 8192, NULL, 0, &loadingTask, 0);
    vTaskDelete(loadingTask);
    tft.fillScreen(TFT_BLACK);
    speedometerWidget.init(SPEEDOMETER_POSITION_X, SPEEDOMETER_POSITION_Y, SPEEDOMETER_RADIUS);
    batteryVoltageWidget.init(200, 10);

    xTaskCreatePinnedToCore(serverStart, "serverStart", 4096, NULL, 0, NULL, 1);
    xTaskCreatePinnedToCore(displayManage, "displayManage", 8192, NULL, 0, NULL, 0);
    startUpdateOBDIIDataTask();
}

void loop() {}

void loading(void *pvParamerters) {
    LoadingIndicator indicator = LoadingIndicator(&tft);
    indicator.init(100, 60, 60, 5);
    while (true) {
        indicator.update();
        vTaskDelay(50);
    }
}

void serverStart(void *pvParamerters) {
    JsonDocument wifiConfig = prefsManager.getWifiConfig();

    server.config(wifiConfig["ssid"], wifiConfig["password"]);

    server.get("/wifi", []() { return prefsManager.getWifiConfig(); });

    server.post(
        "/wifi",
        [](JsonDocument body) {
            prefsManager.updateWifiConfig(body["ssid"], body["password"]);
        },
        []() { prefsManager.reboot(); });

    server.start();
}

void displayManage(void *pvParamerters) {
    while (true) {
        speedometerWidget.update(obd2Manger.speed, obd2Manger.rpm);
        batteryVoltageWidget.update(obd2Manger.batteryVoltage);
        vTaskDelay(1);
    }
}

void updateOBDIIData(void *pvParamerters) {
    if (!obd2Manger.connect(mac)) {
        vTaskDelete(updateOBDIIDataTask);
    }

    while (true) {
        obd2Manger.update();
        vTaskDelay(1);
    }
}
