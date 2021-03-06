/**************************************************************************/
/*!
    @file     SensorBMx280.h
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

#include <BME280I2C.h>
// #include <Wire.h>

#ifndef _SensorBMx280_h_
#define _SensorBMx280_h_

#include "../Sensor.h"

class SensorBMx280 : public Sensor {
  private:
    static BME280I2C* bme76;
    static BME280I2C* bme77;
    BME280I2C* bme;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorBMx280 (long, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif