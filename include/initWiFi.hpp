#include <Arduino.h>
#include <WiFi.h>
#define WIFI_SSID "Uader" // Insert your network credentials Fibertel WiFi764 2.4GHz
#define WIFI_PASSWORD "12345678"//00444074551

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