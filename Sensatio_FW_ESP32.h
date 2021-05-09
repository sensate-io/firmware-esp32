/**************************************************************************/
/*!
    @file     Sensatio_FW_ESP32.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP32 firmware is used to connect ESP32 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp32.git

    @section  HISTORY
    v41 - Changed IDE, Sensatio, Renamed Display Class to support more types
*/
/**************************************************************************/

// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Sensatio_FW_ESP32_H_
#define _Sensatio_FW_ESP32_H_
#include "Arduino.h"

//add your includes for the project Sensatio_FW_ESP32 here
#include "src/controller/Bridge.h"
#include "src/output/display/Display.h"
#include "src/communication/WiFiManager.h"
#include "src/controller/OTA.h"
#include "src/communication/RestServer.h"
#include "src/output/VisualisationHelper.h"
//end of add your includes here

//add your function definitions for the project Sensatio_FW_ESP32 here
void setup();
void loop();
void runLoop();
void runTick();
void runDelayed();
void initSensate();

//Do not add code below this line
#endif /* _Sensatio_FW_ESP32_H_ */
