#ifndef powermeter_h
#define powermeter_h
#include <Arduino.h>
class Powermeter
{
    public:
        Powermeter(byte pinCurr , byte calibrationCurr, byte pinVolt, byte calibrationVolt, byte unidad);
        void getMeter();
        float getCurrent();
        float getVoltage();
        float getPower();
        float getPowerPerHour();
    private:
        byte _unidad;
        byte _pinCurr;
        byte _calibrationCurr;
        byte _pinVolt;
        byte _calibrationVolt;
        float _powerPerHour;
        float _irms;
        float _vrms;
        float _power;
        float _powerTempKwh;
};
#endif