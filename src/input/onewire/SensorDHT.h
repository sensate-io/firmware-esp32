/**************************************************************************/
/*!
    @file     SensorDHT.h
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

#include <DHT.h>
#include <DHT_U.h>

#ifndef _SensorDHT_h_
#define _SensorDHT_h_

#include "../Sensor.h"

class SensorDHT : public Sensor {
  private:
    static boolean initDHT0;
    static boolean initDHT1;
    static boolean initDHT2;
    static boolean initDHT3;
    static boolean initDHT4;
    static boolean initDHT5;
    static boolean initDHT12;
    static boolean initDHT13;
    static boolean initDHT14;
    static boolean initDHT15;
    static boolean initDHT16;
    static boolean initDHT17;
    static boolean initDHT18;
    static boolean initDHT19;
    static boolean initDHT21;
    static boolean initDHT22;
    static boolean initDHT23;
    static boolean initDHT25;
    static boolean initDHT26;
    static boolean initDHT27;
    static boolean initDHT32;
    static boolean initDHT33;
    static DHT_Unified* dht0;
    static DHT_Unified* dht1;
    static DHT_Unified* dht2;
    static DHT_Unified* dht3;
    static DHT_Unified* dht4;
    static DHT_Unified* dht5;
    static DHT_Unified* dht12;
    static DHT_Unified* dht13;
    static DHT_Unified* dht14;
    static DHT_Unified* dht15;
    static DHT_Unified* dht16;
    static DHT_Unified* dht17;
    static DHT_Unified* dht18;
    static DHT_Unified* dht19;
    static DHT_Unified* dht21;
    static DHT_Unified* dht22;
    static DHT_Unified* dht23;
    static DHT_Unified* dht25;
    static DHT_Unified* dht26;
    static DHT_Unified* dht27;
    static DHT_Unified* dht32;
    static DHT_Unified* dht33;
    DHT_Unified* dht;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorDHT (long, String, String, String,String, uint8_t, int, int, float, SensorCalculation*);
};

#endif