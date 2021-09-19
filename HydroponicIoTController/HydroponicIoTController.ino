#include "func.h"

void setup() {
  //Serial.begin(115200);
  setupMemory();
  setupSensors();
  setupDosators();
  setup_wifi();
  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(callback);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  sensorsLoop();
  mqttLoop();
  if (!cdd.turnOff) {
    regulatorLoop();
  }
}
