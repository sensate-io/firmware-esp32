/**************************************************************************/
/*!
    @file     UUID.h
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

#include "thirdparty/ESPTrueRandom.h"
#include <EEPROM.h>

#ifndef _uuid_h_
#define _uuid_h_

String getUUID();
void restoreUUID();
void generateUUID();

#endif