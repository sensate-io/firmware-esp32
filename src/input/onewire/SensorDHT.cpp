/**************************************************************************/
/*!
    @file     SensorDHT.cpp
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

#include "SensorDHT.h"

boolean SensorDHT::initDHT0 = false;
boolean SensorDHT::initDHT1 = false;
boolean SensorDHT::initDHT2 = false;
boolean SensorDHT::initDHT3 = false;
boolean SensorDHT::initDHT4 = false;
boolean SensorDHT::initDHT5 = false;
boolean SensorDHT::initDHT12 = false;
boolean SensorDHT::initDHT13 = false;
boolean SensorDHT::initDHT14 = false;
boolean SensorDHT::initDHT15 = false;
boolean SensorDHT::initDHT16 = false;
boolean SensorDHT::initDHT17 = false;
boolean SensorDHT::initDHT18 = false;
boolean SensorDHT::initDHT19 = false;
boolean SensorDHT::initDHT21 = false;
boolean SensorDHT::initDHT22 = false;
boolean SensorDHT::initDHT23 = false;
boolean SensorDHT::initDHT25 = false;
boolean SensorDHT::initDHT26 = false;
boolean SensorDHT::initDHT27 = false;
boolean SensorDHT::initDHT32 = false;
boolean SensorDHT::initDHT33 = false;

DHT_Unified* SensorDHT::dht0 = NULL;
DHT_Unified* SensorDHT::dht1 = NULL;
DHT_Unified* SensorDHT::dht2 = NULL;
DHT_Unified* SensorDHT::dht3 = NULL;
DHT_Unified* SensorDHT::dht4 = NULL;
DHT_Unified* SensorDHT::dht5 = NULL;
DHT_Unified* SensorDHT::dht12 = NULL;
DHT_Unified* SensorDHT::dht13 = NULL;
DHT_Unified* SensorDHT::dht14 = NULL;
DHT_Unified* SensorDHT::dht15 = NULL;
DHT_Unified* SensorDHT::dht16 = NULL;
DHT_Unified* SensorDHT::dht17 = NULL;
DHT_Unified* SensorDHT::dht18 = NULL;
DHT_Unified* SensorDHT::dht19 = NULL;
DHT_Unified* SensorDHT::dht21 = NULL;
DHT_Unified* SensorDHT::dht22 = NULL;
DHT_Unified* SensorDHT::dht23 = NULL;
DHT_Unified* SensorDHT::dht25 = NULL;
DHT_Unified* SensorDHT::dht26 = NULL;
DHT_Unified* SensorDHT::dht27 = NULL;
DHT_Unified* SensorDHT::dht32 = NULL;
DHT_Unified* SensorDHT::dht33 = NULL;

extern boolean isResetting;
extern int powerMode;

SensorDHT::SensorDHT (long id, String category, String shortName, String name, String dhtType, uint8_t port, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {

  uint8_t type;

  if(dhtType=="DHT11")
    type = DHT11;
  else if(dhtType=="DHT21")
    type = DHT21;
  else if(dhtType=="DHT22")
  {
    type = DHT22;
  }
  
  if(port==0)
  {
    if(!initDHT0)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT0=true;
      dht0 = new DHT_Unified(0, type);
      dht0->begin();
    }
    dht = dht0;
  }
  else if(port==1)
  {
    if(!initDHT1)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT1=true;
      dht1 = new DHT_Unified(1, type);
      dht1->begin();
    }
    dht = dht1;
  }
  else if(port==2)
  {
    if(!initDHT2)
    {
      if(type!=DHT11)
        portFix(port);  
      initDHT2=true;
      dht2 = new DHT_Unified(2, type);
      dht2->begin();
    }
    dht = dht2;
  }
  else if(port==3)
  {
    if(!initDHT3)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT3=true;
      dht3 = new DHT_Unified(3, type);
      dht3->begin();
    }
    else
    {
      dht = dht3;
    }
  }
  else if(port==4)
  {
    if(!initDHT4)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT4=true;
      dht4 = new DHT_Unified(4, type);
      dht4->begin();
    }
    dht = dht4;
  }
  else if(port==5)
  {
    if(!initDHT5)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT5=true;
      dht5 = new DHT_Unified(5, type);
      dht5->begin();
    }
    else
    {
      dht = dht5;
    }
  }
  else if(port==12)
  {
    if(!initDHT12)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT12=true;
      dht12 = new DHT_Unified(12, type);
      dht12->begin();
    }
    dht = dht12;
  }
  else if(port==13)
  {
    if(!initDHT13)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT13=true;
      dht13 = new DHT_Unified(13, type);
      dht13->begin();
    }
    dht = dht13;
  }
  else if(port==14)
  {
    if(!initDHT14)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT14=true;
      dht14 = new DHT_Unified(14, type);
      dht14->begin();
    }
    dht = dht14;
  }
  else if(port==15)
  {
    if(!initDHT15)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT15=true;
      dht15 = new DHT_Unified(15, type);
      dht15->begin();
    }
    dht = dht15;
  }
  else if(port==16)
  {
    if(!initDHT16)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT16=true;
      dht16 = new DHT_Unified(16, type);
      dht16->begin();
    }
    dht = dht16;
  }
  else if(port==17)
  {
    if(!initDHT17)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT17=true;
      dht17 = new DHT_Unified(17, type);
      dht17->begin();
    }
    dht = dht17;
  }
  else if(port==18)
  {
    if(!initDHT18)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT18=true;
      dht18 = new DHT_Unified(18, type);
      dht18->begin();
    }
    dht = dht18;
  }
  else if(port==19)
  {
    if(!initDHT19)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT19=true;
      dht19 = new DHT_Unified(19, type);
      dht19->begin();
    }
    dht = dht19;
  }
  else if(port==21)
  {
    if(!initDHT21)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT21=true;
      dht21 = new DHT_Unified(21, type);
      dht21->begin();
    }
    dht = dht21;
  }
  else if(port==22)
  {
    if(!initDHT22)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT22=true;
      dht22 = new DHT_Unified(22, type);
      dht22->begin();
    }
    dht = dht22;
  }
  else if(port==23)
  {
    if(!initDHT23)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT23=true;
      dht23 = new DHT_Unified(23, type);
      dht23->begin();
    }
    dht = dht23;
  }
  else if(port==25)
  {
    if(!initDHT25)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT25=true;
      dht25 = new DHT_Unified(25, type);
      dht25->begin();
    }
    dht = dht25;
  }
  else if(port==26)
  {
    if(!initDHT26)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT26=true;
      dht26 = new DHT_Unified(26, type);
      dht26->begin();
    }
    dht = dht26;
  }
  else if(port==27)
  {
    if(!initDHT27)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT27=true;
      dht27 = new DHT_Unified(27, type);
      dht27->begin();
    }
    dht = dht27;
  }
  else if(port==32)
  {
    if(!initDHT32)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT32=true;
      dht32 = new DHT_Unified(32, type);
      dht32->begin();
    }
    dht = dht32;
  }
  else if(port==33)
  {
    if(!initDHT33)
    {    
      if(type!=DHT11)
        portFix(port);  
      initDHT33=true;
      dht33 = new DHT_Unified(33, type);
      dht33->begin();
    }
    dht = dht33;
  }
  
}

void SensorDHT::portFix(uint8_t port)
{
  pinMode(port, OUTPUT);
  digitalWrite(port, LOW);
  delay(20);
  pinMode(port, INPUT);
}

void SensorDHT::preCycle(int cycleId)
{
}

Data* SensorDHT::read(bool shouldPostData)
{  
  if(!isResetting)
  {
    sensors_event_t event;

    if(_calculation->getValueType()=="humidity")
    {
      dht->humidity().getEvent(&event);

        if (isnan(event.relative_humidity)) {
          Serial.println("NAN Humidity!");
        }
        else {
          shouldPostData = smartSensorCheck(event.relative_humidity, _smartValueThreshold, shouldPostData);
          return _calculation->calculate(this, event.relative_humidity, shouldPostData);
        }
    }
    else if(_calculation->getValueType()=="temperature")
    {
      dht->temperature().getEvent(&event);
       
       if (isnan(event.temperature)) {
          Serial.println("NAN Temperature!");
        }
        else {
          shouldPostData = smartSensorCheck(event.temperature, _smartValueThreshold, shouldPostData);
          return _calculation->calculate(this, event.temperature, shouldPostData);
        }
    }
  }
 
  return NULL;
}

boolean SensorDHT::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
{
  if(powerMode == 3)
  {
    if(!shouldPostData)
    {
      if(!isnan(lastPostedValue))
      {
          if(lastPostedValue-currentRawValue>threshhold || lastPostedValue-currentRawValue<-threshhold)
          {
            Serial.println("Overwrite shouldPostData!");
            shouldPostData = true;
          }
          
      }
    }

    if(shouldPostData)
      lastPostedValue = currentRawValue;
  }

  return shouldPostData;
  
}