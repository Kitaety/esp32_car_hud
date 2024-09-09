#include "IndicatorWithIconWidget.h"

#include "BatteryVoltageIcon.h"
#include "CoolantTemperatureIcon.h"

#define CHAR_WIDTH_SIZE_3 17
#define CHAR_HEIGHT_SIZE_3 21
#define WIDTH 100

IndicatorIconData getIndicatorIconData(IndicatorIcon icon) {
    IndicatorIconData iconData;
    switch (icon) {
        case BATTERY_ICON:
            iconData.h = batteryVoltageIconHeigth;
            iconData.w = batteryVoltageIconWidth;
            iconData.icon = batteryVoltageIcon;
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

void IndicatorWithIconWidget::init(uint16_t x, uint16_t y ) {
    _x = x;
    _y = y;
}

void IndicatorWithIconWidget::drawIcon() {
    _tft->pushImage(_x, _y, _iconData.w, _iconData.h, _iconData.icon);
}

void IndicatorWithIconWidget::drawText(String text) {
    uint32_t y = _y + _iconData.h + 4;
    uint32_t xText = _x + (WIDTH / 2) - (text.length() * CHAR_WIDTH_SIZE_3 / 2);

    _tft->fillRect(_x, y, WIDTH, CHAR_WIDTH_SIZE_3, TFT_BLACK);
    _tft->setCursor(xText, y);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK, true);
    _tft->setTextWrap(false);
    _tft->setTextSize(3);
    _tft->print(text);
}

void IndicatorWithIconWidget::update(float value) {
    if (_fValue != value || _firstUpdate) {
        drawIcon();
        char buffer[4];
        sprintf(buffer, "%2.1f", value);
        drawText(buffer);
        _fValue = value;
        _firstUpdate = false;
    }
}

void IndicatorWithIconWidget::update(uint32_t value) {
    if (_i32Value != value || _firstUpdate) {
        drawIcon();
        char buffer[4];
        sprintf(buffer, "%d", value);
        drawText(buffer);
        _i32Value = value;
        _firstUpdate = false;
    }
}
