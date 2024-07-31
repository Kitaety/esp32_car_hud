#include <Arduino.h>
#include <ESP32Time.h>
#include "MyAPServer.h"
#include "PreferencesManager.h"

#include <TFT_eSPI.h>
#include <SpeedometerWidget.h>

#define SPEEDOMETER_POSITION_X 10
#define SPEEDOMETER_POSITION_Y 10
#define SPEEDOMETER_RADIUS 90

// ESP32Time rtc;
ESP32Time rtc(0);

bool turnLed = false;
MyAPServer server;
PreferencesManager prefsManager;

TFT_eSPI tft = TFT_eSPI();
SpeedometerWidget speedometerWidget = SpeedometerWidget(&tft);

void ServerStart(void *pvParamerters);
void LedManage(void *pvParamerters);
void DisplayManage(void *pvParamerters);

void setup()
{
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);

	speedometerWidget.init(SPEEDOMETER_POSITION_X, SPEEDOMETER_POSITION_Y, SPEEDOMETER_RADIUS);

	xTaskCreatePinnedToCore(ServerStart, "ServerStart", 4096, NULL, 0, NULL, 1);
	xTaskCreatePinnedToCore(LedManage, "LedManage", 1024, NULL, 0, NULL, 0);
	xTaskCreatePinnedToCore(DisplayManage, "DisplayManage", 8192, NULL, 0, NULL, 0);
}

void loop()
{
}

void ServerStart(void *pvParamerters)
{
	JsonDocument wifiConfig = prefsManager.getWifiConfig();

	server.config(wifiConfig["ssid"], wifiConfig["password"]);

	server.get("/led", []()
			   {
    JsonDocument body;
    body["status"] = turnLed;
    return body; });

	server.post("/led", [](JsonDocument body)
				{ turnLed = body["status"]; });

	server.get("/wifi", []()
			   { return prefsManager.getWifiConfig(); });

	server.post(
		"/wifi", [](JsonDocument body)
		{ prefsManager.updateWifiConfig(body["ssid"], body["password"]); },
		[]()
		{
			prefsManager.reboot();
		});

	server.start();
}

void LedManage(void *pvParamerters)
{
	pinMode(LED_BUILTIN, OUTPUT);

	for (;;)
	{
		digitalWrite(LED_BUILTIN, turnLed);
		vTaskDelay(500);
	}
}

void DisplayManage(void *pvParamerters)
{
	// display.init(1);
	// display.clear();

	bool a = true;
	uint16_t i = 0;
	for (;;)
	{
		// display.drawText(228, 5, rtc.getTime("%H:%M"), CYAN);
		// display.drawSpeedometer(i, i * 25);
		speedometerWidget.update(i, i * 25);

		if (i == 0)
		{
			a = true;
		}
		if (i == 300)
		{
			i = 200;
			a = false;
		}

		if (a)
		{
			i++;
		}
		else
		{
			i--;
		}

		vTaskDelay(5);
	}
}