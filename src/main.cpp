#include <Arduino.h>
#include "EmonLib.h"                 

#define emonTxV3    1

EnergyMonitor sensors;                  

float voltageRMS;
float currentRMS;
void setup()
{  
  Serial.begin(9600);
  analogReadResolution(12);
  sensors.current(A6, 7.85);
  sensors.voltage(A7,185,1.7);//con alimentacion externa de 3.25v
}

void loop()
{
  sensors.calcVI(20,2000);
  //voltageRMS= sensors.Vrms;
  //currentRMS= sensors.Irms;
  //Serial.println(voltageRMS);
  //Serial.println(currentRMS);
    sensors.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realPower       = sensors.realPower;        //extract Real Power into variable
  float apparentPower   = sensors.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = sensors.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = sensors.Vrms;             //extract Vrms into Variable
  float Irms            = sensors.Irms;             //extract Irms into Variable
}