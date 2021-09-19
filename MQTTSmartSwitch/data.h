#include "macros.h"
#define UID "a4b4"
struct CyberDoneData {
  uint8_t dateSecond = 0;
  uint8_t dateMinute = 0;
  uint8_t dateHour = 0;
  uint8_t dateDay = 18;
  uint8_t dateMonth = 9;
  uint16_t dateYear = 2021;

  uint8_t relayValue = 0;
  uint8_t relayLogic = 0;
  
  uint32_t sendDataToServerEvery = 1000;
  uint16_t restartCounter = 0;
  const char uId[20] = UID;

  char wifiSSID[20] = {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
  char wifiPASS[20] = {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
};
