#include <PubSubClient.h>
#include <WiFi.h>
#include "RTClib.h"
#include "data.h"
#include <Preferences.h>
#include "DS18B20.h"
#include "CyberDonePhSensor.h"
#include "CyberDoneTdsSensor.h"
#include "CDPidRegulator.h"

//#define MQTT_SERVER "b85b0aa3fd23.sn.mynetname.net"
#define MQTT_SERVER "192.168.1.100"
#define ADC_MAX 4095
#define ADC_REFERENCE_VOLTAGE 3.3
#define PH_SENSOR_PORT 34
#define TDS_SENSOR_PORT 35
#define TEMPERATURE_SENSOR_PORT 32
#define PH_UP_DOSATOR_PORT_A 23
#define PH_UP_DOSATOR_PORT_B 22
#define PH_DOWN_DOSATOR_PORT_A 19
#define PH_DOWN_DOSATOR_PORT_B 18
#define FERTILIZER_DOSATOR_PORT_A 5
#define FERTILIZER_DOSATOR_PORT_B 17

uint64_t lastSendToServer = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DeviceData cdd;
RTC_Millis rtc;
Preferences memory;

DS18B20 temperatureSensor(TEMPERATURE_SENSOR_PORT);
CyberDonePhSensor phSensor(PH_SENSOR_PORT);
CyberDoneTdsSensor tdsSensor(TDS_SENSOR_PORT, ADC_REFERENCE_VOLTAGE, ADC_MAX);
CDPidRegulator phUpPumpRegulator, phDownPumpRegulator, fertilizerPumpRegulator;
