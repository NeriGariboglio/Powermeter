#include <Arduino.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Time.h>
#include "powermeter.h"
#include "time.h"
#include "updateRTC.hpp"
#include "initWiFi.hpp"       
#include "firebaseP.h"

Powermeter fase(A6,7.50,A7,180,1);
FirebaseP sendFase("/fase");
ESP32Time rtc(-3600*3);

const char* ntpServer = "pool.ntp.org";
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;
float current,voltage,power;
float energyConsumedHour,energyConsumedDay = 0.0;
int timestamp;
unsigned long lastLoopMillis = 0;
void setup(){  
  Serial.begin(9600);
  initWiFi();
  configTime(0, 0, ntpServer);
  sendFase.configuration();
}

void loop(){
  //fase.getMeter();
  //current=fase.getCurrent();
  //voltage=fase.getVoltage();
  //power=fase.getPower();

  unsigned long currentMillis = millis();
    unsigned long elapsedTime = currentMillis - lastLoopMillis;

    if (elapsedTime >= 1000) // Realizar cálculos cada segundo
    {
        fase.getMeter();
        current = fase.getCurrent();
        voltage = fase.getVoltage();
        power = fase.getPower();

        energyConsumedHour += (power * elapsedTime) / 3600000.0; // Multiplicar la potencia por el tiempo y convertir a kWh
        energyConsumedDay += (power * elapsedTime) / 3600000.0; // Actualizar el consumo total del día

        lastLoopMillis = currentMillis;
    }

    // Obtener la hora actual
    timestamp = getTime();
    int currentHour = rtc.getHour(true);

    // Enviar datos a Firebase
    if (currentHour != rtc.getHour(true)) // Si ha pasado una hora
    {
        sendFase.sendNode("/energy_consumed_hour", energyConsumedHour, timestamp);
        energyConsumedHour = 0.0; // Reiniciar el contador de energía para la próxima hora
    }

    // Verificar si ha cambiado el día
    if (currentHour == 0) // Si es medianoche
    {
        // Enviar el consumo total del día a Firebase
        sendFase.sendNode("/energy_consumed_day", energyConsumedDay, timestamp);
        energyConsumedDay = 0.0; // Reiniciar el contador de energía para el próximo día
    }
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    sendFase.sendNode("/current",current,timestamp);
    sendFase.sendNode("/voltage",voltage,timestamp);
    sendFase.sendNode("/power",power,timestamp);
  }
}