#include <Arduino.h>

#include "BluetoothSerial.h"
#include "ELMduino.h"

typedef enum {
    ENG_RPM,
    SPEED,
    BATTERY_VOLTAGE,
    COOLANT_TEMPERATURE,
    FUEL_RATE
} ObdPidStates;

class OBDIIManager {
   public:
    OBDIIManager(ELM327& elm327, BluetoothSerial& bt);
    bool connect(const char* mac);
    void update();
    int32_t speed = 0,
            rpm = 0;
    float batteryVoltage = 0,
          engineCoolantTemp = 0,
          fuelRatePer100km = 0,
          fuelRate = 0;
    bool connected;

   private:
    const char* _macAddress;
    ELM327* _elm327;
    BluetoothSerial* _bt;
    ObdPidStates _obdState = ENG_RPM;
};
