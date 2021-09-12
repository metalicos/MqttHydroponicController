#include "func.h"

void setup() {
  Serial.begin(115200);
  setupMemory();
  setup_wifi();
  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(callback);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  setupDosators();
}

void loop() {
  mqttLoop();
  if (!cdd.turnOff) {
    regulatorLoop();
  }
}
