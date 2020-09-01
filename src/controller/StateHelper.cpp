/**************************************************************************/
/*!
    @file     StateHelper.cpp
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

#include "StateHelper.h"

bool isWiFiConfigured()
{
  int val20 = EEPROM.read(20);
  int val21 = EEPROM.read(21);
  int val22 = EEPROM.read(22);
  int val23 = EEPROM.read(23);

  if (val20 == 10 && val21 == 20 && val22 == 30 && val23 == 40)
  {
    val20 = EEPROM.read(20);
    val21 = EEPROM.read(21);
    val22 = EEPROM.read(22);
    val23 = EEPROM.read(23);
  }
  if (val20 == 11 && val21 == 21 && val22 == 31 && val23 == 41)
    return true;
  else
    return false;
}