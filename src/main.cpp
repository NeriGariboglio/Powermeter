/*
  Rui Santos
  Complete project details at our blog: https://RandomNerdTutorials.com/esp32-data-logging-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "time.h"
#include "EmonLib.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Fibertel WiFi764 2.4GHz"
#define WIFI_PASSWORD "00444074551"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAqv47PT2b2AWK77PnpO5C3CCmQ7gSKmTk"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "nerigaribo@gmail.com"
#define USER_PASSWORD "powermeter"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://esp32-powermeter-default-rtdb.firebaseio.com/"
#define emonTxV3    1
// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String currentPath = "/current";
String timePath = "/timestamp";

EnergyMonitor sensors; 

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

float currentRMS=0;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;



// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void setup(){
  Serial.begin(9600);
  initWiFi();
  configTime(0, 0, ntpServer);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
  analogReadResolution(12);
  sensors.current(A6, 7.85);
}

void loop(){
  //sensors.calcVI(20,2000);
  //currentRMS= sensors.Irms;
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath= databasePath + "/" + String(timestamp);

    //json.set(tempPath.c_str(), String(bme.readTemperature()));
    //json.set(humPath.c_str(), String(bme.readHumidity()));
    json.set(currentPath.c_str(), String(100.0F));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}

/*#include <Arduino.h>
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
}*/