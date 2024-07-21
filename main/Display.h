#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

class Display {
  public:
    Display(int8_t DC, int8_t RST, int8_t CS, int8_t MOSI, int8_t SCLK);

    void init(uint16_t wd, uint16_t ht, int8_t rotation = 0);
    void drawText(int16_t x, int16_t y, String text, uint color, uint8_t size = 3);
    void drawSpeedometer(uint16_t speed, uint16_t rpm);
    void clear();
  private:
    int8_t _csPin, _dcPin, _rstPin, _mosiPin, _sclkPin;
    Adafruit_ST7789 *tft;

    void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint color);

    void drawSpeedometerSpeedIndicators();
    void drawSpeedometerSpeedText(uint16_t speed, uint color);
    void drawSpeedometerArrow(uint16_t speed);
    void drawSpeedometerRpm(uint16_t rpm);
};