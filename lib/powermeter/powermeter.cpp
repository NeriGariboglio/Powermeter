#include <Arduino.h>
#include <EmonLib.h>
#include "powermeter.h"
#define emonTxV3    1
EnergyMonitor sensors;

powermeter::powermeter(byte pinCurr, byte calibrationCurr, byte pinVolt, byte calibrationVolt){
    _pinCurr=pinCurr;
    _calibrationCurr=calibrationCurr;
    _pinVolt=pinVolt;
    _calibrationVolt=calibrationVolt;
    sensors.current(_pinCurr,_calibrationCurr);//A6 , 7.85
    sensors.voltage(_pinVolt,_calibrationVolt,1.7);//A7 , 185 , con alimentacion externa de 3.25v
    analogReadResolution(12);
}

void powermeter::getMeter(float *current, float *voltage, float *power, float *powerPerHour){
    sensors.calcVI(20, 2000);
    float _powerPerHour;
    float _irms = sensors.Irms;
    float _vrms = sensors.Vrms;
    float _power = sensors.realPower * 0.001;
    /*if (_irms <= 0.2) {
        _irms = 0;
        _power = 0;
    }
    if (_vrms <= 60) {
        _irms = 0;
        _vrms = 0;
        _power = 0;
    }*/
    float _powerTempKwh = (_power / 3600);
    _powerPerHour += _powerTempKwh;
    *current = _irms;
    *voltage = _vrms;
    *power = _power;
    *powerPerHour = _powerPerHour;
}
