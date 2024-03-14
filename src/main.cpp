#include <Arduino.h>
#include "EmonLib.h"                 

#define emonTxV3    1

EnergyMonitor sensors;                  

float currentRMS;
void setup()
{  
  Serial.begin(9600);
  analogReadResolution(12);
  sensors.current(A6, 7.85);
}

void loop()
{
  sensors.calcVI(20,2000);
  currentRMS= sensors.Irms;
  Serial.println(currentRMS);
}