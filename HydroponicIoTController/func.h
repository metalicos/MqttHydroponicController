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

struct PHCalibrationValue getPhStruct() {
  struct PHCalibrationValue calValue = phSensor.getCalibrationValue();
  calValue.point = memory.getChar("point", '1');
  calValue.value[0] = memory.getDouble("value[0]", 4.0);
  calValue.value[1] = memory.getDouble("value[1]", 7.0);
  calValue.adc[0] = memory.getLong("adc[0]", 1500);
  calValue.adc[1] = memory.getLong("adc[1]", 2000);
  calValue.slope = memory.getDouble("slope", 0.01);
  calValue.adcOffset = memory.getLong("adcOffset", 700);
  return calValue;
}

void savePhStruct() {
  struct PHCalibrationValue calValue = phSensor.getCalibrationValue();
  memory.putChar("point", calValue.point);
  memory.putDouble("value[0]", calValue.value[0]);
  memory.putDouble("value[1]", calValue.value[1]);
  memory.putLong("adc[0]", calValue.adc[0]);
  memory.putLong("adc[1]", calValue.adc[1]);
  memory.putDouble("slope", calValue.slope);
  memory.putLong("adcOffset", calValue.adcOffset);
}

void setupMemory() {
  memory.begin("app-data", false);
  if (memory.getUChar("isFirst", 12) == 233) {
    memory.putUChar("isFirst", 233);
    memory.putDouble("tdskValue", 1.0);
    savePhStruct();
    saveAllDataToMemory();
  }
  tdsSensor.setKvalue(memory.getDouble("tdskValue", 1.0));
  phSensor.initialize(getPhStruct());

  readAllDataFromMemoryToRAM();

  cdd.restartCounter++;
  memory.putULong("reCou", cdd.restartCounter);
}

void subscribeEndpoints() {
  mqttClient.subscribe("cyberdone/"UUID"/dateSecond");
  mqttClient.subscribe("cyberdone/"UUID"/dateMinute");
  mqttClient.subscribe("cyberdone/"UUID"/dateHour");
  mqttClient.subscribe("cyberdone/"UUID"/dateDay");
  mqttClient.subscribe("cyberdone/"UUID"/dateMonth");
  mqttClient.subscribe("cyberdone/"UUID"/dateYear");

  mqttClient.subscribe("cyberdone/"UUID"/phValue");
  mqttClient.subscribe("cyberdone/"UUID"/temperatureValue");
  mqttClient.subscribe("cyberdone/"UUID"/ecValue");
  mqttClient.subscribe("cyberdone/"UUID"/tdsValue");

  mqttClient.subscribe("cyberdone/"UUID"/sendDataToServerEvery");
  mqttClient.subscribe("cyberdone/"UUID"/checkSensorEvery");
  mqttClient.subscribe("cyberdone/"UUID"/turnOff");
  mqttClient.subscribe("cyberdone/"UUID"/restartCounter");

  mqttClient.subscribe("cyberdone/"UUID"/calibratePhLow");
  mqttClient.subscribe("cyberdone/"UUID"/calibratePhHigh");
  mqttClient.subscribe("cyberdone/"UUID"/calibratePhClear");

  mqttClient.subscribe("cyberdone/"UUID"/calibrateTds");
  mqttClient.subscribe("cyberdone/"UUID"/calibrateTdsClear");

  mqttClient.subscribe("cyberdone/"UUID"/readAll");
  mqttClient.subscribe("cyberdone/"UUID"/saveAll");
  mqttClient.subscribe("cyberdone/"UUID"/restart");

  mqttClient.subscribe("cyberdone/"UUID"/wifiSSID");
  mqttClient.subscribe("cyberdone/"UUID"/wifiPASS");
}

