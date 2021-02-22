/**************************************************************************/
/*!
    @file     DisplayOLED128.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP32 firmware is used to connect ESP32 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v40 - New Display Structure to enable Display Rotation, different Styles etc.
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include "SSD1306Wire.h" 
#include "SH1106Wire.h"
#include "../VisualisationHelper.h"

#ifndef _DisplayOLED128_h_
#define _DisplayOLED128_h_

#define Product_Logo_width 128
#define Product_Logo_height 64
const uint8_t Product_Logo[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 0xE0, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xF0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1C, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3C, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3C, 0x0F, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x07, 0x78, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x9E, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x9E, 0x03, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x03, 0xE0, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x9E, 0x03, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x9E, 0x03, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x07, 0x88, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x04, 0x07, 0x1C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x38, 0x0F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x38, 0x0E, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x3C, 0x30, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 0x0E, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0E, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x38, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0E, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0x1C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x8F, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x8F, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0xC3, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3F, 0xF8, 
  0x3F, 0x70, 0xFF, 0x01, 0xFF, 0x0F, 0xFF, 0x07, 0xFF, 0x0F, 0xFF, 0x0F, 
  0x00, 0xFE, 0x1F, 0xFC, 0x7F, 0xF0, 0xFF, 0x83, 0xFF, 0x0F, 0xFF, 0x0F, 
  0xFF, 0x87, 0xFF, 0x0F, 0x00, 0xFF, 0x1F, 0xFE, 0x7F, 0xF0, 0xF7, 0xC3, 
  0xFF, 0x07, 0xFF, 0x1F, 0xFF, 0xC7, 0xFF, 0x1F, 0x80, 0x07, 0x00, 0x0E, 
  0x70, 0x78, 0xC0, 0xC3, 0x03, 0x00, 0x00, 0x1F, 0x3C, 0xC0, 0x03, 0x1E, 
  0x80, 0x07, 0x00, 0x0F, 0x70, 0x78, 0x80, 0xC3, 0x01, 0x00, 0x00, 0x1E, 
  0x3C, 0xC0, 0x01, 0x1C, 0x00, 0x0F, 0x00, 0x07, 0x78, 0x38, 0xC0, 0xC3, 
  0x03, 0x00, 0x00, 0x1E, 0x1C, 0xC0, 0x01, 0x1E, 0x00, 0xFF, 0x01, 0xFF, 
  0x7F, 0x38, 0xC0, 0xC1, 0x7F, 0x80, 0xFF, 0x0F, 0x1C, 0xE0, 0xFF, 0x0F, 
  0x00, 0xFE, 0x07, 0xFF, 0x7F, 0x38, 0xC0, 0x81, 0xFF, 0xC1, 0xFF, 0x0F, 
  0x1C, 0xE0, 0xFF, 0x0F, 0x00, 0xFC, 0x8F, 0xFF, 0x3F, 0x3C, 0xC0, 0x01, 
  0xFE, 0xC3, 0xFF, 0x0F, 0x1E, 0xE0, 0xFF, 0x0F, 0x00, 0x80, 0x8F, 0x07, 
  0x00, 0x3C, 0xE0, 0x01, 0xE0, 0xE3, 0x01, 0x0F, 0x1E, 0xE0, 0x00, 0x00, 
  0x00, 0x00, 0x8F, 0x07, 0x00, 0x1C, 0xE0, 0x01, 0x80, 0xE7, 0x00, 0x0F, 
  0x0E, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x07, 0x00, 0x1C, 0xE0, 0x00, 
  0xC0, 0xE7, 0x81, 0x07, 0x1E, 0xE0, 0x01, 0x00, 0xC0, 0xFF, 0x87, 0xFF, 
  0x1F, 0x1E, 0xE0, 0xF0, 0xFF, 0xE3, 0xFF, 0x07, 0xFE, 0xE1, 0xFF, 0x03, 
  0xE0, 0xFF, 0x07, 0xFF, 0x1F, 0x1E, 0xE0, 0xF0, 0xFF, 0xC1, 0xFF, 0x07, 
  0xFC, 0xE1, 0xFF, 0x03, 0xE0, 0xFF, 0x03, 0xFE, 0x1F, 0x0E, 0xF0, 0xF0, 
  0xFF, 0xC0, 0x3F, 0x07, 0xF8, 0xC1, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00 };

class Display {
  private:
    OLEDDisplay *display;
    int type;
  public:
    Display (bool, int, String, uint8_t, uint8_t);
    void drawProductLogo();
    void clear(boolean update);
    void drawString(int16_t x, int16_t y, String text);
    void drawArrow();
    void blinkArrow(int count);
    void drawDisconnected(bool update);
    void drawConnected(bool update);
    void flip(int rotation);
    void drawData(unsigned long currentMillis);
    void clearValue(int position);
    void drawValue(int position, String name, String shortName, String value, String unit);
    void drawValue(int position, String name, String shortName, float value, String unit);
    void drawValue(int position, String name, String shortName, bool value, String onString, String offString);
    void drawValueClassic(int position, String name, String shortName, String valueString);
    void drawValueQuad(int position, String name, String shortName, String valueString);
    void clearValueClassic(int position);
    void clearValueQuad(int position);
    int getType();
};

#endif