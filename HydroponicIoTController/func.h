#include "global.h"

void startDosing(int pinA, int pinB, bool direction) {
  digitalWrite(pinA, direction == true ? 1 : 0);
  digitalWrite(pinB, direction == true ? 0 : 1);
}

void stopDosing(int pinA, int pinB) {
  digitalWrite(pinA, 0);
  digitalWrite(pinB, 0);
}

uint64_t doseTimeMs(double doseMl) {
  return lround(doseMl / cdd.mlPerMilisecond);
}

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
  memory.putString("wiSSID", String(cdd.wifiSSID));
  memory.putString("wiPASS", String(cdd.wifiPASS));
  memory.putDouble("mlPerMil", cdd.mlPerMilisecond);
  memory.putDouble("reErPhUp", cdd.regulateErrorPhUp);
  memory.putDouble("reErPhDo", cdd.regulateErrorPhDown);
  memory.putDouble("reErFer", cdd.regulateErrorFertilizer);
  memory.putDouble("phUpDoMl", cdd.phUpDoseMl);
  memory.putDouble("phDoDoMl", cdd.phDownDoseMl);
  memory.putDouble("feDoMl", cdd.fertilizerDoseMl);
  memory.putULong("reDoAfMs", cdd.recheckDosatorsAfterMs);
  memory.putDouble("sePhVal", cdd.setupPhValue);
  memory.putUInt("seTdsVal", cdd.setupTdsValue);
  memory.putDouble("seTempVal", cdd.setupTemperatureValue);

  memory.putBool("devEn", cdd.deviceEnable);
  memory.putBool("dosEn", cdd.dosatorsEnable);
  memory.putBool("senEn", cdd.sensorsEnable);
  memory.putULong("reCount", cdd.restartCounter);
}

