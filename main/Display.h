#include <TFT_eSPI.h>
#include "Arduino.h"

#define BLACK TFT_BLACK
#define WHITE TFT_WHITE
#define RED TFT_RED
#define YELLOW TFT_YELLOW
#define BLUE TFT_BLUE
#define NAVY TFT_NAVY
#define DARKGREEN TFT_DARKGREEN
#define DARKCYAN TFT_DARKCYAN
#define MAROON TFT_MAROON
#define PURPLE TFT_PURPLE
#define OLIVE TFT_OLIVE
#define LIGHTGREY TFT_LIGHTGREY
#define DARKGREY TFT_DARKGREY
#define GREEN TFT_GREEN
#define CYAN TFT_CYAN
#define MAGENTA TFT_MAGENTA           
#define ORANGE TFT_ORANGE      
#define GREENYELLOW TFT_GREENYELLOW 
#define PINK TFT_PINK
#define BROWN TFT_BROWN       
#define GOLD TFT_GOLD
#define SILVER TFT_SILVER
#define SKYBLUE TFT_SKYBLUE
#define VIOLET TFT_VIOLET

class Display {
  public:
    void init(int8_t rotation = 0);
    void drawText(int16_t x, int16_t y, String text, uint color, uint8_t size = 3);
    void drawSpeedometer(uint16_t speed, uint16_t rpm);
    void clear();
  private:
    void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint color);

    void drawSpeedometerSpeedIndicators();
    void drawSpeedometerSpeedText(uint16_t speed, uint color);
    void drawSpeedometerArrow(uint16_t speed);
    void drawSpeedometerRpm(uint16_t rpm);
};