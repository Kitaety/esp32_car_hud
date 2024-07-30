#include "SpeedometerWidget.h"

#define SPEED_MAX_VALUE 300

#define SPEED_ARC_ANGLE 270
#define SPEED_START_ANGLE 45
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

#define SPEED_ARROW_WIDTH 15

int warningSpeedSegment[3] = {2, 3, 6};

SpeedometerWidget::SpeedometerWidget(TFT_eSPI *tft)
{
	_tft = tft;
	_x = 0;
	_y = 0;
	_r = 90;
	_measurementScaleColor = TFT_WHITE;
	_indicatorColor = TFT_RED;
	_warningSpeedValueColor = TFT_RED;
	_textColor = TFT_WHITE;
}

void SpeedometerWidget::init(uint16_t x, uint16_t y, uint16_t r, uint16_t measurementScaleColor, uint16_t indicatorColor, uint16_t warningSpeedValueColor, uint16_t textColor)
{
	_x = x;
	_y = y;
	_r = r;
	_xCenter = _x + _r;
	_yCenter = _y + _r;
	_measurementScaleColor = measurementScaleColor;
	_indicatorColor = indicatorColor;
	_warningSpeedValueColor = warningSpeedValueColor;
	_textColor = textColor;
}

void SpeedometerWidget::update(int16_t speed)
{
	uint16_t truncateSpeed = speed % (SPEED_MAX_VALUE + 1);

	drawArc(_xCenter, _yCenter, _r, SPEED_ARC_WIDTH, SPEED_START_ANGLE, SPEED_ARC_ANGLE, _measurementScaleColor);
	drawSpeedometerSpeedIndicators();
	drawSpeedometerArrow(truncateSpeed);
	drawSpeedometerSpeedText(truncateSpeed);
}

void SpeedometerWidget::drawSpeedometerSpeedIndicators()
{
	int currentWarningSpeedSegment = 0;
	uint16_t colorSpeedSegment = 0;
	int8_t radius = _r - SPEED_ARC_WIDTH;

	for (int i = 0; i < SPEED_SEGMENT_COUNT; i++)
	{
		if (i == warningSpeedSegment[currentWarningSpeedSegment])
		{
			colorSpeedSegment = _warningSpeedValueColor;
			currentWarningSpeedSegment++;
		}
		else
		{
			colorSpeedSegment = _measurementScaleColor;
		}

		int angle = (SPEED_START_ANGLE + (i * SPEED_SEGMENT_ANGLE_SPLIT)) % 360;

		if (i == SPEED_SEGMENT_COUNT - 1)
		{
			angle -= SPEED_SEGMENT_ANGLE;
		}
		else if (i > 0)
		{
			angle -= SPEED_SEGMENT_ANGLE / 2;
		}

		drawArc(_xCenter, _yCenter, radius, SPEED_SEGMENT_WIDTH, angle, SPEED_SEGMENT_ANGLE, colorSpeedSegment);
	}
}

void SpeedometerWidget::drawSpeedometerSpeedText(uint16_t speed)
{
	String speedString = String(speed);

	uint16_t speedTextKmPerHPositionX = _xCenter - 2 * CHAR_WIDTH_SIZE_2;
	uint16_t speedTextKmPerHPositionY = _yCenter;
	uint16_t speedTextKmPositionY = speedTextKmPerHPositionY - 5 - CHAR_HEIGHT_SIZE_3;
	uint16_t speedTextKmPositionX = _xCenter - (speedString.length() * CHAR_WIDTH_SIZE_3) / 2;

	_tft->fillRect(_xCenter - SPEED_TEXT_AREA_WIDTH / 2, speedTextKmPositionY, SPEED_TEXT_AREA_WIDTH, SPEED_TEXT_AREA_HEIGHT, TFT_BLACK);
	drawText(speedTextKmPositionX, speedTextKmPositionY, speedString, _textColor, 3);
	drawText(speedTextKmPerHPositionX, speedTextKmPerHPositionY, "KM/H", _textColor, 2);
}

void SpeedometerWidget::drawSpeedometerArrow(uint16_t speed)
{
	_speedArrowAngle = SPEED_ARC_ANGLE * speed / SPEED_MAX_VALUE;
	uint16_t startAngle = SPEED_START_ANGLE;
	uint16_t angle = 1;
	bool isUp = true;

	if (_oldSpeedArrowAngle > _speedArrowAngle)
	{
		// draw down speed
		startAngle += _speedArrowAngle;
		angle = _oldSpeedArrowAngle - _speedArrowAngle;
		isUp = false;
	}
	else if (_speedArrowAngle > _oldSpeedArrowAngle)
	{
		// draw up speed
		startAngle += _oldSpeedArrowAngle;
		angle = _speedArrowAngle - _oldSpeedArrowAngle;
	}

	drawArc(_xCenter, _yCenter, _r - SPEED_ARC_WIDTH - SPEED_SEGMENT_WIDTH - 2, SPEED_ARROW_WIDTH, startAngle, angle, isUp ? _indicatorColor : TFT_BLACK);
	_oldSpeedArrowAngle = _speedArrowAngle;
}

void SpeedometerWidget::drawArc(uint16_t x, uint16_t y, uint16_t r, uint16_t w, int16_t startAngle, int16_t arcAngle, uint16_t color, bool smooth)
{
	int16_t endAngle = (startAngle + arcAngle) % 360;
	_tft->drawArc(x, y, r, r - w, startAngle, endAngle, color, TFT_BLACK, smooth);
}

void SpeedometerWidget::drawText(int16_t x, int16_t y, String text, uint color, uint8_t size)
{
	_tft->setCursor(x, y);
	_tft->setTextColor(color, TFT_BLACK, true);
	_tft->setTextWrap(false);
	_tft->setTextSize(size);
	_tft->print(text);
}
