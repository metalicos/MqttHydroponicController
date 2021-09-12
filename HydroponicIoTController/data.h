#include "macros.h"
#define UID "a4b4"
struct CyberDoneData {
  uint8_t dateSecond = 0;
  uint8_t dateMinute = 0;
  uint8_t dateHour = 0;
  uint8_t dateDay = 0;
  uint8_t dateMonth = 0;
  uint16_t dateYear = 0;
  uint32_t phValue = 0;
  uint32_t temperatureValue = 0;
  uint32_t ecValue = 0;
  uint32_t tdsValue = 0;

  uint32_t sendDataToServerEvery = 1000;
  uint16_t checkSensorEvery = 1000;
  bool turnOff = false;
  uint16_t restartCounter = 0;
  const char uId[20] = UID;

  char wifiSSID[20] = {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
  char wifiPASS[20] = {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
};
