/**************************************************************************/
/*!
    @file     SensorAnalogue.cpp
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

#include "SensorAnalogue.h"

extern boolean isResetting;
extern int powerMode;

SensorAnalogue::SensorAnalogue (long id, String category, String shortName, String name, uint8_t port, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {
  
  pinMode(port, INPUT);
  _port = port;
}

SensorAnalogue::SensorAnalogue (long id, String category, String shortName, String name, int rSplit, uint8_t port, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {
  
  pinMode(port, INPUT);
  _port = port;
  _rSplit = rSplit;
}

void SensorAnalogue::preCycle(int cycleId)
{
}

Data* SensorAnalogue::read(bool shouldPostData)
{
  if(!isResetting)
  {
    float adc = 0;

    for (int i = 0; i < 10; i++)
    {
      adc = adc + (float) analogRead(_port);
    }

    adc = adc / 10.0;

    if(!std::isinf(adc))
    {
      if(_rSplit!=0)
      {
        double rT = ((double) adc)*_rSplit/(4095-adc);
        shouldPostData = smartSensorCheck(rT, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, (float)rT, shouldPostData);
      }
      else
      {
        shouldPostData = smartSensorCheck(adc, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, adc, shouldPostData);
      }
    }
  }

  return NULL;

}

boolean SensorAnalogue::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
{
  if(powerMode == 3)
  {
    if(!shouldPostData)
    {
      if(!isnan(lastPostedValue))
      {
          if(lastPostedValue-currentRawValue>threshhold|| lastPostedValue-currentRawValue<-threshhold)
          {
            shouldPostData = true;
          }
      }
    }

    if(shouldPostData)
      lastPostedValue = currentRawValue;
  }

  return shouldPostData;
}