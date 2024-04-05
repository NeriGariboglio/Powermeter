#ifndef powermeter_h
#define powermeter_h
#include <Arduino.h>
class powermeter
{
    public:
        powermeter(byte pinCurr, byte calibrationCurr, byte pinVolt, byte calibrationVolt);
        void getMeter(float *current, float *voltage, float *power, float *powerPerHour);
    private:
        byte _pinCurr;
        byte _calibrationCurr;
        byte _pinVolt;
        byte _calibrationVolt;
};
#endif
