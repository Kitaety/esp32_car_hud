#include "Display.h"
#include <SPI.h>

#define DEG2RAD 0.0174532925

#define SPEED_MAX_VALUE 300

#define BLACK_COLOR ST77XX_BLACK
#define WHITE_COLOR ST77XX_WHITE
#define RED_COLOR ST77XX_RED
#define YELLOW_COLOR ST77XX_YELLOW

Display::Display(int8_t DC, int8_t RST, int8_t CS, int8_t MOSI, int8_t SCLK) {
  _dcPin = DC;
  _csPin = CS;
  _rstPin = RST;
  _sclkPin = SCLK;
  _mosiPin = MOSI;
}

void Display::init(uint16_t width, uint16_t height, int8_t rotation) {
  SPIClass* spi = new SPIClass(HSPI);
  spi->begin(_sclkPin, -1, _mosiPin, _csPin);
  tft = new Adafruit_ST7789(spi, _csPin, _dcPin, _rstPin);

  tft->setSPISpeed(80000000);
  tft->init(width, height, SPI_MODE0);
  tft->setRotation(rotation);

  drawSpeedometer(0, 0);
}

void Display::clear() {
  tft->fillScreen(BLACK_COLOR);
}

void Display::fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint color) {
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);

  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  for (int i = start_angle; i < start_angle + seg_count; i += 1) {

    float sx2 = cos((i + 1 - 90) * DEG2RAD);
    float sy2 = sin((i + 1 - 90) * DEG2RAD);

    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    tft->fillTriangle(x0, y0, x1, y1, x2, y2, color);
    tft->fillTriangle(x1, y1, x2, y2, x3, y3, color);

    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}

#define SPEED_CENTER_X 110
#define SPEED_CENTER_Y 110
#define SPEED_START_ANGLE 225
#define SPEED_ARC_ANGLE 270
#define SPEED_ARC_RADIUS 90
#define SPEED_ARC_WIDTH 2

void Display::drawSpeedometer(uint16_t speed, uint16_t rpm) {
  uint16_t truncateSpeed = speed % (SPEED_MAX_VALUE + 1);

  fillArc(SPEED_CENTER_X, SPEED_CENTER_Y, SPEED_START_ANGLE, SPEED_ARC_ANGLE, SPEED_ARC_RADIUS, SPEED_ARC_RADIUS, SPEED_ARC_WIDTH, WHITE_COLOR);
  drawSpeedometerSpeedIndicators();
  drawSpeedometerArrow(truncateSpeed);
  drawSpeedometerSpeedText(truncateSpeed, WHITE_COLOR);
  drawSpeedometerRpm(rpm);
}

int warningSpeedSegment[3] = { 2, 3, 6 };
#define SPEED_SEGMENT_COUNT 16
#define SPEED_SEGMENT_ANGLE_SPLIT (270 / (SPEED_SEGMENT_COUNT - 1))
#define SPEED_SEGMENT_WIDTH 7
#define SPEED_SEGMENT_ANGLE 2
#define SPEED_SEGMENT_RADIUS (SPEED_ARC_RADIUS - SPEED_ARC_WIDTH)

void Display::drawSpeedometerSpeedIndicators() {
  int currentWarningSpeedSegment = 0;

  for (int i = 0; i < SPEED_SEGMENT_COUNT; i++) {
    int colorSpeedSegment = WHITE_COLOR;
    if (i == warningSpeedSegment[currentWarningSpeedSegment]) {
      colorSpeedSegment = RED_COLOR;
      currentWarningSpeedSegment++;
    }

    int angle = (SPEED_START_ANGLE + (i * SPEED_SEGMENT_ANGLE_SPLIT)) % 360;

    if (i > 0) {
      angle -= SPEED_SEGMENT_ANGLE / 2;
    }

    fillArc(SPEED_CENTER_X, SPEED_CENTER_Y, angle, SPEED_SEGMENT_ANGLE, SPEED_SEGMENT_RADIUS, SPEED_SEGMENT_RADIUS, SPEED_SEGMENT_WIDTH, colorSpeedSegment);
  }
}