void readAllDataFromMemoryToRAM() {
  cdd.dateSecond = memory.getUChar("daSec", 0);
  cdd.dateMinute = memory.getUChar("daMin", 0);
  cdd.dateHour = memory.getUChar("daHou", 0);
  cdd.dateDay = memory.getUChar("daDay", 0);
  cdd.dateMonth = memory.getUChar("daMon", 0);
  cdd.dateYear = memory.getUShort("daYea", 0);
  cdd.phValue = memory.getDouble("phVal", 0.0f);
  cdd.temperatureValue = memory.getDouble("teVal", 0.0f);
  cdd.ecValue = memory.getDouble("ecVal", 0.0f);
  cdd.tdsValue = memory.getDouble("tdVal", 0.0f);
  cdd.sendDataToServerEvery = memory.getUInt("seDatToSer", 0);
  cdd.restartCounter = memory.getULong("reCou", 0);
  memory.getString("wifiSSID", cdd.wifiSSID, 20);
  memory.getString("wifiPASS", cdd.wifiPASS, 20);

  cdd.mlPerMilisecond = memory.getDouble("mlPerMil", 0.00106666666);
  cdd.regulateErrorPhUp = memory.getDouble("reErPhUp", 0.5);
  cdd.regulateErrorPhDown = memory.getDouble("reErPhDo", 0.5);
  cdd.regulateErrorFertilizer = memory.getDouble("reErFer", 20);
  cdd.phUpDoseMl = memory.getDouble("phUpDoMl", 2.0);
  cdd.phDownDoseMl = memory.getDouble("phDoDoMl", 2.0);
  cdd.fertilizerDoseMl = memory.getDouble("feDoMl", 2.0);
  cdd.recheckDosatorsAfterMs = memory.getULong("reDoAfMs", 10000);
  cdd.setupPhValue = memory.getDouble("sePhVal", 6.2);
  cdd.setupTdsValue = memory.getUInt("seTdsVal", 600);
  cdd.setupTemperatureValue = memory.getDouble("seTempVal", 22.0);
  cdd.deviceEnable = memory.getBool("devEn", true);
  cdd.dosatorsEnable = memory.getBool("dosEn", true);
  cdd.sensorsEnable = memory.getBool("senEn", true);
  cdd.restartCounter = memory.getULong("reCount", 0);
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

  cdd.restartCounter = cdd.restartCounter + 1;
  memory.putULong("reCount", cdd.restartCounter);
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

  mqttClient.subscribe("cyberdone/"UUID"/phUpStart");
  mqttClient.subscribe("cyberdone/"UUID"/phUpStop");
  mqttClient.subscribe("cyberdone/"UUID"/phDownStart");
  mqttClient.subscribe("cyberdone/"UUID"/phDownStop");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerStart");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerStop");

  mqttClient.subscribe("cyberdone/"UUID"/mlPerMilisecond");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorPhUp");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorPhDown");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorFertilizer");
  mqttClient.subscribe("cyberdone/"UUID"/phUpDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/phDownDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/recheckDosatorsAfterMs");
  mqttClient.subscribe("cyberdone/"UUID"/setupPhValue");
  mqttClient.subscribe("cyberdone/"UUID"/setupTdsValue");
  mqttClient.subscribe("cyberdone/"UUID"/setupTemperatureValue");

  mqttClient.subscribe("cyberdone/"UUID"/sendDataToServerEvery");
  mqttClient.subscribe("cyberdone/"UUID"/checkSensorEvery");

  mqttClient.subscribe("cyberdone/"UUID"/deviceEnable");
  mqttClient.subscribe("cyberdone/"UUID"/dosatorsEnable");
  mqttClient.subscribe("cyberdone/"UUID"/sensorsEnable");
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
  String str = "{\n";
  str += "\"phValue\":\"" + String(cdd.phValue, 2) + "\",\n";
  str += "\"temperatureValue\":\"" + String(cdd.temperatureValue, 2) + "\",\n" ;
  str += "\"ecValue\":\"" + String(cdd.ecValue, 2) + "\",\n";
  str += "\"tdsValue\":\"" + String(cdd.tdsValue) + "\",\n" ;
  str += "\"mlPerMilisecond\":\"" + String(cdd.mlPerMilisecond, 11) + "\",\n" ;
  str += "\"regulateErrorPhUp\":\"" + String(cdd.regulateErrorPhUp, 2) + "\",\n" ;
  str += "\"regulateErrorPhDown\":\"" + String(cdd.regulateErrorPhDown, 2) + "\",\n" ;
  str += "\"phUpDoseMl\":\"" + String(cdd.phUpDoseMl, 1) + "\",\n" ;
  str += "\"phDownDoseMl\":\"" + String(cdd.phDownDoseMl, 1) + "\",\n" ;
  str += "\"fertilizerDoseMl\":\"" + String(cdd.fertilizerDoseMl, 1) + "\",\n" ;
  str += "\"recheckDosatorsAfterMs\":\"" + String((uint32_t)cdd.recheckDosatorsAfterMs) + "\",\n" ;
  str += "\"setupPhValue\":\"" + String(cdd.setupPhValue, 2) + "\",\n" ;
  str += "\"setupTdsValue\":\"" + String(cdd.setupTdsValue) + "\",\n" ;
  str += "\"setupTemperatureValue\":\"" + String(cdd.setupTemperatureValue, 2) + "\",\n" ;
  str += "\"sendDataToServerEvery\":\"" + String((uint32_t)cdd.sendDataToServerEvery) + "\",\n" ;
  str += "\"deviceEnable\":\"" + String(cdd.deviceEnable) + "\",\n" ;
  str += "\"dosatorsEnable\":\"" + String(cdd.dosatorsEnable) + "\",\n" ;
  str += "\"sensorsEnable\":\"" + String(cdd.sensorsEnable) + "\",\n" ;
  str += "\"wifiSSID\":\"" + String(cdd.wifiSSID) + "\",\n" ;
  str += "\"wifiPASS\":\"" + String(cdd.wifiPASS) + "\",\n" ;
  str += "\"dateSecond\":\"" + String(cdd.dateSecond) + "\",\n" ;
  str += "\"dateMinute\":\"" + String(cdd.dateMinute) + "\",\n" ;
  str += "\"dateHour\":\"" + String(cdd.dateHour) + "\",\n" ;
  str += "\"dateDay\":\"" + String(cdd.dateDay) + "\",\n" ;
  str += "\"dateMonth\":\"" + String(cdd.dateMonth) + "\",\n" ;
  str += "\"dateYear\":\"" + String(cdd.dateYear) + "\",\n" ;
  str += "\"restartCounter\":\"" + String((uint32_t)cdd.restartCounter) + "\",\n" ;
  str += "\"UUID\":\""UUID"\"\n" ;
  str += "}";
  return str;
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

  if (strcmp(topic, "cyberdone/"UUID"/deviceEnable") == 0) {
    cdd.deviceEnable = (bool) int32_t_Value;
    memory.putBool("devEn", cdd.deviceEnable);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dosatorsEnable") == 0) {
    cdd.dosatorsEnable = (bool) int32_t_Value;
    memory.putBool("dosEn", cdd.dosatorsEnable);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/sensorsEnable") == 0) {
    cdd.sensorsEnable = (bool) int32_t_Value;
    memory.putBool("senEn", cdd.sensorsEnable);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restartCounter") == 0) {
    cdd.restartCounter = uint64_t_Value;
    memory.putULong("reCount", cdd.restartCounter);
    return;
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

  if (strcmp(topic, "cyberdone/"UUID"/mlPerMilisecond") == 0) {
    cdd.mlPerMilisecond = doubleValue;
    memory.putDouble("mlPerMil", cdd.mlPerMilisecond);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorPhUp") == 0) {
    cdd.regulateErrorPhUp = doubleValue;
    memory.putDouble("reErPhUp", cdd.regulateErrorPhUp);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorPhDown") == 0) {
    cdd.regulateErrorPhDown = doubleValue;
    memory.putDouble("reErPhDo", cdd.regulateErrorPhDown);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorFertilizer") == 0) {
    cdd.regulateErrorFertilizer = doubleValue;
    memory.putDouble("reErFer", cdd.regulateErrorFertilizer);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phUpDoseMl") == 0) {
    cdd.phUpDoseMl = doubleValue;
    memory.putDouble("phUpDoMl", cdd.phUpDoseMl);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phDownDoseMl") == 0) {
    cdd.phDownDoseMl = doubleValue;
    memory.putDouble("phDoDoMl", cdd.phDownDoseMl);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/fertilizerDoseMl") == 0) {
    cdd.fertilizerDoseMl = doubleValue;
    memory.putDouble("feDoMl", cdd.fertilizerDoseMl);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/recheckDosatorsAfterMs") == 0) {
    cdd.recheckDosatorsAfterMs = uint64_t_Value;
    memory.putULong("reDoAfMs", cdd.recheckDosatorsAfterMs);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupPhValue") == 0) {
    cdd.setupPhValue = doubleValue;
    memory.putDouble("sePhVal", cdd.setupPhValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupTdsValue") == 0) {
    cdd.setupTdsValue = uint32_t_Value;
    memory.putUInt("seTdsVal", cdd.setupTdsValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupTemperatureValue") == 0) {
    cdd.setupTemperatureValue = doubleValue;
    memory.putDouble("seTempVal", cdd.setupTemperatureValue);
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

  if (strcmp(topic, "cyberdone/"UUID"/phUpStart") == 0) {
    startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, (uint8_t) int32_t_Value);
  }
  if (strcmp(topic, "cyberdone/"UUID"/phUpStop") == 0) {
    stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
  }

  if (strcmp(topic, "cyberdone/"UUID"/phDownStart") == 0) {
    startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, (uint8_t) int32_t_Value);
  }
  if (strcmp(topic, "cyberdone/"UUID"/phDownStop") == 0) {
    stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
  }

  if (strcmp(topic, "cyberdone/"UUID"/fertilizerStart") == 0) {
    startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, (uint8_t) int32_t_Value);
  }
  if (strcmp(topic, "cyberdone/"UUID"/fertilizerStop") == 0) {
    stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
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
  if (millis() - lastSendToServer > (uint64_t) cdd.sendDataToServerEvery) {
    lastSendToServer = millis();
    mqttClient.publish("hydro-chip", createJSON().c_str());
    setCurrentDateTime();
  }
}

void setupClock() {
  rtc.begin(DateTime(cdd.dateYear, cdd.dateMonth, cdd.dateDay, cdd.dateHour, cdd.dateMinute, cdd.dateSecond));
}

uint64_t lastRecheckDosators = 0;

void dosatorsLoop() {
  if (millis() - lastRecheckDosators >= cdd.recheckDosatorsAfterMs) {
    lastRecheckDosators = millis();
    if (cdd.setupPhValue > (cdd.setupPhValue - (cdd.regulateErrorPhUp / 2))) {
      startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, true);
      delay(doseTimeMs(cdd.phUpDoseMl));
      stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
    }
    if (cdd.setupPhValue < (cdd.setupPhValue + (cdd.regulateErrorPhDown / 2))) {
      startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, true);
      delay(doseTimeMs(cdd.phDownDoseMl));
      stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
    }
    if (cdd.setupTdsValue > (cdd.setupTdsValue - (cdd.regulateErrorFertilizer / 2))) {
      startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, true);
      delay(doseTimeMs(cdd.fertilizerDoseMl));
      stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
    }
  }
}

void setupDosators() {
  pinMode(PH_UP_DOSATOR_PORT_A, OUTPUT);
  pinMode(PH_UP_DOSATOR_PORT_B, OUTPUT);
  pinMode(PH_DOWN_DOSATOR_PORT_A, OUTPUT);
  pinMode(PH_DOWN_DOSATOR_PORT_B, OUTPUT);
  pinMode(FERTILIZER_DOSATOR_PORT_A, OUTPUT);
  pinMode(FERTILIZER_DOSATOR_PORT_B, OUTPUT);
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
