#include <Arduino.h>

#include "BluetoothSerial.h"
#include "ELMduino.h"

typedef enum {
    ENG_RPM,
    SPEED,
    BATTERY_VOLTAGE
} ObdPidStates;

class OBDIIManager {
   public:
    OBDIIManager(ELM327& elm327, BluetoothSerial& bt);
    bool connect(const char* mac);
    void update();
    int32_t speed = 0,
            rpm = 0;
    double batteryVoltage = 0;
    bool connected;

   private:
    const char* _macAddress;
    ELM327* _elm327;
    BluetoothSerial* _bt;
    ObdPidStates _obdState = ENG_RPM;
};
