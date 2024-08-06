#include "SpeedometerWidget.h"

#define SPEED_MAX_VALUE 300
#define RPM_MAX_VALUE 8000
#define RPM_MAX_GREEN_ZONE_VALUE 2500
#define RPM_MAX_YELLOW_ZONE_VALUE 5000
#define RPM_MAX_VALUE 8000

#define ARC_ANGLE 270
#define ARC_START_ANGLE 45
#define SPEED_ARC_WIDTH 2

#define SPEED_SEGMENT_COUNT 16
#define SPEED_SEGMENT_ANGLE_SPLIT (270 / (SPEED_SEGMENT_COUNT - 1))
#define SPEED_SEGMENT_WIDTH 8
#define SPEED_SEGMENT_ANGLE 2

#define CHAR_WIDTH_SIZE_2 11
#define CHAR_HEIGHT_SIZE_2 16
#define CHAR_WIDTH_SIZE_3 17
#define CHAR_HEIGHT_SIZE_3 21

#define SPEED_TEXT_AREA_WIDTH CHAR_WIDTH_SIZE_3 * 3
#define SPEED_TEXT_AREA_HEIGHT CHAR_HEIGHT_SIZE_3

#define RPM_TEXT_AREA_WIDTH CHAR_WIDTH_SIZE_3 * 4
#define RPM_TEXT_AREA_HEIGHT CHAR_HEIGHT_SIZE_2

#define ARROW_WIDTH 15

int warningSpeedSegment[3] = {2, 3, 6};

uint16_t getRpmColor(uint16_t rpm) {
    if (rpm <= RPM_MAX_GREEN_ZONE_VALUE) {
        return TFT_GREEN;
    }
    if (rpm > RPM_MAX_GREEN_ZONE_VALUE && rpm <= RPM_MAX_YELLOW_ZONE_VALUE) {
        return TFT_YELLOW;
    }
    return TFT_RED;
}

SpeedometerWidget::SpeedometerWidget(TFT_eSPI *tft) {
    _tft = tft;
    init(0, 0, 90);
}

void SpeedometerWidget::init(uint16_t x, uint16_t y, uint16_t r, uint16_t measurementScaleColor, uint16_t indicatorColor, uint16_t warningSpeedValueColor, uint16_t textColor) {
    _x = x;
    _y = y;
    _r = r;
    _textColor = textColor;

    _xCenter = _x + _r;
    _yCenter = _y + _r;

    _speedIndicatorRadius = _r - SPEED_ARC_WIDTH;

    _measurementScaleColor = measurementScaleColor;
    _indicatorColor = indicatorColor;
    _warningSpeedValueColor = warningSpeedValueColor;

    _speedArrowRadius = _r - SPEED_ARC_WIDTH - SPEED_SEGMENT_WIDTH - 2;
    _speedTextKmPerHPositionX = _xCenter - 2 * CHAR_WIDTH_SIZE_2;
    _speedTextKmPerHPositionY = _yCenter + CHAR_HEIGHT_SIZE_2;
    _speedTextKmPositionY = _speedTextKmPerHPositionY - 5 - CHAR_HEIGHT_SIZE_3;

    _rpmArrowRadius = _r - SPEED_ARC_WIDTH - SPEED_SEGMENT_WIDTH - ARROW_WIDTH - 4;
    _xRpm = _x;
    _yRpm = _r * 2 + 2;
}

void SpeedometerWidget::update(int32_t speed, int32_t rpm) {
    updateSpeed(speed);
    updateRpm(rpm);
}

void SpeedometerWidget::updateSpeed(int32_t speed) {
    uint16_t truncatedSpeed = speed % (SPEED_MAX_VALUE + 1);

    drawArc(_xCenter, _yCenter, _r, SPEED_ARC_WIDTH, ARC_START_ANGLE, ARC_ANGLE, _measurementScaleColor);
    drawSpeedIndicators();
    drawSpeedArrow(truncatedSpeed);
    drawSpeedText(truncatedSpeed);
}

void SpeedometerWidget::updateRpm(int32_t rpm) {
    uint16_t truncatedRpm = rpm % (RPM_MAX_VALUE + 1);
    drawRpmArrow(truncatedRpm);
    drawRpmText(truncatedRpm);
}

void SpeedometerWidget::drawSpeedIndicators() {
    int currentWarningSpeedSegment = 0;
    uint16_t colorSpeedSegment = 0;

    for (int i = 0; i < SPEED_SEGMENT_COUNT; i++) {
        if (i == warningSpeedSegment[currentWarningSpeedSegment]) {
            colorSpeedSegment = _warningSpeedValueColor;
            currentWarningSpeedSegment++;
        } else {
            colorSpeedSegment = _measurementScaleColor;
        }

        int angle = (ARC_START_ANGLE + (i * SPEED_SEGMENT_ANGLE_SPLIT)) % 360;

        if (i == SPEED_SEGMENT_COUNT - 1) {
            angle -= SPEED_SEGMENT_ANGLE;
        } else if (i > 0) {
            angle -= SPEED_SEGMENT_ANGLE / 2;
        }

        drawArc(_xCenter, _yCenter, _speedIndicatorRadius, SPEED_SEGMENT_WIDTH, angle, SPEED_SEGMENT_ANGLE, colorSpeedSegment);
    }
}

