#include "IndicatorWithIconWidget.h"

#include "BatteryVoltageIcon.h"
#include "FuelLevelIcon.h"
#include "CoolantTemperatureIcon.h"

IndicatorIconData getIndicatorIconData(IndicatorIcon icon) {
    IndicatorIconData iconData;
    switch (icon) {
        case BATTERY_ICON:
            iconData.h = batteryVoltageIconHeigth;
            iconData.w = batteryVoltageIconWidth;
            iconData.icon = batteryVoltageIcon;
            break;
        case FUEL_ICON:
            iconData.h = fuelLevelIconHeigth;
            iconData.w = fuelLevelIconWidth;
            iconData.icon = fuelLevelIcon;
            break;
        case COOLANT_TEMPERATURE_ICON:
            iconData.h = coolantTemperatureIconHeigth;
            iconData.w = coolantTemperatureIconWidth;
            iconData.icon = coolantTemperatureIcon;
            break;
    }

    return iconData;
}

IndicatorWithIconWidget::IndicatorWithIconWidget(TFT_eSPI* tft, IndicatorIcon icon) {
    _tft = tft;
    _iconData = getIndicatorIconData(icon);
    init(0, 0);
}

void IndicatorWithIconWidget::init(uint16_t x, uint16_t y) {
    _x = x;
    _y = y;
}

void IndicatorWithIconWidget::drawIcon() {
    _tft->pushImage(_x, _y, _iconData.h, _iconData.w, _iconData.icon);
    _tft->setCursor(_x + _iconData.w + 5, _y);
}

void IndicatorWithIconWidget::update(float value) {
    if (_fValue != value || _firstUpdate) {
        _tft->setTextSize(2);
        drawIcon();
        _tft->printf("% 2.1f", value);
        _tft->setCursor(0, 0);
        _fValue = value;
        _firstUpdate = false;
    }
}

void IndicatorWithIconWidget::update(uint32_t value) {
    if (_i32Value != value || _firstUpdate) {
        _tft->setTextSize(2);
        drawIcon();
        _tft->printf("%d", value);
        _tft->setCursor(0, 0);
        _i32Value = value;
        _firstUpdate = false;
    }
}
