#include <Arduino.h>
#include <TFT_eSPI.h>

class SpeedometerWidget {
   public:
    SpeedometerWidget(TFT_eSPI *tft);
    void init(uint16_t x, uint16_t y, uint16_t r, uint16_t measurementScaleColor = TFT_WHITE, uint16_t indicatorColor = TFT_RED, uint16_t warningSpeedValueColor = TFT_RED, uint16_t textColor = TFT_WHITE);
    void update(int32_t speed, int32_t rpm);

   private:
    void updateSpeed(int32_t speed);
    void updateRpm(int32_t rpm);
    void drawSpeedIndicators();
    void drawRpmArrow(int32_t rpm);
    void drawRpmText(int32_t rpm);
    void drawSpeedText(int32_t speed);
    void drawSpeedArrow(int32_t speed);
    void drawArc(uint16_t x, uint16_t y, uint16_t r, uint16_t w, int16_t startAngle, int16_t arcAngle, uint16_t color, bool smooth = true);
    void drawText(int16_t x, int16_t y, String text, uint color, uint8_t size = 3);

    TFT_eSPI *_tft;
    uint16_t _x,
        _y,
        _r,
        _xCenter,
        _yCenter,
        _textColor,
        _indicatorColor;

    uint16_t
        _measurementScaleColor,
        _warningSpeedValueColor,
        _speedArrowRadius = 0,
        _speedArrowAngle = 0,
        _oldSpeedArrowAngle = 0,
        _startSpeedAngle = 0,
        _speedAngle = 0,
        _speedIndicatorRadius = 0;

    uint16_t
        _speedTextAreaX,
        _speedTextAreaY,
        _speedTextKmPerHPositionX,
        _speedTextKmPerHPositionY,
        _speedTextKmPositionY;

    uint16_t _xRpm,
        _yRpm,
        _rpmArrowRadius = 0,
        _startRpmArrowAngle = 0,
        _rpmAngle = 0,
        _rpmArrowAngle = 0,
        _oldRpmArrowAngle = 0;
};