void SpeedometerWidget::drawSpeedText(int32_t speed) {
    String speedString = String(speed);

    uint16_t speedTextKmPositionX = _xCenter - (speedString.length() * CHAR_WIDTH_SIZE_3) / 2;

    _tft->fillRect(_xCenter - SPEED_TEXT_AREA_WIDTH / 2, _speedTextKmPositionY, SPEED_TEXT_AREA_WIDTH, SPEED_TEXT_AREA_HEIGHT, TFT_BLACK);
    drawText(speedTextKmPositionX, _speedTextKmPositionY, speedString, _textColor, 3);
    drawText(_speedTextKmPerHPositionX, _speedTextKmPerHPositionY, "KM/H", _textColor, 2);
}

void SpeedometerWidget::drawSpeedArrow(int32_t speed) {
    _speedArrowAngle = ARC_ANGLE * speed / SPEED_MAX_VALUE;
    _startSpeedAngle = ARC_START_ANGLE;
    _speedAngle = 1;
    bool isUp = true;

    if (_oldSpeedArrowAngle > _speedArrowAngle) {
        // draw down
        _startSpeedAngle += _speedArrowAngle;
        _speedAngle = _oldSpeedArrowAngle - _speedArrowAngle;
        isUp = false;
    } else if (_speedArrowAngle > _oldSpeedArrowAngle) {
        // draw up
        _startSpeedAngle += _oldSpeedArrowAngle;
        _speedAngle = _speedArrowAngle - _oldSpeedArrowAngle;
    }

    drawArc(_xCenter, _yCenter, _speedArrowRadius, ARROW_WIDTH, _startSpeedAngle, _speedAngle, isUp ? _indicatorColor : TFT_BLACK);
    _oldSpeedArrowAngle = _speedArrowAngle;
}

void SpeedometerWidget::drawRpmArrow(int32_t rpm) {
    _rpmArrowAngle = ARC_ANGLE * rpm / RPM_MAX_VALUE;
    _startRpmArrowAngle = ARC_START_ANGLE;
    _rpmAngle = 1;
    bool isUp = true;

    if (_oldRpmArrowAngle > _rpmArrowAngle) {
        // draw down
        _startRpmArrowAngle += _rpmArrowAngle;
        _rpmAngle = _oldRpmArrowAngle - _rpmArrowAngle;
        isUp = false;
    } else if (_rpmArrowAngle > _oldRpmArrowAngle) {
        // draw up
        _startRpmArrowAngle += _oldRpmArrowAngle;
        _rpmAngle = _rpmArrowAngle - _oldRpmArrowAngle;
    }

    drawArc(_xCenter, _yCenter, _rpmArrowRadius, ARROW_WIDTH, _startRpmArrowAngle, _rpmAngle, isUp ? getRpmColor(rpm) : TFT_BLACK);
    _oldRpmArrowAngle = _rpmArrowAngle;
}

void SpeedometerWidget::drawRpmText(int32_t rpm) {
    String rpmString = String(rpm);
    uint16_t _rpmdTextPositionY = _speedTextKmPerHPositionY + CHAR_HEIGHT_SIZE_3 + 10;
    uint16_t _rpmTextPositionX = _xCenter - (rpmString.length() * CHAR_WIDTH_SIZE_2) / 2;

    _tft->fillRect(_xCenter - RPM_TEXT_AREA_WIDTH / 2, _rpmdTextPositionY, RPM_TEXT_AREA_WIDTH, RPM_TEXT_AREA_HEIGHT, TFT_BLACK);
    drawText(_rpmTextPositionX, _rpmdTextPositionY, rpmString, _textColor, 2);
}

void SpeedometerWidget::drawArc(uint16_t x, uint16_t y, uint16_t r, uint16_t w, int16_t startAngle, int16_t arcAngle, uint16_t color, bool smooth) {
    int16_t endAngle = (startAngle + arcAngle) % 360;
    _tft->drawArc(x, y, r, r - w, startAngle, endAngle, color, TFT_BLACK, smooth);
}

void SpeedometerWidget::drawText(int16_t x, int16_t y, String text, uint color, uint8_t size) {
    _tft->setCursor(x, y);
    _tft->setTextColor(color, TFT_BLACK, true);
    _tft->setTextWrap(false);
    _tft->setTextSize(size);
    _tft->print(text);
}
