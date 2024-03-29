/**************************************************************************/
/*!
    @file     Bridge.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP32 firmware is used to connect ESP32 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v44 - More Memory Improvements
    v43 - Fixed data transmit issues in configurations with many sensors
    v42 - Fixed low memory issues in configurations with many sensors and a ST7735 Bug
    v41 - Renamed Display Class to support more types
    v40 - New Display Structure to enable Display Rotation, different Styles etc.
    v35 - Added Support for VEML6075 and SI1145 UVI Sensors
    v34 - First Public Release (Feature parity with ESP8266 Release v34)
*/
/**************************************************************************/

#include "Bridge.h"

#define maxSensorCount 25

extern State state;
extern bool isResetting;
extern StaticJsonBuffer<10000> jsonBuffer;
extern String apiVersion;
extern int currentVersion;

extern int powerMode;
extern Display* display;
extern int displayType;
extern boolean displayEnabled;
extern int displayMode;
extern int displayHeight;
extern int displayWidth;
extern int displayRotation;
extern boolean printMemory;

extern String name;
extern String board;
extern String ucType;

extern long powerOnDelay;
extern String powerSavePort;
extern boolean invPowerSavePort;

extern boolean enableMQTT;
extern MQTT* mqtt;

uint8_t i2cSDAPort;
uint8_t i2cSCLPort;

unsigned long nextSensorDue = -1;

char pwdHash[41] = "";

String bridgeURL;

Sensor *sensors[maxSensorCount];
int sensorCount = 0;

bool wasDisconnected = false;
bool serverError = false;

int registerRetry = 0;
int configRetry = 0;
int postSensorDataRetry = 0;
int sensorCycle = 1;

extern VisualisationHelper* vHelper;

int portNumber = 0;
bool foundPorts = false;

String urlString;
String requestDataString;
String payload;

const char* prodCertificate =  //hub.sensate.cloud
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdjCCAl4CCQC/NxFNyZE79DANBgkqhkiG9w0BAQsFADB9MQswCQYDVQQGEwJB\n" \
"VDEPMA0GA1UECAwGU3R5cmlhMRAwDgYDVQQHDAdMaWVib2NoMRAwDgYDVQQKDAdT\n" \
"ZW5zYXRlMRowGAYDVQQDDBFodWIuc2Vuc2F0ZS5jbG91ZDEdMBsGCSqGSIb3DQEJ\n" \
"ARYOc3NsQHNlbnNhdGUuaW8wHhcNMTkwMjEyMTAwNTM2WhcNNDQwMjA2MTAwNTM2\n" \
"WjB9MQswCQYDVQQGEwJBVDEPMA0GA1UECAwGU3R5cmlhMRAwDgYDVQQHDAdMaWVi\n" \
"b2NoMRAwDgYDVQQKDAdTZW5zYXRlMRowGAYDVQQDDBFodWIuc2Vuc2F0ZS5jbG91\n" \
"ZDEdMBsGCSqGSIb3DQEJARYOc3NsQHNlbnNhdGUuaW8wggEiMA0GCSqGSIb3DQEB\n" \
"AQUAA4IBDwAwggEKAoIBAQDI6b3TMr6GEF8ZwvoB7n7osTtaBSmXclgKy43UhlGD\n" \
"JY+tT9U6UmEiyvuEp7Tr2P4qgCPLeOXPyrOCSsTi5dg5AsPHW43SNAOVxeNkI92O\n" \
"EyNVmRrJ1+wEIDe1PZNkwMYKznNEnW/4sTCSE6S2YuMc1vcfipO54Tiae35FcXLQ\n" \
"z3CPn9aFj3TSJJoIoW5af43LqckHG5mIk0KrcsQoWDS7N121ojhfC5F3sanivYCv\n" \
"02DZMrda51flcbj8Y+19SvBSio7ixqQOF1dhmkSGe/BnMNnzPu7kCwqjKu5OocU8\n" \
"Vn2jKv0uVGu+z7eVsSlSFZMUbS5e7BrCzT3X6HZ2S8crAgMBAAEwDQYJKoZIhvcN\n" \
"AQELBQADggEBAIYsG9Pojfut07kn8+DGAD1rcQYV10JbASuUGyjo1FKLGWYOIEUO\n" \
"UlpTnUISkEhVxZw18fYOSzPPmDwNVbKK+MNZisvWDWH+zU+a8GQIeTavHcD9LAMh\n" \
"dRY1JjvSHCyb6ECxVBxHzIAQ7k1wQfzfOsMaI6FegIVobi7wtlBGeQPDRTS07iRE\n" \
"UzZS5kRDRnB7p64De1kO96xksP7mIMstiOPl8e7IybSM4WzYfC2lTlaueHtlk/hY\n" \
"NvtYO2Dq6j00CnZV6GXdgaQFnNA0jO9NNP73LtK16NC4R7ZmZni3vAGYYgxDjA7W\n" \
"6AF7R/S+doCrQtJmcRYgndSEKypzzBmrI+U=\n" \
"-----END CERTIFICATE-----\n";