#define CHAR_WIDTH_SIZE_2 11
#define CHAR_HEIGHT_SIZE_2 16
#define CHAR_WIDTH_SIZE_3 17
#define CHAR_HEIGHT_SIZE_3 21

#define SPEED_TEXT_KM_H_POSITION_Y (SPEED_CENTER_Y + CHAR_HEIGHT_SIZE_2)
#define SPEED_TEXT_KM_H_POSITION_X (SPEED_CENTER_X - 2 * CHAR_WIDTH_SIZE_2)
#define SPEED_TEXT_POSITION_Y (SPEED_TEXT_KM_H_POSITION_Y - 5 - CHAR_HEIGHT_SIZE_3)
#define SPEED_TEXT_POSITION_X(length) (SPEED_CENTER_X - (length * CHAR_WIDTH_SIZE_3) / 2)

#define SPEED_TEXT_AREA_Y SPEED_TEXT_POSITION_Y
#define SPEED_TEXT_AREA_X SPEED_TEXT_POSITION_X(3)
#define SPEED_TEXT_AREA_WIDTH CHAR_WIDTH_SIZE_3 * 3
#define SPEED_TEXT_AREA_HEIGHT CHAR_HEIGHT_SIZE_3

#define SPEED_ARROW_WIDTH 15
#define SPEED_ARROW_RADIUS (SPEED_SEGMENT_RADIUS - SPEED_ARROW_WIDTH)

void Display::drawSpeedometerSpeedText(uint16_t speed, uint color) {
  tft->fillRect(SPEED_TEXT_AREA_X, SPEED_TEXT_AREA_Y, SPEED_TEXT_AREA_WIDTH, SPEED_TEXT_AREA_HEIGHT, BLACK_COLOR);
  String speedString = String(speed);
  drawText(SPEED_TEXT_POSITION_X(speedString.length()), SPEED_TEXT_POSITION_Y, speedString, color, 3);
  drawText(SPEED_TEXT_KM_H_POSITION_X, SPEED_TEXT_KM_H_POSITION_Y, "KM/H", color, 2);
}

uint16_t oldSpeedArrowAngle = 0;

void Display::drawSpeedometerArrow(uint16_t speed) {
  uint16_t speedArrowAngle = SPEED_ARC_ANGLE * speed / SPEED_MAX_VALUE;
  
  //draw down speed
  if (oldSpeedArrowAngle > speedArrowAngle) {
    uint16_t angle = SPEED_START_ANGLE + speedArrowAngle;
    fillArc(SPEED_CENTER_X, SPEED_CENTER_Y, angle, oldSpeedArrowAngle - speedArrowAngle, SPEED_ARROW_RADIUS, SPEED_ARROW_RADIUS, SPEED_ARROW_WIDTH, BLACK_COLOR);
  }

  //draw up speed
  if (speedArrowAngle > oldSpeedArrowAngle || speedArrowAngle == 0) {
    uint16_t angle = SPEED_START_ANGLE + oldSpeedArrowAngle;
    fillArc(SPEED_CENTER_X, SPEED_CENTER_Y, angle, speedArrowAngle - oldSpeedArrowAngle, SPEED_ARROW_RADIUS, SPEED_ARROW_RADIUS, SPEED_ARROW_WIDTH, RED_COLOR);
  }

  oldSpeedArrowAngle = speedArrowAngle;
}
//TODO
void Display::drawSpeedometerRpm(uint16_t rpm) {
}

void Display::drawText(int16_t x, int16_t y, String text, uint color, uint8_t size) {
  tft->setCursor(x, y);
  tft->setTextColor(color, BLACK_COLOR);
  tft->setTextWrap(false);
  tft->setTextSize(size);
  tft->print(text);
}