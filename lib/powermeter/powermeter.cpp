#include <Arduino.h>
#include <EmonLib.h>
#include "powermeter.h"
#define emonTxV3    1
EnergyMonitor sensors;

Powermeter::Powermeter(byte pinCurr, byte calibrationCurr /*algo*/, byte pinVolt, byte calibrationVolt,byte unidad){
    _pinCurr=pinCurr;
    _calibrationCurr=calibrationCurr;
    _pinVolt=pinVolt;
    _calibrationVolt=calibrationVolt;
    switch (unidad)
    {
    case 1:
        _unidad= 1;
    break;
    case 2:
        _unidad=0.001;
    break;
    default:
        _unidad=0.001;
        break;
    }
    sensors.current(_pinCurr,_calibrationCurr);//A6 , 7.85
    sensors.voltage(_pinVolt,_calibrationVolt,1.7);//A7 , 185 con alimentacion externa de 3.25v
    analogReadResolution(12);
}

void Powermeter::getMeter(){
    sensors.calcVI(20, 2000);
    _irms = sensors.Irms;
    _vrms = sensors.Vrms;
    _power = sensors.realPower *_unidad;
    if (_irms <= 0.2) {
        _irms = 0;
        _power = 0;
    }
    if (_vrms <= 40) {
        _irms = 0;
        _vrms = 0;
        _power = 0;
    }
    _powerTempKwh = (_power / 3600);
    _powerPerHour += _powerTempKwh;
}

float  Powermeter::getCurrent(){
    return _irms;
}

float  Powermeter::getVoltage(){
    return _vrms;
}

float  Powermeter::getPower(){
    return _power;
}

float  Powermeter::getPowerPerHour(){
    return _powerPerHour;
}