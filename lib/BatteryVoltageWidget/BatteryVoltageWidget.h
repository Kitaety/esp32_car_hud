#include <Arduino.h>
#include <TFT_eSPI.h>

class BatteryVoltageWidget {
   public:
    BatteryVoltageWidget(TFT_eSPI *tft);
    void init(uint16_t x, uint16_t y);
    void update(float value);

   private:
    TFT_eSPI *_tft;
    uint16_t _x,
        _y;
};
