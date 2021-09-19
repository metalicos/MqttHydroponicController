#include "global.h"

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(cdd.wifiSSID, cdd.wifiPASS);
  randomSeed(micros());
}

void reconnect() {
  if (!mqttClient.connected()) {
    if (mqttClient.connect(UID))
      subscribeEndpoints();
  }
}

void setCurrentDateTime() {
  DateTime now = rtc.now();
  cdd.dateSecond = now.second();
  cdd.dateMinute = now.minute();
  cdd.dateHour = now.hour();
  cdd.dateDay = now.day();
  cdd.dateMonth = now.month();
  cdd.dateYear = now.year();
}

void mqttLoop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  if (millis() - lastSendToServer > (unsigned long)cdd.sendDataToServerEvery) {
    lastSendToServer = millis();
    mqttClient.publish("hydro-chip", createJSON().c_str());
    setCurrentDateTime();
  }
}

void setupClock() {
  rtc.begin(DateTime(cdd.dateYear, cdd.dateMonth, cdd.dateDay, cdd.dateHour, cdd.dateMinute, cdd.dateSecond));
}

void setupDosators() {
}

void regulatorLoop() {
  char arr[100];
  Serial.println(rtc.now().toString(arr));
}
