#include "SpeedometerWidget.h"

#define SPEED_MAX_VALUE 300
#define RPM_MAX_VALUE 8000

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
	init(0, 0, 90);
}

void SpeedometerWidget::init(uint16_t x, uint16_t y, uint16_t r, uint16_t measurementScaleColor, uint16_t indicatorColor, uint16_t warningSpeedValueColor, uint16_t textColor)
{
	_x = x;
	_y = y;
	_r = r;
	_textColor = textColor;

	_xSpeedCenter = _x + _r;
	_ySpeedCenter = _y + _r;

	_measurementScaleColor = measurementScaleColor;
	_indicatorColor = indicatorColor;
	_warningSpeedValueColor = warningSpeedValueColor;

	_speedTextKmPerHPositionX = _xSpeedCenter - 2 * CHAR_WIDTH_SIZE_2;
	_speedTextKmPerHPositionY = _ySpeedCenter + CHAR_HEIGHT_SIZE_2;
	_speedTextKmPositionY = _speedTextKmPerHPositionY - 5 - CHAR_HEIGHT_SIZE_3;

	_xRpm = _x;
	_yRpm = _r * 2 + 2;
	_rpmHeight = _r / 4;
	_rpmWidth = _r * 2;
}

void SpeedometerWidget::update(int16_t speed, uint16_t rpm)
{
	updateSpeed(speed);
	updateRpm(rpm);
}

void SpeedometerWidget::updateSpeed(uint16_t speed)
{
	uint16_t truncatedSpeed = speed % (SPEED_MAX_VALUE + 1);

	drawArc(_xSpeedCenter, _ySpeedCenter, _r, SPEED_ARC_WIDTH, SPEED_START_ANGLE, SPEED_ARC_ANGLE, _measurementScaleColor);
	drawSpeedIndicators();
	drawSpeedArrow(truncatedSpeed);
	drawSpeedText(truncatedSpeed);
}

void SpeedometerWidget::updateRpm(uint16_t rpm)
{
	uint16_t truncatedRpm = rpm % (RPM_MAX_VALUE + 1);

	// drawRpmIndicators();
	drawRpmArrow(truncatedRpm);
}

void SpeedometerWidget::drawSpeedIndicators()
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

		drawArc(_xSpeedCenter, _ySpeedCenter, radius, SPEED_SEGMENT_WIDTH, angle, SPEED_SEGMENT_ANGLE, colorSpeedSegment);
	}
}

void SpeedometerWidget::drawSpeedText(uint16_t speed)
{
	String speedString = String(speed);

	uint16_t speedTextKmPositionX = _xSpeedCenter - (speedString.length() * CHAR_WIDTH_SIZE_3) / 2;

	_tft->fillRect(_xSpeedCenter - SPEED_TEXT_AREA_WIDTH / 2, _speedTextKmPositionY, SPEED_TEXT_AREA_WIDTH, SPEED_TEXT_AREA_HEIGHT, TFT_BLACK);
	drawText(speedTextKmPositionX, _speedTextKmPositionY, speedString, _textColor, 3);
	drawText(_speedTextKmPerHPositionX, _speedTextKmPerHPositionY, "KM/H", _textColor, 2);
}

void SpeedometerWidget::drawSpeedArrow(uint16_t speed)
{
	_speedSpeedArrowAngle = SPEED_ARC_ANGLE * speed / SPEED_MAX_VALUE;
	_startSpeedAngle = SPEED_START_ANGLE;
	_speedAngle = 1;
	bool isUp = true;

	if (_oldSpeedArrowAngle > _speedSpeedArrowAngle)
	{
		// draw down speed
		_startSpeedAngle += _speedSpeedArrowAngle;
		_speedAngle = _oldSpeedArrowAngle - _speedSpeedArrowAngle;
		isUp = false;
	}
	else if (_speedSpeedArrowAngle > _oldSpeedArrowAngle)
	{
		// draw up speed
		_startSpeedAngle += _oldSpeedArrowAngle;
		_speedAngle = _speedSpeedArrowAngle - _oldSpeedArrowAngle;
	}

	drawArc(_xSpeedCenter, _ySpeedCenter, _r - SPEED_ARC_WIDTH - SPEED_SEGMENT_WIDTH - 2, SPEED_ARROW_WIDTH, _startSpeedAngle, _speedAngle, isUp ? _indicatorColor : TFT_BLACK);
	_oldSpeedArrowAngle = _speedSpeedArrowAngle;
}

void SpeedometerWidget::drawRpmIndicators()
{
	_tft->fillRect(_xRpm, _yRpm, _rpmWidth, 3, _measurementScaleColor);
	uint16_t step = _rpmWidth / RPM_INDICATOR_COUNT;
	for (int i = 0; i < RPM_INDICATOR_COUNT + 1; i++)
	{
		uint16_t x = _xRpm + step * i;
		_tft->fillRect(x, _yRpm + 2, 2, 5, _measurementScaleColor);
	}
}
void SpeedometerWidget::drawRpmArrow(uint16_t rpm)
{
	uint16_t radius = _r - SPEED_ARC_WIDTH - SPEED_SEGMENT_WIDTH - SPEED_ARROW_WIDTH - 4;
	uint16_t angle = SPEED_ARC_ANGLE * (rpm * 100 / RPM_MAX_VALUE) * 100;
	drawArc(_xSpeedCenter, _ySpeedCenter, radius, SPEED_ARROW_WIDTH, SPEED_START_ANGLE, angle, TFT_GREEN);
	// _tft->fillRect(_xRpm, _yRpm + 8, _rpmWidth, _rpmHeight, TFT_GREEN);
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