const char* testCertificate =  //test.sensate.cloud
"-----BEGIN CERTIFICATE-----\n" \
"MIIDgDCCAmgCCQC8slY1nFphgDANBgkqhkiG9w0BAQsFADCBgTELMAkGA1UEBhMC\n" \
"QVQxDzANBgNVBAgMBlN0eXJpYTEQMA4GA1UEBwwHTGllYm9jaDEQMA4GA1UECgwH\n" \
"U2Vuc2F0ZTEbMBkGA1UEAwwSdGVzdC5zZW5zYXRlLmNsb3VkMSAwHgYJKoZIhvcN\n" \
"AQkBFhFtYW51ZWxAc2Vuc2F0ZS5pbzAeFw0xOTAzMTUxMTQ3MDhaFw00NDAzMDgx\n" \
"MTQ3MDhaMIGBMQswCQYDVQQGEwJBVDEPMA0GA1UECAwGU3R5cmlhMRAwDgYDVQQH\n" \
"DAdMaWVib2NoMRAwDgYDVQQKDAdTZW5zYXRlMRswGQYDVQQDDBJ0ZXN0LnNlbnNh\n" \
"dGUuY2xvdWQxIDAeBgkqhkiG9w0BCQEWEW1hbnVlbEBzZW5zYXRlLmlvMIIBIjAN\n" \
"BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAoZIi16mwZKogWseSg/3eoEry2nRW\n" \
"vFzg52ebHxaWNumRhXhvt9EskDn7GtSTD505X2oVeBagJiWAyBZYBP9MdJwyLYMe\n" \
"SyWxozB0pxfuxnTc4uPcgvKghALe3ttjy6aEAaPHP2aiPAdb1K6cfBFJbNJBZEAB\n" \
"50JHS9TdLNlm7Zp5rLe/mOgyH2mhiMu8kHMCU5k3V3Q1qE8wGQWWB3Ji7O6Dqnly\n" \
"tb1hkWf5Gj4ir0lyQOb+V+2qYjdh8k0ipiSAKH5IJ+RPQKTzMJcvEcD+sXMzaGK+\n" \
"IVMCn7RXqkGwOdKaeLuBprgG34EfucPC5r9IHTbu+m6ExLccqhhhr8/ybwIDAQAB\n" \
"MA0GCSqGSIb3DQEBCwUAA4IBAQBMbPgA5a1aTbMobKxCI6/3n+B8oWL3cLCSBLop\n" \
"uWYO7s/jvfwXDIECbzHDx9+GnCiER9gTvvRh7lWveHPfhyL9caM3Blt8KyQ/Bpul\n" \
"zylxMB2O5hJqLzudTubCeqK9GjRab4MSPf6RjGyzsLeSZ1ZQEDifwaOsHTTBHGOo\n" \
"eGljv8ejTtBXBGbyQaFTS9/+IHH/ZUibcUNYStXjjYvyUCo/FumWkjEiFGbEgqPc\n" \
"SaY5+TtGdCikUpr0nE2j8wr4jXyFp2IuY1NvbcR2or/yOgFaxEjgJfL3ruIoB2Y9\n" \
"mshx5oPkM80P0lIyYSqbX9EEeQmAiQkWR1CJ4n/Kk/ubNW/F\n" \
"-----END CERTIFICATE-----\n";

void registerBridge()
{
  if(display!=NULL)
  {
    display->clear(false);
    display->drawProductLogo();
    display->drawString(0, 10, "Signing in...");
  }
    
  String uuid = getUUID();
  String networkIP = WiFi.localIP().toString();

  if (uuid.length() > 0 && networkIP.length() > 0)
  {
    Serial.println("Registering Bridge " + uuid + " to Bridge located at " + bridgeURL);
    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient httpClient;

      urlString = bridgeURL + "/" + apiVersion + "/bridge/";

      if(urlString.startsWith("https://hub"))
        httpClient.begin(urlString, prodCertificate);
      else if(urlString.startsWith("https://test"))
        httpClient.begin(urlString, testCertificate);
      else
        httpClient.begin(urlString);
        
      httpClient.addHeader("Content-Type", "application/json");
      httpClient.setTimeout(5000);

      String pwdHashString = "";

      if(pwdHash[0]!=0xff)
      {
        pwdHashString = String(pwdHashString);
      }
      
      String message = "{\"uuid\":\"" + uuid + "\",\"networkIP\":\"" + networkIP + "\",\"name\":\"" + name + "\",\"vendor\":\"" + board + "\",\"type\":\"" + ucType + "\",\"firmwareVersion\":" + currentVersion + ",\"secPassword\":\"" + pwdHashString + "\"}";

      int httpCode = httpClient.POST(message);

      if (httpCode == HTTP_CODE_OK)
      {
        payload = httpClient.getString();
        if (payload == uuid)
        {
          registerRetry=0;
          state = Init_Configuration;
          httpClient.end();
          return;
        }          
      }
      else if (httpCode == HTTP_CODE_UPGRADE_REQUIRED)
      {
        httpClient.end();
        restart();
        return;
      }
      else
      {
        registerRetry++;
        Serial.println("Register failed..? - HTTP:" + String(httpCode));
        if(registerRetry>=10)
        {
          if(powerMode==2)
          {
            httpClient.end();
            Serial.println("Registering not possible, going back to Deep Sleep for 5 minutes.");
            trySleep(300000000);
            return;
          }
          else if (registerRetry>=25)
          {
            httpClient.end();
            restart();
            return;
          }
        }
      }
      
      httpClient.end();
      return;
    }
    else
    {
      Serial.println(WiFi.status());
      Serial.println("Register failed..? - WIFI:" + WiFi.status());
      Serial.println("Trying to reconnect... " + WiFi.status());
      WiFi.reconnect();
      int conRes = WiFi.waitForConnectResult();
    }
    
  }
  else
  {
    Serial.println("Failed to register Bridge");
  }
}

