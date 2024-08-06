#include "BatteryVoltageWidget.h"

#include "BatteryVoltageIcon.h"

BatteryVoltageWidget::BatteryVoltageWidget(TFT_eSPI *tft) {
    _tft = tft;
    init(0, 0);
}

void BatteryVoltageWidget::init(uint16_t x, uint16_t y) {
    _x = x;
    _y = y;
}

void BatteryVoltageWidget::update(float value) {
    _tft->setTextSize(2);
    _tft->pushImage(_x, _y, batteryVoltageIconWidth, batteryVoltageIconWidth, batteryVoltageIcon);
    _tft->setCursor(_x + batteryVoltageIconWidth, _y);
    _tft->printf("% 2.1f", value);
    _tft->setCursor(0, 0);
}
