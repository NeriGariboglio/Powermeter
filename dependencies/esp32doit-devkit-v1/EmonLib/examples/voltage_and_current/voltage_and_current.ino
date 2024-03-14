// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor sensors;             // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  sensors.voltage(2, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  sensors.current(1, 111.1);       // Current: input pin, calibration.
}

void loop()
{
  sensors.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  sensors.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realPower       = sensors.realPower;        //extract Real Power into variable
  float apparentPower   = sensors.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = sensors.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = sensors.Vrms;             //extract Vrms into Variable
  float Irms            = sensors.Irms;             //extract Irms into Variable
}