void restoreBridgeConfig() {
  
  int val20 = EEPROM.read(20);
  int val21 = EEPROM.read(21);
  int val22 = EEPROM.read(22);
  int val23 = EEPROM.read(23);

  if (val20 == 11 && val21 == 21 && val22 == 31 && val23 == 41)
  {
    char urlChars[100];
    EEPROM.get(121, urlChars);
    bridgeURL = String(urlChars);
    Serial.println("Registered Bridge URL: " + bridgeURL);
  }

  int val221 = EEPROM.read(221);
  int val222 = EEPROM.read(222);

  if (val221 == 221 && val222 == 222)
  {
    char nameChars[25];
    EEPROM.get(223, nameChars);
    String tmpName = String(nameChars);
    if(!tmpName.equals(("")))
        name = tmpName;
      
    EEPROM.get(248, pwdHash);
  }

  int val290 = EEPROM.read(290);
  int val291 = EEPROM.read(291);

  // Display Config

  if (val290 == 90 && val291 == 91)
  {
    Serial.println("Restore Display Mode!");
    displayType = EEPROM.read(292);
    
    int dE = EEPROM.read(293);
    if(dE==0)
      displayEnabled = false;
    else
      displayEnabled = true;

    int dM = EEPROM.read(306);
    if(dM==1)
      displayMode=1;
    else
      displayMode=0;  

    int dR = EEPROM.read(307);
    if(dR==180)
      displayRotation = 180;
    else
      displayRotation = 0;
  }
  else
  {
    displayType = 1;
    displayEnabled = true;
  }

  // Power Config

  int val294 = EEPROM.read(294);
  int val295 = EEPROM.read(295);

  if (val294 == 94 && val295 == 95)
  {
    Serial.println("Restore Power Mode!");

    powerMode = EEPROM.read(296);

    char pspChars[4];
    EEPROM.get(297, pspChars);
    powerSavePort = String(pspChars);
    EEPROM.get(301, powerOnDelay);

    int iPSP = EEPROM.read(305);

    if(iPSP==0)
      invPowerSavePort = false;
    else
      invPowerSavePort = true;
  }
  else
  {
    powerMode = 0;
    powerSavePort = "";
    powerOnDelay = 0;
    invPowerSavePort = false;
  }

  int val308 = EEPROM.read(308);
  int val309 = EEPROM.read(309);

  if (val308 == 108 && val309 == 109)
  {
    Serial.println("Restore I2C And Display Size");

    i2cSDAPort = EEPROM.read(310);
    i2cSCLPort = EEPROM.read(311);

    displayWidth = EEPROM.read(312);
    displayHeight = EEPROM.read(313);
  }
  else
  {
    i2cSDAPort = 4;
    i2cSCLPort = 5;
    displayWidth = 128;
    displayHeight = 64;
  }

  Serial.println("I2C and Display Config: "+String(i2cSDAPort)+"/"+String(i2cSCLPort)+" - "+String(displayWidth)+"x"+String(displayHeight)+" ");
}

void restart() {

  Serial.println("restarting..");

  isResetting = true;

  shutDownWiFi();

  EEPROM.end();
  Serial.end();

  delay(500);

  ESP.restart();
}

void getBridgeConfig() {

  Serial.println("Getting Bridge Config from " + bridgeURL);

  HTTPClient httpClient;

  if(display!=NULL)
  {
    display->clear(false);
    display->drawProductLogo();
    display->drawString(0, 10, "Waiting for config...");
  }

  urlString = bridgeURL + "/" + apiVersion + "/bridge/" + getUUID();

  if(urlString.startsWith("https://hub"))
    httpClient.begin(urlString, prodCertificate);
  else if(urlString.startsWith("https://test"))
    httpClient.begin(urlString, testCertificate);
  else
    httpClient.begin(urlString);

  httpClient.addHeader("Content-Type", "application/json");
    
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK)
  {
    payload = httpClient.getString();
    httpClient.end();

    portNumber = 0;
    foundPorts = false;

    configRetry=0;

    if (payload != NULL && payload != "")
    {
      JsonObject& bridgeConfig = jsonBuffer.parseObject(payload);

      if(bridgeConfig.containsKey("mv"))
      {
        int minimalVersion = bridgeConfig["mv"];
        if(currentVersion < minimalVersion)
        {
          Serial.println("Firmware Version is "+String(currentVersion)+" but required: "+String(minimalVersion));
          String fwUpdateToken = bridgeConfig["ut"];
          if(fwUpdateToken!=NULL)
          {
            tryFirmwareUpdate(fwUpdateToken);
          }
          else
          {
            Serial.println("MISSING FW Update Token, canceling update!");
          }
        }
      }

      configureBridge(bridgeConfig);
      yield();

      if(bridgeConfig.containsKey("p"))
      {
        JsonArray& portConfigArray = bridgeConfig["p"];
        handlePortConfigArray(portConfigArray);
        yield();
      }

      if(bridgeConfig.containsKey("ppc"))
      {
    	  int portPageCount = bridgeConfig["ppc"];
		  Serial.println("Config contains "+String(portPageCount)+ " port pages");

		  for(int i=1;i<portPageCount;i++) {

				Serial.println("Fetching port page "+String(i));

				urlString = bridgeURL + "/" + apiVersion + "/bridge/" + getUUID() + "/" + String(i);

				if(urlString.startsWith("https://hub"))
					httpClient.begin(urlString, prodCertificate);
				else if(urlString.startsWith("https://test"))
					httpClient.begin(urlString, testCertificate);
				else
					httpClient.begin(urlString);

				httpClient.addHeader("Content-Type", "application/json");

				Serial.print("g");
				int pageHttpCode = httpClient.GET();

				if (pageHttpCode == HTTP_CODE_OK)
				{
					Serial.print("o");

					payload = httpClient.getString();
					httpClient.end();

					if (payload != NULL && payload != "")
					{
						JsonObject& bridgePortConfig = jsonBuffer.parseObject(payload);
						if(bridgePortConfig.containsKey("p"))
						{
							JsonArray& pagedPortConfigArray = bridgePortConfig["p"];
							handlePortConfigArray(pagedPortConfigArray);
						}

					}
				}
				else
					httpClient.end();

				yield();
			}

      }

      if(display!=NULL)
      {
        display->clear(false);
	    display->drawProductLogo();
	    display->drawString(0, 10, "Waiting for sensors...");
	    initVisualisationHelper(bridgeConfig);
	  }

      jsonBuffer.clear();

      httpClient.end();

      if(foundPorts)
        state = Operating;

      return;
    }
  }

  httpClient.end();

  configRetry++;
  if(configRetry>=10 && powerMode==2)
  {
    Serial.println("Fetching configuration not possible, going back to Deep Sleep for 5 minutes.");
    trySleep(300000000);
  }

}

