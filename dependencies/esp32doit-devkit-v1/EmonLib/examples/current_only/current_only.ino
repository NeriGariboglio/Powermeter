// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor sensors;                   // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  sensors.current(1, 111.1);             // Current: input pin, calibration.
}

void loop()
{
  double Irms = sensors.calcIrms(1480);  // Calculate Irms only
  
  Serial.print(Irms*230.0);	       // Apparent power
  Serial.print(" ");
  Serial.println(Irms);		       // Irms
}
