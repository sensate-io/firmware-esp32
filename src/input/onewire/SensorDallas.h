/**************************************************************************/
/*!
    @file     SensorDallas.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP32 firmware is used to connect ESP32 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include <DallasTemperature.h>

#ifndef _SensorDallas_h_
#define _SensorDallas_h_

#include "../Sensor.h"

//DS18B20
#define ONE_WIRE_MAX_DEV 15 //The maximum number of devices per Bus

class SensorDallas : public Sensor {
  private:
    static boolean initDallas0;
    static boolean initDallas1;
    static boolean initDallas2;
    static boolean initDallas3;
    static boolean initDallas4;
    static boolean initDallas5;
    static boolean initDallas12;
    static boolean initDallas13;
    static boolean initDallas14;
    static boolean initDallas15;
    static boolean initDallas16;
    static boolean initDallas17;
    static boolean initDallas18;
    static boolean initDallas19;
    static boolean initDallas21;
    static boolean initDallas22;
    static boolean initDallas23;
    static boolean initDallas25;
    static boolean initDallas26;
    static boolean initDallas27;
    static boolean initDallas32;
    static boolean initDallas33;
    static DallasTemperature* dallasTemperature0;
    static DallasTemperature* dallasTemperature1;
    static DallasTemperature* dallasTemperature2;
    static DallasTemperature* dallasTemperature3;
    static DallasTemperature* dallasTemperature4;
    static DallasTemperature* dallasTemperature5;
    static DallasTemperature* dallasTemperature12;
    static DallasTemperature* dallasTemperature13;
    static DallasTemperature* dallasTemperature14;
    static DallasTemperature* dallasTemperature15;
    static DallasTemperature* dallasTemperature16;
    static DallasTemperature* dallasTemperature17;
    static DallasTemperature* dallasTemperature18;
    static DallasTemperature* dallasTemperature19;
    static DallasTemperature* dallasTemperature21;
    static DallasTemperature* dallasTemperature22;
    static DallasTemperature* dallasTemperature23;
    static DallasTemperature* dallasTemperature25;
    static DallasTemperature* dallasTemperature26;
    static DallasTemperature* dallasTemperature27;
    static DallasTemperature* dallasTemperature32;
    static DallasTemperature* dallasTemperature33;
    static OneWire* oneWire0;
    static OneWire* oneWire1;
    static OneWire* oneWire2;
    static OneWire* oneWire3;
    static OneWire* oneWire4;
    static OneWire* oneWire5;
    static OneWire* oneWire12;
    static OneWire* oneWire13;
    static OneWire* oneWire14;
    static OneWire* oneWire15;
    static OneWire* oneWire16;
    static OneWire* oneWire17;
    static OneWire* oneWire18;
    static OneWire* oneWire19;
    static OneWire* oneWire21;
    static OneWire* oneWire22;
    static OneWire* oneWire23;
    static OneWire* oneWire25;
    static OneWire* oneWire26;
    static OneWire* oneWire27;
    static OneWire* oneWire32;
    static OneWire* oneWire33;
    DallasTemperature* dallasTemperature;
    int numberOfDevices;
    DeviceAddress devAddr[ONE_WIRE_MAX_DEV];
    int _channel;
    DeviceAddress _address;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorDallas (long, String, String, String, uint8_t, int, int, int, float, SensorCalculation*);
};

#endif