void configureBridge(JsonObject& bridgeConfig) {

  powerMode = bridgeConfig["pm"];
  displayType = bridgeConfig["dt"];

  powerOnDelay =  bridgeConfig["pod"];
  powerSavePort = bridgeConfig["psp"].asString();
  invPowerSavePort = bridgeConfig["ipsp"];

  uint8_t oldSCL = i2cSCLPort;
  uint8_t oldSDA = i2cSDAPort;

  i2cSCLPort = bridgeConfig["i2c"];
  i2cSDAPort = bridgeConfig["i2d"];

  boolean updateRotation = false;

  int oldDisplayWidth = displayWidth;
  int oldDisplayHeight = displayHeight;

  if(displayType!=0)
  {
    displayMode = bridgeConfig["dm"];
    displayEnabled = bridgeConfig["de"];

    displayHeight = bridgeConfig["dh"];
    displayWidth = bridgeConfig["dw"];

    int newRotation = bridgeConfig["dr"];

    enableMQTT = bridgeConfig["me"];

    if(displayRotation!=newRotation)
    {
        displayRotation = newRotation;
        updateRotation = true;
    }

  }
  else
  {
    displayEnabled = false;
  }

  boolean requireInitI2C = true;

  if(((displayType==1 || displayType==2)) && (((oldSCL != i2cSCLPort) || (oldSDA != i2cSDAPort) || (oldDisplayWidth != displayWidth) || (oldDisplayHeight != displayHeight)) || ((display!=NULL) && (display->getType()!=displayType))))
  {
    Serial.println("CREATE TEMP DISPLAY FOR NEW CONFIG");
    boolean rotateDisplay = (displayRotation == 180);

    switch(displayType)
	{
		case 3:
			display = new DisplayST7735(rotateDisplay, displayType);
			break;
		default:	// Fallback to OLED init
			display = new DisplayOLED128(displayWidth, displayHeight, rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);
			requireInitI2C=false;
			break;
	}
  }
  else
  {
    if(display!=NULL && !displayEnabled)
    {
      display->clear(true);
    }
    else if(display==NULL && displayType!=0)
    {
		Serial.println("CREATE TEMP DISPLAY!");
		boolean rotateDisplay = (displayRotation == 180);
		switch(displayType)
		{
			case 3:
				display = new DisplayST7735(rotateDisplay, displayType);
				break;
			default:	// Fallback to OLED init
				display = new DisplayOLED128(displayWidth, displayHeight, rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);
				requireInitI2C=false;
				break;
		}
    }
    else if(updateRotation && display!=NULL)
    {
      Serial.println("FLIP!");
      display->flip(displayRotation);
    }
  }

  if(requireInitI2C && (i2cSCLPort != i2cSDAPort))
  {
  	Serial.println("Init I2C Bus: SDA-"+String(i2cSDAPort)+", SCL-"+String(i2cSCLPort));
  	Wire.begin(i2cSDAPort, i2cSCLPort);
  }


  tryInitMQTT();
  
  storeDisplayAndPowerConfig(true);

  doPowerSavingInit(false);
}

void initVisualisationHelper(JsonObject& bridgeConfig) {

  unsigned long displayCycleInterval = bridgeConfig["di"];
  
  if(displayCycleInterval==0)
  {
    Serial.println("Display cycle disabled (Interval = 0)");
  }
  else
  {
    vHelper->enableDisplayCycle(millis(), display->getSimultanValueCount(), displayCycleInterval);
  }
  

}

