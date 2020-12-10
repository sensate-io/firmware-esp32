/**************************************************************************/
/*!
    @file     UUID.cpp
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

#include "UUID.h"

String _uuid = "";

String getUUID() {

  if (_uuid.length() != 0)
    return _uuid;
  else
  {
    restoreUUID();
    if (_uuid.length() == 0)
      generateUUID();
  }
  return _uuid;

}

void restoreUUID() {

  digitalWrite(16, HIGH);
  delay(300);
  digitalWrite(16, LOW);
  delay(300);

  int val0 = EEPROM.read(0);
  int val1 = EEPROM.read(1);
  int val2 = EEPROM.read(2);
  int val3 = EEPROM.read(3);

  if (val0 == 10 && val1 == 20 && val2 == 30 && val3 == 40)
  {
    byte uuidNumber[16];
    EEPROM.get(4, uuidNumber);
    _uuid =  ESPTrueRandom.uuidToString(uuidNumber);
  }

}

void generateUUID() {
  Serial.println("Generating new UUID");
  byte uuidNumber[16];
  ESPTrueRandom.uuid(uuidNumber);
  EEPROM.write(0, 10);
  EEPROM.write(1, 20);
  EEPROM.write(2, 30);
  EEPROM.write(3, 40);
  EEPROM.put(4, uuidNumber);
  EEPROM.commit();
  _uuid =  ESPTrueRandom.uuidToString(uuidNumber);
}