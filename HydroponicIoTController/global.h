#include <PubSubClient.h>
#include <WiFi.h>
#include "RTClib.h"
#include "data.h"
#include <Preferences.h>
#include "Button2.h";

//#define MQTT_SERVER "b85b0aa3fd23.sn.mynetname.net"
#define MQTT_SERVER "192.168.1.200"                       
#define ADC_MAX 4095

unsigned long lastSendToServer = 0;
unsigned long lastTransmit = 0;
unsigned long lastReceive = 0;

WiFiClient espClient;                                                     
PubSubClient mqttClient(espClient);                                         
CyberDoneData cdd;
RTC_Millis rtc;
Preferences memory;
Button2 button = Button2(5);