void storeDisplayAndPowerConfig(boolean withPowerSettings) {

// Display Config
  EEPROM.write(290, 90);
  EEPROM.write(291, 91);

  EEPROM.write(292, displayType);

  if(displayEnabled)
    EEPROM.write(293, 1);
  else
    EEPROM.write(293, 0);

  EEPROM.write(306, displayMode);
  EEPROM.write(307, displayRotation);

// Power Config

  if(withPowerSettings)
  {
    EEPROM.write(294, 94);
    EEPROM.write(295, 95);

    EEPROM.write(296, powerMode);

    char pspChars[4];
    powerSavePort.toCharArray(pspChars, 4);
    
    EEPROM.put(297, pspChars);
    EEPROM.put(301, powerOnDelay);

    if(invPowerSavePort)
      EEPROM.write(305, 1);
    else
      EEPROM.write(305, 0);
  }

  //I2C Config and More

  EEPROM.write(308, 108);
  EEPROM.write(309, 109);

  EEPROM.write(310, i2cSDAPort);
  EEPROM.write(311, i2cSCLPort);
  EEPROM.write(312, displayWidth);
  EEPROM.write(313, displayHeight);

  EEPROM.commit();
  yield();
}

void tryInitMQTT() {

  if(!enableMQTT)
    return;
  
  Serial.println("Trying to init MQTT client...");

  int val314 = EEPROM.read(314);
  int val315 = EEPROM.read(315);

  if(val314 == 114 && val315 == 115)
  {
    char brokerUrl[36];
    EEPROM.get(316, brokerUrl);
    long brokerPort;
    EEPROM.get(352, brokerPort);

    int val356 = EEPROM.read(356);
    int val357 = EEPROM.read(357);

    if(val356 == 156 && val357 == 157)
    {
      Serial.println("Starting Authenticated MQTT Broker connection...");

      char username[20];
      EEPROM.get(358, username);
      char password[20];
      EEPROM.get(378, password);

      mqtt = new MQTT(brokerUrl, brokerPort, String(username), String(password));
    }
    else
    {
      Serial.println(val356);
      Serial.println(val357);
      Serial.println("Starting Anonymous MQTT Broker connection...");
      mqtt = new MQTT(brokerUrl, brokerPort);
    }

    mqtt->connect();
  }
  else
  {
    Serial.println("No MQTT Broker configured.");
  }

  yield();

}

void handlePortConfigArray(JsonArray& portConfigArray) {

	for (JsonObject& configEntry : portConfigArray) {

		int portRow = portNumber;

		if (configEntry.containsKey("sod") && !configEntry["sod"])
		{
			portRow = -1;
		}
		else
		{
			portNumber++;
			configureDisplayValueData(portRow, configEntry);
		}

		if (configEntry.containsKey("et"))
			configureExpansionPort(portRow, configEntry);
		else
			configurePort(portRow, configEntry);

		foundPorts = true;
		yield();
	}

	if(printMemory)
	{
		Serial.print("HEAP: ");
		Serial.println(ESP.getFreeHeap());
	}

}


void configureDisplayValueData(int portNumber, JsonObject& portConfig) {

  vHelper->getDisplayDataModel()->setData(portNumber, new DisplayValueData(portNumber, portConfig["n"], portConfig["sn"], "WAIT"));

}


