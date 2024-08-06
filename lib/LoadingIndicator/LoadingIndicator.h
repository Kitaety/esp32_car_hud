#include <Arduino.h>
#include <TFT_eSPI.h>

class LoadingIndicator {
   public:
    LoadingIndicator(TFT_eSPI *tft);
    void init(uint16_t x, uint16_t y, uint16_t r, uint16_t w);
    void update();

   private:
    TFT_eSPI *_tft;
    uint16_t _x,
        _y,
        _r,
        _w,
        _xCenter,
        _yCenter,
        _xText,
        _yText,
        _currentAngle = 0,
        _oldAngle = 0;
};
