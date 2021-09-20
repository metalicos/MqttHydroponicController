#include "macros.h"
//#define UUID "80aeff91-bf00-4d67-a44f-479344820f5c"
#define UUID "id"
struct DeviceData {
  uint8_t dateSecond = 0;
  uint8_t dateMinute = 0;
  uint8_t dateHour = 0;
  uint8_t dateDay = 0;
  uint8_t dateMonth = 0;
  uint16_t dateYear = 0;

  double phValue = 5.5;
  double temperatureValue = 23.0;
  double ecValue = 0;
  uint32_t tdsValue = 600; //ppm

  double phUpPumpRegulatorKp = 1.0;
  double phUpPumpRegulatorKi = 5.0;
  double phUpPumpRegulatorKd = 0.0;
  uint32_t phUpPumpRegulatorDt = 500;

  double phDownPumpRegulatorKp = 1.0;
  double phDownPumpRegulatorKi = 5.0;
  double phDownPumpRegulatorKd = 0.0;
  uint32_t phDownPumpRegulatorDt = 500;

  double fertilizerPumpRegulatorKp = 1.0;
  double fertilizerPumpRegulatorKi = 5.0;
  double fertilizerPumpRegulatorKd = 5.0;
  uint32_t fertilizerPumpRegulatorDt = 500;

  double mlPerSecond = 1.5;
  
  uint32_t phUpDoseML = 5;
  uint32_t phDownDoseML = 5;
  uint32_t fertilizerDoseML = 5;
  
  uint64_t recheckDoseCompensationAfterMS = 120000;

  double setupPhValue = 6.2;
  uint32_t setupTdsValue = 600; //ppm
  double setupTemperatureValue = 22.0;

  uint64_t sendDataToServerEvery = 1000;
  uint64_t checkSensorEvery = 10;
  bool turnOff = false;
  uint64_t restartCounter = 0;

  char wifiSSID[20] = {'A','B','C'}; //  {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
  char wifiPASS[20] = {'0', '6', '8', '1', '8', '4', '5', '0', '4', '0'};
};
