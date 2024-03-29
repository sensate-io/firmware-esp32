/**************************************************************************/
/*!
    @file     OTA.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP32 firmware is used to connect ESP32 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v41 - Renamed Display Class to support more types
    v39 - Changed automatic Update to only if required Update
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include "OTA.h"

extern State state;
extern Display* display;

extern char firmwareType[];
extern int currentVersion; 

WiFiClient wifiClient;

void tryFirmwareUpdate(String fwUpdateToken) {

  Serial.println("Trying firmware Update...");
  
  if(display!=NULL)
  {
    display->clear(true);
    display->drawProductLogo();
    display->drawString(10, 5, "Trying Firmware");
    display->drawString(20, 21, "Update...");
  }

  String updatePath = "http://hub.sensate.cloud/v1/bridge/firmware/"+fwUpdateToken+"?version="+String(currentVersion)+"&type="+firmwareType;

  Serial.println(updatePath);
  
  t_httpUpdate_return ret = httpUpdate.update(wifiClient, "hub.sensate.cloud", 80, updatePath); /// FOR ESP32 HTTP FOTA
      switch(ret) {
        case HTTP_UPDATE_FAILED:
            if(display!=NULL)
            {
              display->clear(false); 
              display->drawProductLogo();
            }
            Serial.println("[update] Update failed.");
            break;
        case HTTP_UPDATE_NO_UPDATES:
            if(display!=NULL)
            {
              display->clear(false); 
              display->drawProductLogo();
            }
            Serial.println("[update] Update no Update.");
            break;
        case HTTP_UPDATE_OK:
            if(display!=NULL)
            {
              display->clear(true); 
            }
            Serial.println("[update] Update ok."); // may not called we reboot the ESP
            break;

      }

  if(state==Check_Firmware)
    state = Connected_WiFi;

}
