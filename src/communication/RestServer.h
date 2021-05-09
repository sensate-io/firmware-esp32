/**************************************************************************/
/*!
    @file     RestServer.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP32 firmware is used to connect ESP32 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebServer.h>

#ifndef _RestServer_h_
#define _RestServer_h_

#include "../controller/StateHelper.h"
#include "../controller/UUID.h"
#include "../output/display/Display.h"
#include "../communication/WiFiManager.h"
#include "MQTT.h"

void startRestServer();
void loopRestserver();
void configRestServerRouting();
void androidCaptiveResponse();
void captiveResponse();
void tryIdentify();
void restGetUuid();
void handleNotFound();
bool captivePortal();
void handleRoot();
void returnNetworkList();
void presentWiFiSetupScreen();
void tryRestart();
void restUpdateName();
void handleAppleCaptivePortal();
void initMqtt();

String toStringIp(IPAddress ip);
bool isIp(String str);

#endif
