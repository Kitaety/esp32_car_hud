#include <Arduino.h>
// #include <BatteryVoltageWidget.h>
#include <ELMduino.h>
#include <IndicatorWithIconWidget.h>
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
IndicatorWithIconWidget batteryVoltageWidget = IndicatorWithIconWidget(&tft, BATTERY_ICON);
IndicatorWithIconWidget coolantTempertureWidget = IndicatorWithIconWidget(&tft, FUEL_ICON);
IndicatorWithIconWidget fuelRateWidghet = IndicatorWithIconWidget(&tft, COOLANT_TEMPERATURE_ICON);

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
    batteryVoltageWidget.init(210, 10);
    coolantTempertureWidget.init(210, 28);
    fuelRateWidghet.init(210, 46);
    
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
        coolantTempertureWidget.update(obd2Manger.engineCoolantTemp);

        if(obd2Manger.speed > 0) {
            fuelRateWidghet.update(obd2Manger.fuelRatePer100km);
        } else {
            fuelRateWidghet.update(obd2Manger.fuelRate);
        }

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
