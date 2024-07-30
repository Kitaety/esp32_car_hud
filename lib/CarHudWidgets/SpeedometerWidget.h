// Standard support
#include <Arduino.h>
#include <TFT_eSPI.h>

class SpeedometerWidget : public TFT_eSPI
{
public:
	SpeedometerWidget(TFT_eSPI *tft);
	void init(uint16_t x, uint16_t y, uint16_t r, uint16_t measurementScaleColor = TFT_WHITE, uint16_t indicatorColor = TFT_RED, uint16_t warningSpeedValueColor = TFT_RED, uint16_t textColor = TFT_WHITE);
	void update(int16_t speed);

private:
	void drawSpeedometerSpeedIndicators();
	void drawSpeedometerSpeedText(uint16_t speed);
	void drawSpeedometerArrow(uint16_t speed);
	void drawArc(uint16_t x, uint16_t y, uint16_t r, uint16_t w, int16_t startAngle, int16_t arcAngle, uint16_t color, bool smooth = true);
	void drawText(int16_t x, int16_t y, String text, uint color, uint8_t size = 3);

	TFT_eSPI *_tft;
	uint16_t _x,
		_y,
		_r,
		_xCenter,
		_yCenter,
		_measurementScaleColor,
		_indicatorColor,
		_warningSpeedValueColor,
		_textColor,
		_speedArrowAngle = 0,
		_oldSpeedArrowAngle = 0;
};
