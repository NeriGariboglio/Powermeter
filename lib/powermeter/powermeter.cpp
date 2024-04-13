#include <Arduino.h>
#include <EmonLib.h>
#include "powermeter.h"
#define emonTxV3    1
EnergyMonitor sensors;

Powermeter::Powermeter(byte pinCurr, byte calibrationCurr, byte pinVolt, byte calibrationVolt){
    _pinCurr=pinCurr;
    _calibrationCurr=calibrationCurr;
    _pinVolt=pinVolt;
    _calibrationVolt=calibrationVolt;
    sensors.current(_pinCurr,_calibrationCurr);
    sensors.voltage(_pinVolt,_calibrationVolt,1.7);
    analogReadResolution(12);
}

void Powermeter::getMeter(){
    sensors.calcVI(20, 2000);
    _irms = sensors.Irms;
    _vrms = sensors.Vrms;
    _power = sensors.realPower*0.001;
    _powerFactor= sensors.powerFactor;
    
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

float  Powermeter::getPowerFactor(){
    return _powerFactor;
}