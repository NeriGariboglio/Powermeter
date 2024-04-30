#include <Arduino.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Time.h>
#include "powermeter.h"
#include "time.h"
#include "updateRTC.hpp"
#include "initWiFi.hpp"       
#include "firebaseP.h"

Powermeter fase(A6,7.50,A7,180);
FirebaseP sendFase("/fase");
ESP32Time rtc(-3600*3);

const char* ntpServer = "pool.ntp.org";
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;
unsigned long lastLoopMillis = 0;
unsigned long currentMillis;
unsigned long elapsedTime;
float current,voltage,power,powerFactor,energyConsumedHour,energyConsumedDay,energyConsumedMonth;
float powerMonth,powerDay,powerHour=0;
int timestamp,currentHour,currentDay,currentMonth;

void setup(){  
  Serial.begin(9600);
  initWiFi();
  configTime(0, 0, ntpServer);
  currentDay = rtc.getDay();
  currentMonth = rtc.getMonth();
  currentHour = rtc.getHour(true);
  sendFase.configuration();
}

void loop(){
  currentMillis = millis();
  elapsedTime = currentMillis - lastLoopMillis;
  fase.getMeter();
  if (elapsedTime >= 1000){ // Realizar cálculos cada segundo
    current = fase.getCurrent();
    voltage = fase.getVoltage();
    power = fase.getPower();
    powerFactor = fase.getPowerFactor();
    energyConsumedHour += (power * elapsedTime) / 3600000.0; // Multiplicar la potencia por el tiempo y convertir a kWh
    energyConsumedDay += (power * elapsedTime) / 3600000.0;
    energyConsumedMonth += (power * elapsedTime)/3600000.0;
    lastLoopMillis = currentMillis;
  }
  timestamp = getTime();// Obtener la hora actual
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    sendFase.sendNode("/current",current,timestamp);
    sendFase.sendNode("/voltage",voltage,timestamp);
    sendFase.sendNode("/power",power*1000,timestamp);
    sendFase.sendNode("/powerFactor",powerFactor,timestamp);
    sendFase.sendNode("/powerHourCurrent",energyConsumedHour,timestamp);
    sendFase.sendNode("/powerDayCurrent",energyConsumedDay,timestamp);
    sendFase.sendNode("/powerMonthCurrent",energyConsumedMonth,timestamp);
  }
  if (currentHour != rtc.getHour(true)){ // Si ha pasado una hora
    sendFase.sendNode("/powerHour", energyConsumedHour, timestamp);
    energyConsumedHour = 0.0; // Reiniciar el contador de energía para la próxima hora
    currentHour = rtc.getHour(true);
  }
  if (currentDay !=rtc.getDay()){ // Si ha pasado un dia
    sendFase.sendNode("/powerDay", energyConsumedDay, timestamp);
    energyConsumedDay = 0.0; // Reiniciar el contador de energía para el próximo día
    currentDay = rtc.getDay();
  }
  if (currentMonth !=rtc.getMonth()){ // Si el mes cambio
    sendFase.sendNode("/powerMonth", energyConsumedMonth, timestamp);
    energyConsumedMonth = 0.0; // Reiniciar el contador de energía para el próximo mes
    currentMonth = rtc.getMonth();
  }
}