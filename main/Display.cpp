#include "Display.h"
#include <SPI.h>

#define DEG2RAD 0.0174532925

#define SPEED_MAX_VALUE 300

#define SPEED_CENTER_X 110
#define SPEED_CENTER_Y 110
#define SPEED_ARC_ANGLE 270
#define SPEED_START_ANGLE 45
#define SPEED_END_ANGLE (SPEED_START_ANGLE + SPEED_ARC_ANGLE) % 360
#define SPEED_ARC_RADIUS 90
#define SPEED_ARC_WIDTH 2

#define SPEED_SEGMENT_COUNT 16
#define SPEED_SEGMENT_ANGLE_SPLIT (270 / (SPEED_SEGMENT_COUNT - 1))
#define SPEED_SEGMENT_WIDTH 7
#define SPEED_SEGMENT_ANGLE 2
#define SPEED_SEGMENT_RADIUS (SPEED_ARC_RADIUS - SPEED_ARC_WIDTH)

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

int warningSpeedSegment[3] = { 2, 3, 6 };
uint16_t oldSpeedArrowAngle = 0;

My_TFT_eSPI tft = My_TFT_eSPI();

void Display::init(int8_t rotation) {
  tft.init();
  tft.setRotation(rotation);
  drawSpeedometer(0, 0);
}

void Display::clear() {
  tft.fillScreen(BLACK);
}

void Display::drawSpeedometer(uint16_t speed, uint16_t rpm) {
  uint16_t truncateSpeed = speed % (SPEED_MAX_VALUE + 1);

  tft.drawArc(SPEED_CENTER_X, SPEED_CENTER_Y, SPEED_ARC_RADIUS, SPEED_ARC_WIDTH, SPEED_START_ANGLE, SPEED_ARC_ANGLE, WHITE);
  drawSpeedometerSpeedIndicators();
  drawSpeedometerArrow(truncateSpeed);
  drawSpeedometerSpeedText(truncateSpeed, WHITE);
  drawSpeedometerRpm(rpm);
}

void Display::drawSpeedometerSpeedIndicators() {
  int currentWarningSpeedSegment = 0;

  for (int i = 0; i < SPEED_SEGMENT_COUNT; i++) {
    int colorSpeedSegment = WHITE;
    if (i == warningSpeedSegment[currentWarningSpeedSegment]) {
      colorSpeedSegment = RED;
      currentWarningSpeedSegment++;
    }

    int angle = (SPEED_START_ANGLE + (i * SPEED_SEGMENT_ANGLE_SPLIT)) % 360;

    if (i > 0) {
      angle -= SPEED_SEGMENT_ANGLE / 2;
    }
    tft.drawArc(SPEED_CENTER_X, SPEED_CENTER_Y, SPEED_SEGMENT_RADIUS, SPEED_SEGMENT_WIDTH, angle, SPEED_SEGMENT_ANGLE, colorSpeedSegment);
  }
}

void Display::drawSpeedometerSpeedText(uint16_t speed, uint color) {
  tft.fillRect(SPEED_TEXT_AREA_X, SPEED_TEXT_AREA_Y, SPEED_TEXT_AREA_WIDTH, SPEED_TEXT_AREA_HEIGHT, BLACK);
  String speedString = String(speed);
  drawText(SPEED_TEXT_POSITION_X(speedString.length()), SPEED_TEXT_POSITION_Y, speedString, color, 3);
  drawText(SPEED_TEXT_KM_H_POSITION_X, SPEED_TEXT_KM_H_POSITION_Y, "KM/H", color, 2);
}


void Display::drawSpeedometerArrow(uint16_t speed) {
  uint16_t speedArrowAngle = SPEED_ARC_ANGLE * speed / SPEED_MAX_VALUE;

  //draw down speed
  if (oldSpeedArrowAngle > speedArrowAngle) {
    uint16_t angle = SPEED_START_ANGLE + speedArrowAngle;
    tft.drawArc(SPEED_CENTER_X, SPEED_CENTER_Y, SPEED_ARROW_RADIUS, SPEED_ARROW_WIDTH, angle, oldSpeedArrowAngle - speedArrowAngle, BLACK);
  }

  //draw up speed
  if (speedArrowAngle > oldSpeedArrowAngle) {
    uint16_t angle = SPEED_START_ANGLE + oldSpeedArrowAngle;
    tft.drawArc(SPEED_CENTER_X, SPEED_CENTER_Y, SPEED_ARROW_RADIUS, SPEED_ARROW_WIDTH, angle, speedArrowAngle - oldSpeedArrowAngle, RED);
  }
  if (speedArrowAngle == 0) {
    tft.drawArc(SPEED_CENTER_X, SPEED_CENTER_Y, SPEED_ARROW_RADIUS, SPEED_ARROW_WIDTH, SPEED_START_ANGLE, 1, TFT_RED);
  }

  oldSpeedArrowAngle = speedArrowAngle;
}

//TODO
void Display::drawSpeedometerRpm(uint16_t rpm) {
}

void Display::drawText(int16_t x, int16_t y, String text, uint color, uint8_t size) {
  tft.setCursor(x, y);
  tft.setTextColor(color, BLACK);
  tft.setTextWrap(false);
  tft.setTextSize(size);
  tft.print(text);
}
