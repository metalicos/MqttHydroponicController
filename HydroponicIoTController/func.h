#include "global.h"

void saveAllDataToMemory() {
  memory.putUChar("daSec", cdd.dateSecond);
  memory.putUChar("daMin", cdd.dateMinute);
  memory.putUChar("daHou", cdd.dateHour);
  memory.putUChar("daDay", cdd.dateDay);
  memory.putUChar("daMon", cdd.dateMonth);
  memory.putUShort("daYea", cdd.dateYear);
  memory.putFloat("phVal", cdd.phValue);
  memory.putFloat("teVal", cdd.temperatureValue);
  memory.putFloat("ecVal", cdd.ecValue);
  memory.putFloat("tdVal", cdd.tdsValue);
  memory.putUInt("seDatToSer", cdd.sendDataToServerEvery);
  memory.putUShort("chSenEve", cdd.checkSensorEvery);
  memory.putBool("tuOff", cdd.turnOff);
  memory.putUShort("reCou", cdd.restartCounter);
  memory.putString("wiSSID", String(cdd.wifiSSID));
  memory.putString("wiPASS", String(cdd.wifiPASS));
}

void readAllDataFromMemoryToRAM() {
  cdd.dateSecond = memory.getUChar("daSec", 0);
  cdd.dateMinute = memory.getUChar("daMin", 0);
  cdd.dateHour = memory.getUChar("daHou", 0);
  cdd.dateDay = memory.getUChar("daDay", 0);
  cdd.dateMonth = memory.getUChar("daMon", 0);
  cdd.dateYear = memory.getUShort("daYea", 0);
  cdd.phValue = memory.getFloat("phVal", 0.0f);
  cdd.temperatureValue = memory.getFloat("teVal", 0.0f);
  cdd.ecValue = memory.getFloat("ecVal", 0.0f);
  cdd.tdsValue = memory.getFloat("tdVal", 0.0f);
  cdd.sendDataToServerEvery = memory.getUInt("seDatToSer", 0);
  cdd.checkSensorEvery = memory.getUShort("chSenEve", 0);
  cdd.turnOff = memory.getBool("tuOff", true);
  cdd.restartCounter = memory.getUShort("reCou", 0);
  memory.getString("wifiSSID", cdd.wifiSSID, 20);
  memory.getString("wifiPASS", cdd.wifiPASS, 20);
}

void setupMemory() {
  memory.begin("app-data", false);
  if (memory.getUChar("isFirst", 12) == 233) {
    memory.putUChar("isFirst", 233);
    saveAllDataToMemory();
  }
  readAllDataFromMemoryToRAM();
  cdd.restartCounter++;
  memory.putUShort("rstCounter", cdd.restartCounter);
}

void subscribeEndpoints() {
  mqttClient.subscribe("cyberdone/"UID"/dateSecond");
  mqttClient.subscribe("cyberdone/"UID"/dateMinute");
  mqttClient.subscribe("cyberdone/"UID"/dateHour");
  mqttClient.subscribe("cyberdone/"UID"/dateDay");
  mqttClient.subscribe("cyberdone/"UID"/dateMonth");
  mqttClient.subscribe("cyberdone/"UID"/dateYear");
  mqttClient.subscribe("cyberdone/"UID"/phValue");
  mqttClient.subscribe("cyberdone/"UID"/temperatureValue");
  mqttClient.subscribe("cyberdone/"UID"/ecValue");
  mqttClient.subscribe("cyberdone/"UID"/tdsValue");
  mqttClient.subscribe("cyberdone/"UID"/sendDataToServerEvery");
  mqttClient.subscribe("cyberdone/"UID"/checkSensorEvery");
  mqttClient.subscribe("cyberdone/"UID"/turnOff");
  mqttClient.subscribe("cyberdone/"UID"/restartCounter");
  mqttClient.subscribe("cyberdone/"UID"/wifiSSID");
  mqttClient.subscribe("cyberdone/"UID"/wifiPASS");
}

String createJSON() {
  return "{\"dateSecond\":\"" + String(cdd.dateSecond) + "\",\"dateMinute\":\"" + String(cdd.dateMinute) + "\",\"dateHour\":\"" + String(cdd.dateHour) + "\",\"dateDay\":\"" + String(cdd.dateDay) + "\",\"dateMonth\":\"" + String(cdd.dateMonth) + "\",\"dateYear\":\"" + String(cdd.dateYear) + "\",\"phValue\":\"" + String(cdd.phValue) + "\",\"temperatureValue\":\"" + String(cdd.temperatureValue) + "\",\"ecValue\":\"" + String(cdd.ecValue) + "\",\"tdsValue\":\"" + String(cdd.tdsValue) + "\",\"sendDataToServerEvery\":\"" + String(cdd.sendDataToServerEvery) + "\",\"checkSensorEvery\":\"" + String(cdd.checkSensorEvery) + "\",\"turnOff\":\"" + String(cdd.turnOff) + "\",\"restartCounter\":\"" + String(cdd.restartCounter) + "\"}";
}

void callback(char* topic, byte* payload, unsigned int length) {
  const char *data = reinterpret_cast<const char*>(payload);

  int32_t int32_t_Value = 0;
  sscanf(data, "#%d#", &int32_t_Value);

  uint32_t uint32_t_Value = 0;
  sscanf(data, "#%ud#", &uint32_t_Value);

  int64_t int64_t_Value = 0;
  sscanf(data, "#%ld#", &int64_t_Value);

  uint64_t uint64_t_Value = 0;
  sscanf(data, "#%lud#", &uint64_t_Value);

  float floatValue = 0.0f;
  sscanf(data, "#%f#", &floatValue);

  double doubleValue = 0.0;
  sscanf(data, "#%Lf#", &doubleValue);


  if (strcmp(topic, "cyberdone/"UID"/dateSecond") == 0) {
    cdd.dateSecond = (uint8_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/dateMinute") == 0) {
    cdd.dateMinute = (uint8_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/dateHour") == 0) {
    cdd.dateHour = (uint8_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/dateDay") == 0) {
    cdd.dateDay = (uint8_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/dateMonth") == 0) {
    cdd.dateMonth = (uint8_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/dateYear") == 0) {
    cdd.dateYear = (uint16_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/phValue") == 0) {
    cdd.phValue = (float) floatValue; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/temperatureValue") == 0) {
    cdd.temperatureValue = (float) floatValue; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/ecValue") == 0) {
    cdd.ecValue = (float) floatValue; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/tdsValue") == 0) {
    cdd.tdsValue = (float) floatValue; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/sendDataToServerEvery") == 0) {
    cdd.sendDataToServerEvery = (uint32_t) uint32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/checkSensorEvery") == 0) {
    cdd.checkSensorEvery = (uint16_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/turnOff") == 0) {
    cdd.turnOff = (bool) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/restartCounter") == 0) {
    cdd.restartCounter = (uint16_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UID"/wifiSSID") == 0) {
    sscanf(data, " %s ", cdd.wifiSSID);
    return;
  }

  if (strcmp(topic, "cyberdone/"UID"/wifiPASS") == 0) {
    sscanf(data, " %s ", cdd.wifiPASS);
    return;
  }

}

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

void singleClick(Button2& btn) {
}

void pressed(Button2& btn) {
}

void released(Button2& btn) {
}

void longSingleClick(Button2& btn) {
}

void doubleClick(Button2& btn) {
}

void tripleClick(Button2& btn) {
}

void setupDosators() {
}

void regulatorLoop() {
}
