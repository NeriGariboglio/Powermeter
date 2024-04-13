#ifndef powermeter_h
#define powermeter_h
#include <Arduino.h>
class Powermeter
{
    public:
        Powermeter(byte pinCurr , byte calibrationCurr, byte pinVolt, byte calibrationVolt);
        void getMeter();
        float getCurrent();
        float getVoltage();
        float getPower();
        float getPowerFactor();
    private:
        byte _pinCurr;
        byte _calibrationCurr;
        byte _pinVolt;
        byte _calibrationVolt;
        float _powerFactor;
        float _irms;
        float _vrms;
        float _power;
};
#endif