String createJSON() {
  return "{\"dateSecond\":\"" + String(cdd.dateSecond) + "\",\"dateMinute\":\"" + String(cdd.dateMinute) + "\",\"dateHour\":\"" + String(cdd.dateHour) + "\",\"dateDay\":\"" + String(cdd.dateDay) + "\",\"dateMonth\":\"" + String(cdd.dateMonth) + "\",\"dateYear\":\"" + String(cdd.dateYear) + "\",\"phValue\":\"" + String(cdd.phValue) + "\",\"temperatureValue\":\"" + String(cdd.temperatureValue) + "\",\"ecValue\":\"" + String(cdd.ecValue) + "\",\"tdsValue\":\"" + String(cdd.tdsValue) + "\",\"sendDataToServerEvery\":\"" + String((uint32_t)cdd.sendDataToServerEvery) + "\",\"checkSensorEvery\":\"" + String((uint32_t)cdd.checkSensorEvery) + "\",\"turnOff\":\"" + String(cdd.turnOff) + "\",\"restartCounter\":\"" + String((uint32_t)cdd.restartCounter) + "\"}";
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


  if (strcmp(topic, "cyberdone/"UUID"/dateSecond") == 0) {
    cdd.dateSecond = (uint8_t) int32_t_Value;
    memory.putUChar("daSec", cdd.dateSecond);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dateMinute") == 0) {
    cdd.dateMinute = (uint8_t) int32_t_Value;
    memory.putUChar("daMin", cdd.dateMinute);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dateHour") == 0) {
    cdd.dateHour = (uint8_t) int32_t_Value;
    memory.putUChar("daHou", cdd.dateHour);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dateDay") == 0) {
    cdd.dateDay = (uint8_t) int32_t_Value;
    memory.putUChar("daDay", cdd.dateDay);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dateMonth") == 0) {
    cdd.dateMonth = (uint8_t) int32_t_Value;
    memory.putUChar("daMon", cdd.dateMonth);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dateYear") == 0) {
    cdd.dateYear = (uint16_t) int32_t_Value;
    memory.putUShort("daYea", cdd.dateYear);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phValue") == 0) {
    cdd.phValue = (float) floatValue;
    memory.putFloat("phVal", cdd.phValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/temperatureValue") == 0) {
    cdd.temperatureValue = (float) floatValue;
    memory.putFloat("teVal", cdd.temperatureValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/ecValue") == 0) {
    cdd.ecValue = (float) floatValue;
    memory.putFloat("ecVal", cdd.ecValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/tdsValue") == 0) {
    cdd.tdsValue = (float) floatValue;
    memory.putFloat("tdVal", cdd.tdsValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/sendDataToServerEvery") == 0) {
    cdd.sendDataToServerEvery = uint32_t_Value;
    memory.putUInt("seDatToSer", cdd.sendDataToServerEvery);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/checkSensorEvery") == 0) {
    cdd.checkSensorEvery = int32_t_Value;
    memory.putUShort("chSenEve", cdd.checkSensorEvery);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/turnOff") == 0) {
    cdd.turnOff = (bool) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restartCounter") == 0) {
    cdd.restartCounter = (uint16_t) int32_t_Value; return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiSSID") == 0) {
    sscanf(data, " %s ", cdd.wifiSSID);
    memory.putString("wiSSID", String(cdd.wifiSSID));
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiPASS") == 0) {
    sscanf(data, " %s ", cdd.wifiPASS);
    memory.putString("wiPASS", String(cdd.wifiPASS));
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhLow") == 0) {
    phSensor.calibrationLow(doubleValue);
    savePhStruct();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhHigh") == 0) {
    phSensor.calibrationHigh(doubleValue);
    savePhStruct();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhClear") == 0) {
    phSensor.calibrationClear();
    savePhStruct();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibrateTds") == 0) {
    memory.putDouble("tdskValue", tdsSensor.tdsCalibration(doubleValue));
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibrateTdsClear") == 0) {
    memory.putDouble("tdskValue", tdsSensor.tdsClearCalibration());
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restart") == 0) {
    ESP.restart();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/saveAll") == 0) {
    saveAllDataToMemory();
  }

  if (strcmp(topic, "cyberdone/"UUID"/readAll") == 0) {
    readAllDataFromMemoryToRAM();
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
    if (mqttClient.connect(UUID)) subscribeEndpoints();
  }
}

void setCurrentDateTime() {
  now = rtc.now();
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
  if (millis() - lastSendToServer > (uint64_t) cdd.sendDataToServerEvery) {
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

void setupSensors() {
  temperatureSensor.setResolution(12);
  tdsSensor.begin();
}

void sensorsLoop() {
  tdsSensor.setTemperature(cdd.temperatureValue);
  tdsSensor.update();

  cdd.temperatureValue = temperatureSensor.getTempC();
  cdd.tdsValue = tdsSensor.getTdsValue();
  cdd.ecValue = tdsSensor.getEcValue();

  cdd.phValue = phSensor.singleReading().getpH();
  phSensor.temperatureCompensation(cdd.temperatureValue);
}

void regulatorLoop() {
}
