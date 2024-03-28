#include <Arduino.h>
#include <ESP32Time.h>
#include <WiFi.h>
#include "FirebaseESP32.h"
#include "EmonLib.h"                 

#define FIREBASE_HOST "powermeter-esp32-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "LuEhdnkBM7XuM2AaGiyUHXLNxeew51TucH8Lifeo"
#define emonTxV3    1

ESP32Time rtc;
FirebaseData firebaseData;
EnergyMonitor sensors;                  
String nodo="/Sensores";

//configuracion servidor NPT
const char* ntpServer="ntp.lonelybinary.com";
const long gmtOffset_sec=-3*3600;// GMT -3 horas
const int daylightOffset_sec=0;// Ajuste por horario de verano de 1 hora (3600 segundos)


const char* ssid="Fibertel WiFi764 2.4GHz";
const char* password="00444074551";

bool iterar=true;
float currentRMS=0;
void setup()
{  
  Serial.begin(9600);

  Serial.print("intentando conectar a ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");Serial.println("WiFi conectado");
  Serial.print("IP asignada ");
  Serial.print(WiFi.localIP());

  

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  analogReadResolution(12);
  sensors.current(A6, 7.85);
  delay(1000);
  //ya conectado a internet se actualiza el rtc interno
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
  rtc.setTimeStruct(timeinfo);  
  }
}

void loop()
{
  //sensors.calcVI(20,2000);
  currentRMS= sensors.Irms;
  Serial.println(currentRMS);
  Firebase.setInt(firebaseData, nodo +"/corriente",currentRMS);
  Serial.println(rtc.getTime("%d/%m/%Y %H:%M:%S"));
}