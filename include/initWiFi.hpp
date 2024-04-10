#include <Arduino.h>
#include <WiFiManager.h>

void initWiFi(){
  WiFi.mode(WIFI_STA);
  WiFiManager WiFiManager;
  //WiFiManager.resetSettings();
  bool res;
  res=WiFiManager.autoConnect("PowermeterESP32","12345678");
  if (!res){
    Serial.println("Failed to connect");
  }
  else{
    Serial.println("connected");
  }
}