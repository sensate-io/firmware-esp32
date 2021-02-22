/**************************************************************************/
/*!
    @file     firmware-esp32.ino
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP32 firmware is used to connect ESP32 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp32.git

    @section  HISTORY
    v40 - New Display Structure to enable Display Rotation, different Styles etc.
    v39 - Changed automatic Update to only if required Update (skipping versions to be on par with ESP8266)
    v35 - Added Support for VEML6075 and SI1145 UVI Sensors
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include "src/controller/Bridge.h"
#include "src/output/display/DisplayOLED128.h"
#include "src/communication/WiFiManager.h"
#include "src/controller/OTA.h"
#include "src/communication/RestServer.h"
#include "src/output/VisualisationHelper.h"

VisualisationHelper* vHelper;
Display* display = NULL;

int currentVersion = 40; 
boolean printMemory = false;

String board = "Generic";
char firmwareType[] = "ESP32";

// String board = "DevKitC";
// char firmwareType[] = "ESP32-DevKitC";

// String board = "ESP32S";
// char firmwareType[] = "ESP32-S";

// String board = "ESP-WROOM-32";
// char firmwareType[] = "ESP32-WROOM";

// String board = "M5StickC";
// char firmwareType[] = "ESP32-M5StickC";

extern String name = "Bridge";
extern String ucType = "ESP32";

String variant = "SensateV"+String(currentVersion)+board;

String apiVersion = "v1";

int powerMode;
long powerOnDelay = 0;
String powerSavePort;
boolean invPowerSavePort = false;

bool isResetting = false;

unsigned long previousDelayMillis = 0; 
unsigned long previousTickerMillis = 0; 
unsigned long currentMillis;
State state = Boot;

esp_sleep_wakeup_cause_t resetInfo;

extern int displayType;
extern boolean displayEnabled;
extern int displayRotation;
extern int displayMode;

extern uint8_t i2cSDAPort;
extern uint8_t i2cSCLPort;
extern MQTT* mqtt;

#define tickerInterval 250
#define delayInterval 10000

void initSensate();

void setup()
{
  EEPROM.begin(398);

	Serial.begin(9600);
  Serial.println("---------------------------");
  Serial.println(variant);
  Serial.println("---------------------------");
  Serial.println("Startup: ");
  Serial.println(getUUID());

  restoreBridgeConfig();

  vHelper =  new VisualisationHelper();

  doPowerSavingInit(true);

  Serial.println("Display Type:"+String(displayType));
  Serial.println("Display Enabled:"+String(displayEnabled));
  Serial.println("Display Rotation:"+String(displayRotation));
  Serial.println("Display Mode:"+String(displayMode));

  Serial.println("Power Mode:"+String(powerMode));
  Serial.println("Power On Delay:"+String(powerOnDelay));
  Serial.println("Power Save Port:"+String(powerSavePort));
  Serial.println("Power Save Port Inv:"+String(invPowerSavePort));
  

  if(displayType!=0)
  {
    boolean rotateDisplay = (displayRotation == 180);

    display = new Display(rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);

    if(!displayEnabled)
      display->clear(true);
  }

  if(display!=NULL)
    display->drawProductLogo();

  state = Connect_WiFi;
  
  initSensate();
}

void loop()
{
  if(!isResetting)
  {
    //First: Do whatever should be done in every loop!
    runLoop();

    //Then: Do short delayed actions (once every #tickerInterval miliseconds)
    currentMillis = millis();
    if(currentMillis - previousTickerMillis >= tickerInterval)
    {
      previousTickerMillis = currentMillis;
      runTick();
    }

    //Then: Do long delayed actions (once every #delayInterval miliseconds)
    currentMillis = millis();
    if(currentMillis - previousDelayMillis >= delayInterval)
    {
      previousDelayMillis = currentMillis;
      runDelayed();
    }
  }
}

void runLoop() {
  switch(state)
  {
    case Setup:
      loopDNS();
      checkWiFiStatus();
    case Connected_WiFi:
    case Init_Configuration:
    case Operating:
      loopRestserver();
      if(mqtt!=NULL)
        mqtt->loop();
      break;
  }
}

void runTick() {
  // Serial.print("-T-");
  switch(state)
  {
    case Operating:
      loopDisplay(currentMillis);
      loopSensor(currentMillis);
      break;
  }

  if(printMemory)
  {
    Serial.print("HEAP: ");
    Serial.println(ESP.getFreeHeap());
  }
    
  if(ESP.getFreeHeap()<1000)
  {
    Serial.println("RUNNING OUT OF HEAP - RESTART");
    restart();
  }
}

void runDelayed() {
  // Serial.print("----D----");
  if(state!=Operating)
    initSensate();
  else
    checkStatus();
}

void initSensate() {

  if(state==Connect_WiFi)
  {
    Serial.println("STATE: Connect_WiFi");
    connectToNetwork();
  }
  if(state==Check_Firmware)
  {
    switch(resetInfo)
    {
      case ESP_SLEEP_WAKEUP_EXT0 : 
        Serial.println("Skip Check_Firmware (Wakeup caused by external signal using RTC_IO)"); 
        state=Connected_WiFi;
        break;
      case ESP_SLEEP_WAKEUP_EXT1 : 
        Serial.println("Skip Check_Firmware (Wakeup caused by external signal using RTC_CNTL)"); 
        state=Connected_WiFi;
        break;
      case ESP_SLEEP_WAKEUP_TIMER : 
        Serial.println("Skip Check_Firmware (Wakeup caused by timer)"); 
        state=Connected_WiFi;
        break;
      case ESP_SLEEP_WAKEUP_TOUCHPAD : 
        Serial.println("Skip Check_Firmware (Wakeup caused by touchpad)"); 
        state=Connected_WiFi;
        break;
      case ESP_SLEEP_WAKEUP_ULP : 
        Serial.println("Skip Check_Firmware (Wakeup caused by ULP program)"); 
        state=Connected_WiFi;
        break;
      default : 
        Serial.println("STATE: Check_Firmware");
        tryFirmwareUpdate("");
        break;
    }
  }
  if(state==Connected_WiFi)
  {
    Serial.println("STATE: Connected_WiFi");
    startRestServer();
  }
  if(state==Init_Setup)
  {
    Serial.println("STATE: Init_Setup");
    startRestServer();
    startDNS();
  }
  if(state==Register)
  {
    switch(resetInfo)
    {
      case ESP_SLEEP_WAKEUP_EXT0 : 
        Serial.println("Skip Register_Bridge (Wakeup caused by external signal using RTC_IO)"); 
        state=Init_Configuration;
        break;
      case ESP_SLEEP_WAKEUP_EXT1 : 
        Serial.println("Skip Register_Bridge (Wakeup caused by external signal using RTC_CNTL)"); 
        state=Init_Configuration;
        break;
      case ESP_SLEEP_WAKEUP_TIMER : 
        Serial.println("Skip Register_Bridge (Wakeup caused by timer)"); 
        state=Init_Configuration;
        break;
      case ESP_SLEEP_WAKEUP_TOUCHPAD : 
        Serial.println("Skip Register_Bridge (Wakeup caused by touchpad)"); 
        state=Init_Configuration;
        break;
      case ESP_SLEEP_WAKEUP_ULP : 
        Serial.println("Skip Register_Bridge (Wakeup caused by ULP program)"); 
        state=Init_Configuration;
        break;
      default : 
        Serial.println("STATE: Register_Bridge");
        registerBridge();
        break;
    }
  }
  if(state==Init_Configuration)
  {
    Serial.println("STATE: Get_Configuration");
    getBridgeConfig();
  }

}