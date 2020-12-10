/**************************************************************************/
/*!
    @file     SensorDigitalSwitch.h
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

#ifndef _SensorDigitalSwitch_h_
#define _SensorDigitalSwitch_h_

#include "Sensor.h"

class SensorDigitalSwitch : public Sensor {
  private:
    bool lastPostedValue = 0;
    uint8_t _port;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorDigitalSwitch (long, String, String, String, uint8_t, int, int, SensorCalculation*);
};

#endif