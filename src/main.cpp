#include <Arduino.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include "powermeter.h"
#include "time.h"
#include "updateRTC.hpp"
#include "initWiFi.hpp"       
#include "firebaseP.h"

Powermeter fase(A6,7.58,A7,185,2);
FirebaseP sendFase("/fase");

const char* ntpServer = "pool.ntp.org";
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;
float current,voltage,power,powerPerHour;
int timestamp;

void setup(){  
  Serial.begin(9600);
  initWiFi();
  configTime(0, 0, ntpServer);
  sendFase.configuration();
}

void loop(){
  fase.getMeter();
  current=fase.getCurrent();
  voltage=fase.getVoltage();
  power=fase.getPower();
  powerPerHour=fase.getPowerPerHour();
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    sendFase.sendNode("/current",current,timestamp);
    sendFase.sendNode("/voltage",voltage,timestamp);
    sendFase.sendNode("/power",power,timestamp);
    sendFase.sendNode("/powerPerHour",powerPerHour,timestamp);
  }
}