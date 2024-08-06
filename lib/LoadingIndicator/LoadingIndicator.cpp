#include "LoadingIndicator.h"

#define INDICATOR_STEP 10

LoadingIndicator::LoadingIndicator(TFT_eSPI *tft) {
    _tft = tft;
    init(0, 0, 90, 5);
}

void LoadingIndicator::init(uint16_t x, uint16_t y, uint16_t r, uint16_t w) {
    _x = x;
    _y = y;
    _r = r;
    _w = r - w;

    _xCenter = _x + _r;
    _yCenter = _y + _r;
    _xText = _xCenter - 21;  // width 3.5 characters font size 1
    _yText = _yCenter - 4;   // half height character font size 1
}

void LoadingIndicator::update() {
    _tft->drawSmoothArc(_xCenter, _yCenter, _r, _w, _oldAngle, (_oldAngle + INDICATOR_STEP) % 360, TFT_BLACK, TFT_BLACK, false);
    _tft->drawSmoothArc(_xCenter, _yCenter, _r, _w, _currentAngle, (_currentAngle + _w) % 360, TFT_WHITE, TFT_BLACK, false);

    _tft->setCursor(_xText, _yText);
    _tft->print("LOADING");
    _tft->setCursor(0, 0);

    _oldAngle = _currentAngle;
    _currentAngle = (_currentAngle + INDICATOR_STEP) % 360;
}