void configureExpansionPort(int portNumber, JsonObject& portConfig) {
  Serial.print("Configure Expansion Port: ");

  //portConfig.prettyPrintTo(Serial);
  
  SensorCalculation* calc = NULL;

  if (portConfig["s"]["cf"] == "APPROX_QUAD")
    calc = new SensorCalculationApproxQuad(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portConfig["s"]["cv3"], portConfig["s"]["cv4"], portNumber);
  else if (portConfig["s"]["cf"] == "PT1001000")
    calc = new SensorCalculationPT1001000(portConfig["s"]["cv1"], portNumber);
  else if (portConfig["s"]["cf"] == "DUAL_NTC")
    calc = new SensorCalculationDualNtc(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KELVIN")
    calc = new SensorCalculationDirectKelvin(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_CELSIUS")
    calc = new SensorCalculationDirectCelsius(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_PERCENT")
    calc = new SensorCalculationDirectPercent(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_HEKTOPASCAL")
    calc = new SensorCalculationDirectHektoPascal(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_METER")
    calc = new SensorCalculationDirectMeter(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUX")
    calc = new SensorCalculationDirectLux(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUMEN")
    calc = new SensorCalculationDirectLumen(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_OHM")
    calc = new SensorCalculationDirectOhm(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KOHM")
    calc = new SensorCalculationDirectKOhm(portNumber);  
  else if (portConfig["s"]["cf"] == "DIRECT_PPM")
    calc = new SensorCalculationDirectPPM(portNumber);  
  else if (portConfig["s"]["cf"] == "DIRECT_NONE")
    calc = new SensorCalculationDirectNone(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_WPM2")
    calc = new SensorCalculationDirectWpm2(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_METER")
    calc = new SensorCalculationCalcAltitude(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_RAW_PERCENT")
    calc = new SensorCalculationRawToPercent(portConfig["c1"], portConfig["c2"], portNumber);
  else if (portConfig["s"]["cf"] == "RAW")
    calc = new SensorCalculationRaw(portNumber);
  else if (portConfig["s"]["cf"] == "RAW_A")
    calc = new SensorCalculationRaw(portNumber, "a");
  else if (portConfig["s"]["cf"] == "RAW_B")
    calc = new SensorCalculationRaw(portNumber, "b");
  else if (portConfig["s"]["cf"] == "RAW_C")
    calc = new SensorCalculationRaw(portNumber, "c");
  else if (portConfig["s"]["cf"] == "CALC_RAW_VREF")
    calc = new SensorCalculationRawToVoltage(portConfig["c1"], portConfig["c2"], portNumber);

  if(calc==NULL)
  {
    Serial.println("Unsupported Calculation Formula, please check Firmware Version.");
    return;
  }

  int refreshInterval = portConfig["ri"];
  if (refreshInterval == 0)
  {
    Serial.println("No Sensor Refresh Update interval set, falling back to default (10sec).");
    refreshInterval = 10000;
  }

  int postDataInterval = portConfig["pdi"];
  if (postDataInterval == 0)
  {
    Serial.println("No Sensor Post Data Update interval set, falling back to default (60sec).");
    postDataInterval = 60000;
  }

  Serial.print(portConfig["n"].asString());
  Serial.print(" of type: ");
  Serial.println(portConfig["et"].asString());

  if (portConfig["et"] == "ADS1015" || portConfig["et"] == "ADS1115")
  {
    addSensor(new Ads1x15 (portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["et"], portConfig["ec1"], portConfig["ch"], portConfig["c1"], portConfig["c2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "DALLAS" || portConfig["et"] == "DS18B20")
  {
    uint8_t port = translateGPIOPort(portConfig["ec1"]);
    addSensor(new SensorDallas(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], port, portConfig["ch"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "DHT11" || portConfig["et"] == "DHT21" || portConfig["et"] == "DHT22")
  {
    uint8_t port = translateGPIOPort(portConfig["ec1"]);
    if(port<99)
    {
      addSensor(new SensorDHT(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["et"], port, refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
    }
  }
  else if (portConfig["et"] == "BME280" || portConfig["et"] == "BMP280")
  {    
    addSensor(new SensorBMx280(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "BME680")
  {    
    addSensor(new SensorBME680(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "MAX44009")
  {    
    addSensor(new SensorMax44009(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "BH1750")
  {    
    addSensor(new SensorBH1750(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "VEML6075")
  {    
    addSensor(new SensorVEML6075(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "SI1145")
  {    
    addSensor(new SensorSI1145(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }

}

void configurePort(int portNumber, JsonObject& portConfig) {
  
  String port = portConfig["p"];
  
  Serial.print("Configure Onboard Port:");
  Serial.println(port);

  //portConfig.prettyPrintTo(Serial);
  Serial.println("");

  SensorCalculation* calc = NULL;

  if (portConfig["s"]["cf"] == "APPROX_QUAD")
    calc = new SensorCalculationApproxQuad(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portConfig["s"]["cv3"], portConfig["s"]["cv4"], portNumber);
  else if (portConfig["s"]["cf"] == "PT1001000")
    calc = new SensorCalculationPT1001000(portConfig["s"]["cv1"], portNumber);
  else if (portConfig["s"]["cf"] == "DUAL_NTC")
    calc = new SensorCalculationDualNtc(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KELVIN")
    calc = new SensorCalculationDirectKelvin(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_CELSIUS")
    calc = new SensorCalculationDirectCelsius(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_PERCENT")
    calc = new SensorCalculationDirectPercent(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_HEKTOPASCAL")
    calc = new SensorCalculationDirectHektoPascal(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_METER")
    calc = new SensorCalculationDirectMeter(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUX")
    calc = new SensorCalculationDirectLux(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUMEN")
    calc = new SensorCalculationDirectLumen(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_OHM")
    calc = new SensorCalculationDirectOhm(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KOHM")
    calc = new SensorCalculationDirectKOhm(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_PPM")
    calc = new SensorCalculationDirectPPM(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_NONE")
    calc = new SensorCalculationDirectNone(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_WPM2")
    calc = new SensorCalculationDirectWpm2(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_METER")
    calc = new SensorCalculationCalcAltitude(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_RAW_PERCENT")
    calc = new SensorCalculationRawToPercent(portConfig["c1"], portConfig["c2"], portNumber);
  else if (portConfig["s"]["cf"] == "RAW")
    calc = new SensorCalculationRaw(portNumber);
  else if (portConfig["s"]["cf"] == "RAW_A")
    calc = new SensorCalculationRaw(portNumber, "a");
  else if (portConfig["s"]["cf"] == "RAW_B")
    calc = new SensorCalculationRaw(portNumber, "b");
  else if (portConfig["s"]["cf"] == "RAW_C")
    calc = new SensorCalculationRaw(portNumber, "c");
  else if (portConfig["s"]["cf"] == "CALC_RAW_VREF")
    calc = new SensorCalculationRawToVoltage(portConfig["c1"], portConfig["c2"], portNumber);
    
  if(calc==NULL)
  {
    Serial.println("Unsupported Calculation Formula, please check Firmware Version.");
    return;
  }

  int refreshInterval = portConfig["ri"];
  if (refreshInterval == 0)
  {
    Serial.println("No Sensor Refresh Update interval set, falling back to default (10sec).");
    refreshInterval = 10000;
  }

  int postDataInterval = portConfig["pdi"];
  if (postDataInterval == 0)
  {
    Serial.println("No Sensor Post Data Update interval set, falling back to default (60sec).");
    postDataInterval = 60000;
  }

  uint8_t aPort = translateAnalogueGPIOPort(port);

  if (aPort<99)
  {
    if(portConfig["c1"]!=0 && portConfig["c2"]==0)
    {
      addSensor(new SensorAnalogue (portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["c1"], aPort, refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
    }
    else
    {
      addSensor(new SensorAnalogue (portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], 0, aPort, refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
    }
  }
  else
  {
    uint8_t intPort = translateGPIOPort(port);
    if(intPort<99)
    {
    	Serial.print("Setting up Digital Switch at Port: ");
    	Serial.println(port);
    	addSensor(new SensorDigitalSwitch(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], intPort, refreshInterval, postDataInterval, calc));
    }
    else
      Serial.println("Invalid Port specified? check config! " + port);
  }
}

void addSensor(Sensor *sensor)
{
  if (sensorCount <= maxSensorCount)
  {
    sensors[sensorCount] = sensor;
    sensorCount++;
  }
}

void loopSensor(int currentTimeMs) {

  Data *data[maxSensorCount];
  int dataCount = 0;

  if(wasDisconnected)
  {
    if(WiFi.status() == WL_CONNECTED)
    {
      if(display!=NULL)
        display->drawConnected(true);
      wasDisconnected = false;
    }
  }

  for (int i = 0; i < sensorCount; i++)
  {
    Data* newData = sensors[i]->trySensorRead(currentTimeMs, sensorCycle);
    yield();
    if (newData != NULL)
    {
       data[dataCount] = newData;
       dataCount++;
    }
    yield();
  }

  sensorCycle++;
  if(sensorCycle==1000)
    sensorCycle = 10; //Cycles 1->9 only run once at startup!

  boolean submitSuccess = false;
  int retry = 1;

  while(retry < 10 && !submitSuccess && dataCount>0)
  {
    boolean abortDelay = false;
    yield();

    if(WiFi.status() == WL_CONNECTED)
    {
      submitSuccess = postSensorData(data, dataCount);
      abortDelay = submitSuccess;
      yield();
      if(mqtt!=NULL)
      {
        mqtt->publishSensorData(data, dataCount);
      }
    }
    else
    {
      wasDisconnected = true;
      if(display!=NULL)
        display->drawDisconnected(true);

      if(WiFi.status()==WL_DISCONNECTED)
      {
        Serial.println("Not Connected - Trying to reconnect to WiFi..");
        WiFi.reconnect();
        int connRes = WiFi.waitForConnectResult();
          if(connRes==WL_CONNECTED)
          {
            Serial.println("Reconnect success!");
            abortDelay = true;
          }
      } 
      yield();
    }

    if(!abortDelay)
    {
      int delayTime=100*retry;  
      Serial.print("Communication Issues - Retrying in ");
      Serial.print(delayTime);
      Serial.println("ms ");
      yield();
      delay(delayTime);
    }
    retry++;
  }

// If dataCount = 0 this is only a display refresh cycle - the disconnected symbol has to be drawn again, otherwise it looks like the device has a connection
  if(dataCount==0)
  {
      if((serverError || (WiFi.status()!=WL_CONNECTED)) && display!=NULL)
      {
        wasDisconnected=true;
        display->drawDisconnected(true);
      }
        
      yield();
  }
  else  // dealloc the data array from memory
  {
    for (int i = 0; i < dataCount; i++)
      {
        delete data[i];
        data[i] = NULL;
      }
  }

  long time = nextSensorDue-millis();

  if(time>10000)
  {
    trySleep((time-3000)*1000);
  }
}

void loopDisplay(unsigned long currentTimeMs) {
  if(display!=NULL)
  {
    display->drawData(currentTimeMs);
  }
}

void trySleep(long microseconds)
{
    // Serial.println("Power Mode... "+String(powerMode));

    if(powerMode==2 && microseconds>0)
    {
      loopDisplay(millis());
      doPowerSaving();
      Serial.println("Going to deep sleep for "+String(microseconds));
      ESP.deepSleep(microseconds);
    }
}

void doPowerSaving() {

    if((powerSavePort!=NULL) && (!(powerSavePort.length()>0)))
    {
    	Serial.print("Switching Power Save Port: ");
		Serial.println(powerSavePort);

        int port = translateGPIOPort(powerSavePort);

        if(port>=0)
        {
          pinMode(port, OUTPUT);
          digitalWrite(port, invPowerSavePort);
        }

    }

    yield();
}

void doPowerSavingInit(boolean doDelay) {

  if(powerSavePort!="")
  {
	Serial.print("Init Power Save Port: ");
	Serial.println(powerSavePort);

    int port = translateGPIOPort(powerSavePort);

    if(port>=0)
    {
      pinMode(port, OUTPUT);
      digitalWrite(port, !invPowerSavePort);
    }
    else
    {
    	Serial.print("No valid Power Save Port defined: ");
    	Serial.println(powerSavePort);
    }
  }
  else
  {
    Serial.println("No Power Save Port defined!");
  }

  yield();

  if(doDelay && powerOnDelay>0)
  {
    Serial.println("Delaying Power on for: "+String(powerOnDelay));
    delay(powerOnDelay);
  }

}

uint8_t translateGPIOPort(String gpioPort)
{
  if(gpioPort=="0")
    return 0;
  if(gpioPort=="1")
    return 1;
  if(gpioPort=="2")
    return 2;
  if(gpioPort=="3")
    return 3;
  if(gpioPort=="4")
    return 4;
  if(gpioPort=="5")
    return 5;
  if(gpioPort=="12")
    return 12;
  if(gpioPort=="13")
    return 13;
  if(gpioPort=="14")
    return 14;
  if(gpioPort=="15")
    return 15;
  if(gpioPort=="16")
    return 16;  
  if(gpioPort=="17")
    return 17;  
  if(gpioPort=="18")
    return 18;  
  if(gpioPort=="19")
    return 19;  
  if(gpioPort=="21")
    return 21;  
  if(gpioPort=="22")
    return 22;  
  if(gpioPort=="23")
    return 23;  
  if(gpioPort=="25")
    return 25;  
  if(gpioPort=="26")
    return 26;  
  if(gpioPort=="27")
    return 27;  
  if(gpioPort=="32")
    return 32;  
  if(gpioPort=="33")
    return 33;  		

  return 99;
}

uint8_t translateAnalogueGPIOPort(String gpioPort)
{
  if(gpioPort=="A4")
    return 4;  
  if(gpioPort=="A0")
    return 0;  
  if(gpioPort=="A2")
    return 2;  
  if(gpioPort=="A15")
    return 15;  
  if(gpioPort=="A12")
    return 12;  
  if(gpioPort=="A13")
    return 13;  
  if(gpioPort=="A14")
    return 14;  
  if(gpioPort=="A27")
    return 27;  
  if(gpioPort=="A25")
    return 25;  
  if(gpioPort=="A26")
    return 26;  
  if(gpioPort=="A32")
    return 32;  
  if(gpioPort=="A33")
    return 33;  
  if(gpioPort=="A34")
    return 34;  
  if(gpioPort=="A35")
    return 35;  
  if(gpioPort=="A36")
    return 36;  
  if(gpioPort=="A39")
    return 39;  

  return 99;
}

boolean postSensorData(Data* data[], int dataCount)
{
	if(printMemory)
	{
		Serial.print("HEAP: ");
		Serial.println(ESP.getFreeHeap());
	}

	boolean success = true;

	if(dataCount<=5)
	{
		success = postSensorDataPart(data, 0, dataCount-1);
	}
	else
	{
		Serial.print("s");
		Serial.print(dataCount);

		int start = 0;
		int end = 4;

		while(end<=dataCount)
		{
			if(!postSensorDataPart(data, start, end))
				success = false;

			yield();

			if(printMemory)
			{
				Serial.print("HEAP: ");
				Serial.println(ESP.getFreeHeap());
			}

			start+=5;
			end+=5;

			if(start>dataCount-1)
				break;
			if(end>dataCount-1)
				end = dataCount-1;
		}
	}

	if(!success)
	{
		postSensorDataRetry++;

		if(postSensorDataRetry>=25)
		{
		  postSensorDataRetry=0;
		  restart();
		}

		Serial.print("Retry #");
		Serial.print(postSensorDataRetry);
		Serial.println(", restart at 25");
	}

	return success;
}


boolean postSensorDataPart(Data* data[], int startIndex, int endIndex)
{
  HTTPClient httpClient;

  requestDataString = "";

  for (int i = startIndex; i <= endIndex; i++)
  {
	  if (i > startIndex)
	  {
		requestDataString += ",";
	  }
	  requestDataString += data[i]->getRequestString();
  }

  urlString = "";
  urlString += bridgeURL;
  urlString += "/";
  urlString += apiVersion ;
  urlString += "/data/";
  urlString += getUUID();
  urlString += "/";
  urlString += requestDataString;

  if(urlString.startsWith("https://hub"))
    httpClient.begin(urlString, prodCertificate);
  else if(urlString.startsWith("https://test"))
    httpClient.begin(urlString, testCertificate);
  else
    httpClient.begin(urlString);

  Serial.print("p");
  
  int httpCode = httpClient.PUT("");

  if (httpCode == HTTP_CODE_UPGRADE_REQUIRED)
  {
    Serial.print("u");

    httpClient.end();
    restart();
    return true;
  }
  else if (httpCode == HTTP_CODE_OK)
  {
    Serial.print("o");

    if(serverError || wasDisconnected)
    {
      postSensorDataRetry=0;
      serverError = false;
      wasDisconnected = false;
      if(display!=NULL)
        display->drawConnected(true);
    }

    payload = httpClient.getString();
    httpClient.end();

    if(payload!=NULL && displayType!=0)
    {
      if(payload.equals("ed") && displayEnabled==false)
      {
        Serial.println("Enabling display!");
        displayEnabled=true;
        boolean rotateDisplay = (displayRotation == 180);
        if(display==NULL)
        {
			switch(displayType)
			{
				case 3:
					display = new DisplayST7735(rotateDisplay, displayType);
					break;
				default:	// Fallback to OLED init
					display = new DisplayOLED128(displayWidth, displayHeight, rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);
					break;
			}
        }
        storeDisplayAndPowerConfig(false);
      }
      else if(payload.equals("dd") && displayEnabled==true)
      {
        Serial.println("Disabling display!");
        displayEnabled=false;
        if(display!=NULL)
          display->clear(true);
        storeDisplayAndPowerConfig(false);
      }
    }

    return true;
  }
  else
  {
    Serial.print("e");

    if(display!=NULL)
    {
      wasDisconnected=true;
      display->drawDisconnected(true);
    }
      
    serverError = true;

    Serial.print("\nServer Error: ");
	Serial.print(httpCode);
	Serial.print(" - ");
	Serial.print(httpClient.errorToString(httpCode));
	Serial.print(" - WIFI: ");
    Serial.println(WiFi.status());

    httpClient.end();

  }
  return false;
}

void checkStatus() 
{
  if(WiFi.status() != WL_CONNECTED)
  {
    wasDisconnected=true;
    if(display!=NULL)
      display->drawDisconnected(true);
    Serial.println("Not Connected");
  }
}
