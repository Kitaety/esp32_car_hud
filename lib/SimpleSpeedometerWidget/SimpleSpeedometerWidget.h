#include <Arduino.h>
#include <TFT_eSPI.h>

class SimpleSpeedometerWidget {
   public:
    SimpleSpeedometerWidget(TFT_eSPI *tft);
    void init(uint16_t x, uint16_t y);
    void update(int32_t speed, int32_t rpm);

   private:
    void drawRpmText(int32_t rpm);
    void drawSpeedText(int32_t speed);
    void drawText(int16_t x, int16_t y, String text, uint8_t size = 3);

    TFT_eSPI *_tft;
    uint16_t _x,
        _y,
        _rpmTextPositionY,
        _oldSpeed = 0,
        _oldRpm = 0;
    bool _firstUpdate = true;
};
