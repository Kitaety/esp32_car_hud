#include "SimpleSpeedometerWidget.h"

#define WIDTH 180

#define CHAR_HEIGHT_SIZE_7 49
#define CHAR_HEIGHT_SIZE_5 40

SimpleSpeedometerWidget::SimpleSpeedometerWidget(TFT_eSPI *tft) {
    _tft = tft;
    init(0, 0);
}

void SimpleSpeedometerWidget::init(uint16_t x, uint16_t y) {
    _x = x;
    _y = y;
}

void SimpleSpeedometerWidget::update(int32_t speed, int32_t rpm) {
    if (_oldSpeed != speed || _firstUpdate) {
        drawSpeedText(speed);
        _oldSpeed = speed;
    }
    if (_oldRpm != rpm || _firstUpdate) {
        drawRpmText(rpm);
        _oldRpm = rpm;
    }

    _firstUpdate = false;
}

void SimpleSpeedometerWidget::drawSpeedText(int32_t speed) {
    String speedString = String(speed);

    _tft->fillRect(_x, _y, WIDTH, CHAR_HEIGHT_SIZE_7, TFT_BLACK);
    drawText(_x, _y, speedString, 7);
}

void SimpleSpeedometerWidget::drawRpmText(int32_t rpm) {
    String rpmString = String(rpm);
    uint16_t _rpmdTextPositionY = _y + CHAR_HEIGHT_SIZE_7 + 10;

    _tft->fillRect(_x, _rpmdTextPositionY, WIDTH, CHAR_HEIGHT_SIZE_5, TFT_BLACK);
    drawText(_x, _rpmdTextPositionY, rpmString, 5);
}

void SimpleSpeedometerWidget::drawText(int16_t x, int16_t y, String text, uint8_t size) {
    _tft->setCursor(x, y);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK, true);
    _tft->setTextWrap(false);
    _tft->setTextSize(size);
    _tft->print(text);
}
