#include "macros.h"
//#define UUID "80aeff91-bf00-4d67-a44f-479344820f5c"
#define UUID "id"
struct DeviceData {
  double phValue = 5.5;
  double temperatureValue = 23.0;
  double ecValue = 0;
  uint32_t tdsValue = 600; //ppm

  double mlPerMilisecond = 0.00106666666;     //64ml -> 60sec
  double regulateErrorPhUp = 0.5;             //0.5 PH
  double regulateErrorPhDown = 0.5;           //0.5 PH
  double regulateErrorFertilizer = 20;         //5 TDS
  double phUpDoseMl = 2.0;
  double phDownDoseMl = 2.0;
  double fertilizerDoseMl = 2.0;
  uint64_t recheckDosatorsAfterMs = 10000;
  double setupPhValue = 6.2;
  uint32_t setupTdsValue = 600; //ppm
  double setupTemperatureValue = 22.0;

  uint64_t sendDataToServerEvery = 1000;
  bool deviceEnable = true;
  bool dosatorsEnable = true;
  bool sensorsEnable = true;
  
  uint64_t restartCounter = 0;
  char wifiSSID[20] = {'A', 'B', 'C'}; //  {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
  char wifiPASS[20] = {'0', '6', '8', '1', '8', '4', '5', '0', '4', '0'};
  uint8_t dateSecond = 0;
  uint8_t dateMinute = 0;
  uint8_t dateHour = 0;
  uint8_t dateDay = 0;
  uint8_t dateMonth = 0;
  uint16_t dateYear = 0;
};
