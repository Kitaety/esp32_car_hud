#include <Arduino.h>
#include <TFT_eSPI.h>

typedef enum {
    BATTERY_ICON,
    FUEL_ICON,
    COOLANT_TEMPERATURE_ICON
} IndicatorIcon;

struct IndicatorIconData {
    uint16_t w, h;
    const uint16_t *icon;
};

class IndicatorWithIconWidget {
   public:
    IndicatorWithIconWidget(TFT_eSPI *tft, IndicatorIcon icon);
    void init(uint16_t x, uint16_t y);
    void update(float value);
    void update(uint32_t value);

   private:
    void drawIcon();
    bool _firstUpdate = true;
    float _fValue = 0;
    uint32_t _i32Value = 0;
    TFT_eSPI *_tft;
    uint16_t _x,
        _y;
    IndicatorIconData _iconData;
};
