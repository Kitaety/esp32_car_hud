#include <TFT_eSPI.h>

class My_TFT_eSPI : public TFT_eSPI {
public:
  My_TFT_eSPI()
    : TFT_eSPI() {}
  void drawArc(int8_t x, int8_t y, int8_t r, int8_t w, int16_t startAngle, int16_t arcAngle, uint16_t color) {
    int16_t endAngle = (startAngle + arcAngle) % 360;
    TFT_eSPI::drawArc(x, y, r, r - w, startAngle, endAngle, color, TFT_BLACK);
  }
};
