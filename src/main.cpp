#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "powermeter.h"
#include "time.h"
#include "updateRTC.hpp"
#include "initWiFi.hpp"
#include "confFirebase.hpp"       

Powermeter fase(A6,7.58,A7,185,2);
FirebaseJson json;

// Timer variables (send new readings every three minutes)
const char* ntpServer = "pool.ntp.org";
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;
int timestamp;
float current,voltage,power,powerPerHour;

void setup(){  
  Serial.begin(9600);
  initWiFi();
  configTime(0, 0, ntpServer);
  initFirebase();
}

void loop(){
  fase.getMeter();
  current=fase.getCurrent();
  voltage=fase.getVoltage();
  power=fase.getPower();
  powerPerHour=fase.getPowerPerHour();
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    parentPath= databasePath + "/" + String(timestamp);
    json.set(currPath.c_str(), String(current));
    json.set(voltPath.c_str(), String(voltage));
    json.set(powPath.c_str(), String(power));
    json.set(powHsPath.c_str(),String(powerPerHour));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}