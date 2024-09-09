#include "OBDIIManager.h"

OBDIIManager::OBDIIManager(ELM327& elm327, BluetoothSerial& bt) {
    _elm327 = &elm327;
    _bt = &bt;
    _obdState = ENG_RPM;
}

bool OBDIIManager::connect(const char* mac) {
    _bt->begin("ArduHUD", true);
    _bt->setPin("1234");

    BTAddress address = BTAddress(mac);

    if (!_bt->connect(address)) {
        Serial.println("Error BT Phase 1");
        return false;
    }
    if (!_elm327->begin(*_bt, true, 3000)) {
        Serial.println("Error BT Phase 2");
        return false;
    }
   
    Serial.println("Connected to ELM327");
    _macAddress = mac;

    return true;
}

void OBDIIManager::update() {
    switch (_obdState) {
        case ENG_RPM: {
            int32_t _rpm = _elm327->rpm();

            if (_elm327->nb_rx_state == ELM_SUCCESS) {
                rpm = _rpm;
                _obdState = SPEED;
            } else if (_elm327->nb_rx_state != ELM_GETTING_MSG) {
                rpm = 0;
                _obdState = SPEED;
            }

            break;
        }
        case SPEED: {
            int32_t _speed = _elm327->kph();
            if (_elm327->nb_rx_state == ELM_SUCCESS) {
                speed = _speed;
                _obdState = BATTERY_VOLTAGE;
            } else if (_elm327->nb_rx_state != ELM_GETTING_MSG) {
                speed = 0;
                _obdState = BATTERY_VOLTAGE;
            }

            break;
        }
        case BATTERY_VOLTAGE: {
            float _batteryVoltage = _elm327->batteryVoltage();

            if (_elm327->nb_rx_state == ELM_SUCCESS) {
                batteryVoltage = _batteryVoltage;
                _obdState = COOLANT_TEMPERATURE;
            } else if (_elm327->nb_rx_state != ELM_GETTING_MSG) {
                batteryVoltage = 0;
                _obdState = COOLANT_TEMPERATURE;
            }

            break;
        }
        case COOLANT_TEMPERATURE: {
            float _temp = _elm327->engineCoolantTemp();

            if (_elm327->nb_rx_state == ELM_SUCCESS) {
                engineCoolantTemp = _temp;
                _obdState = ENG_RPM;
            } else if (_elm327->nb_rx_state != ELM_GETTING_MSG) {
                engineCoolantTemp = 0;
                _obdState = ENG_RPM;
            }

            break;
        }
    }
}