/**************************************************************************/
/*!
    @file     Data.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include <Arduino.h>

#ifndef _Data_h_
#define _Data_h_

#include "../input/Sensor.h"

class Sensor;

class Data {
  private:
    Sensor *_sensor;
    float _valueFloat;
    int _valueInt;
    boolean _valueBoolean;
    String _unit;
    int _type;
  public:
    Data(Sensor*, float, String);
    Data(Sensor*, int, String);
    Data(Sensor*, boolean, String);
    String getValueString(void);
    String getRequestString(void);
    Sensor* getSensor(void);
};

#endif