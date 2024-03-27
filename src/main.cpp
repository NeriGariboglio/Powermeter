#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"
#include "EmonLib.h"                 
#define FIREBASE_HOST "powermeter-esp32-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "LuEhdnkBM7XuM2AaGiyUHXLNxeew51TucH8Lifeo"

const char* ssid="Fibertel WiFi764 2.4GHz";
const char* password="00444074551";

#define emonTxV3    1

FirebaseData firebaseData;
EnergyMonitor sensors;                  

String nodo="/Sensores";
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
}

void loop()
{
  sensors.calcVI(20,2000);
  currentRMS= sensors.Irms;
  //Serial.println(currentRMS);
  Firebase.setInt(firebaseData, nodo +"/corriente",currentRMS);
  
}
