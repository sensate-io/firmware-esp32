/**************************************************************************/
/*!
    @file     WiFiManager.h
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

#include <EEPROM.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <WebServer.h>

#ifndef _WiFiManager_h_
#define _WiFiManager_h_

#include "../output/display/DisplayOLED128.h"
#include "../controller/StateHelper.h"
#include "../controller/Bridge.h"

void connectToNetwork();
void connectToAP();
bool startUpLocalAP();
void setupBridgeConfig();
void resetBridgeConfig();
void setupBridgeConfigManual();
void shutDownWiFi();
void startDNS();
void loopDNS();
void checkWiFiStatus();

#endif