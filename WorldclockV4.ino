/**
 * IotWebConf03CustomParameters.ino -- IotWebConf is an ESP8266/ESP32
 *   non blocking WiFi/AP web configuration library for Arduino.
 *   https://github.com/prampec/IotWebConf 
 *
 * Copyright (C) 2020 Balazs Kelemen <prampec+arduino@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/**
 * Example: Custom parameters
 * Description:
 *   In this example it is shown how to attach your custom parameters
 *   to the config portal. Your parameters will be maintained by 
 *   IotWebConf. This means, they will be loaded from/saved to EEPROM,
 *   and will appear in the config portal.
 *   Note the configSaved and formValidator callbacks!
 *   (See previous examples for more details!)
 * 
 * Hardware setup for this example:
 *   - An LED is attached to LED_BUILTIN pin with setup On=LOW.
 *   - [Optional] A push button is attached to pin D2, the other leg of the
 *     button should be attached to GND.
 */

#include <IotWebConf.h>
#include <IotWebConfUsing.h>  // This loads aliases for easier class names.
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN 2  // D8
#define LED_COUNT 139

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 3600);

/** LED PART **/

int wordDefinitions[26][16] = {
  { 0, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },             //ES          0
  { 3, 4, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },              //IST         1
  { 7, 8, 9, 10, 11, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },             //FÜNF        2
  { 23, 24, 25, 26, 27, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //ZEHN        3
  { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1, -1, -1, -1, -1, -1 },          //ZWANZIG     4
  { 29, 30, 31, 32, 33, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //DREI        5
  { 29, 30, 31, 32, 33, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //DREIVIERTEL 6
  { 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, -1, -1, -1, -1, -1, -1 },          //VIERTEL     7
  { 57, 58, 59, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //VOR         8
  { 51, 52, 53, 54, 55, 56, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //FUNK        9
  { 45, 46, 47, 48, 49, 50, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //NACH        10
  { 61, 62, 63, 64, 65, 66, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //HALB        11
  { 67, 68, 69, 70, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //ELF         12
  { 70, 71, 72, 73, 74, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //FÜNF        13
  { 82, 83, 84, 85, 81, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //EINS        14
  { 75, 76, 77, 78, 79, 80, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //ZWEI        15
  { 86, 87, 88, 89, 90, 91, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //DREI        16
  { 92, 93, 94, 95, 96, 97, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },          //VIER        17
  { 104, 105, 106, 107, 108, 109, 110, -1, -1, -1, -1, -1, -1, -1, -1, -1 },   //SECHS       18
  { 98, 99, 100, 101, 102, 103, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },      //ACHT        19
  { 111, 112, 113, 114, 115, 116, 117, 118, -1, -1, -1, -1, -1, -1, -1, -1 },  //SIEBEN      20
  { 119, 120, 121, 122, 123, 124, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1 },   //ZWÖLF       21
  { 135, 136, 137, 138, 134, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },     //ZEHN        22
  { 134, 130, 131, 132, 133, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },     //NEUN        23
  { 126, 127, 128, 129, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },      //UHR         24
  { 82, 83, 84, 85, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

boolean enabledWords[26] = {
  false, false, false, false, false, false, false, false, false, true,
  false, false, false, false, false, false, false, false, false, false,
  false, false, false, false, false, false
};

int nextUpdateMinute = -1;

/** LED PART END **/

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "testThing";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "smrtTHNG8266";

#define STRING_LEN 128
#define NUMBER_LEN 32

// -- Configuration specific key. The value should be modified if config structure was changed.
#define CONFIG_VERSION "wc1"

// -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
//      password to buld an AP. (E.g. in case of lost password)
#define CONFIG_PIN 5

// -- Status indicator pin.
//      First it will light up (kept LOW), on Wifi connection it will blink,
//      when connected to the Wifi it will turn off (kept HIGH).
//#define STATUS_PIN LED_BUILTIN

// -- Method declarations.
void handleRoot();
// -- Callback methods.
void configSaved();
bool formValidator(iotwebconf::WebRequestWrapper* webRequestWrapper);

DNSServer dnsServer;
WebServer server(80);

/** Config Section **/

char config_r[NUMBER_LEN];  //int
char config_g[NUMBER_LEN];  //int
char config_b[NUMBER_LEN];  //int
char config_w[NUMBER_LEN];  //int

char config_td1[STRING_LEN];  //chooser
static char config_td1_values[][STRING_LEN] = { "0", "1" };
static char config_td1_names[][STRING_LEN] = { "10 vor halb", "20 nach" };
char config_td2[STRING_LEN];  //chooser
static char config_td2_values[][STRING_LEN] = { "0", "1" };
static char config_td2_names[][STRING_LEN] = { "10 nach halb", "20 vor" };
char config_td3[STRING_LEN];  //chooser
static char config_td3_values[][STRING_LEN] = { "0", "1" };
static char config_td3_names[][STRING_LEN] = { "Viertel vor", "Dreiviertel" };
char config_td4[NUMBER_LEN];  //chooser
static char config_td4_values[][STRING_LEN] = { "0", "1" };
static char config_td4_names[][STRING_LEN] = { "Es ist ausblenden", "Es ist anzeigen" };

char config_br[NUMBER_LEN];  //int
char config_to[NUMBER_LEN];  //int
char config_st[STRING_LEN];  //chooser
static char config_st_values[][STRING_LEN] = { "0", "1" };
static char config_st_names[][STRING_LEN] = { "Sommerzeit aus", "Sommerzeit an" };

char config_dim[STRING_LEN];  //chooser
static char config_dim_values[][STRING_LEN] = { "0", "1" };
static char config_dim_names[][STRING_LEN] = { "Dimmeffekt aus", "Dimmeffekt an" };

char config_sm_act[STRING_LEN];  //chooser
static char config_sm_act_values[][STRING_LEN] = { "0", "1" };
static char config_sm_act_names[][STRING_LEN] = { "Deaktiviert", "Aktiviert" };

char config_ss[NUMBER_LEN];  //int
char config_se[NUMBER_LEN];  //int
char config_sm[NUMBER_LEN];  //int

/**char stringParamValue[STRING_LEN];
char intParamValue[NUMBER_LEN];
char floatParamValue[NUMBER_LEN];
char checkboxParamValue[STRING_LEN];
char chooserParamValue[STRING_LEN];**/

//static char chooserValues[][STRING_LEN] = { "red", "blue", "darkYellow" };
//static char chooserNames[][STRING_LEN] = { "Red", "Blue", "Dark yellow" };

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword, CONFIG_VERSION);

IotWebConfParameterGroup group_color = IotWebConfParameterGroup("color", "Farbe");
IotWebConfParameterGroup group_display = IotWebConfParameterGroup("display", "Anzeige");
IotWebConfParameterGroup group_settings = IotWebConfParameterGroup("settings", "Einstellungen");
IotWebConfParameterGroup group_sleepmode = IotWebConfParameterGroup("sleepmode", "Schlafmodus");

// -- You can also use namespace formats e.g.: iotwebconf::TextParameter
//IotWebConfTextParameter stringParam = IotWebConfTextParameter("String param", "stringParam", stringParamValue, STRING_LEN);

//IotWebConfParameterGroup group1 = IotWebConfParameterGroup("group1", "");

IotWebConfNumberParameter config_r_param = IotWebConfNumberParameter("R Wert", "r", config_r, NUMBER_LEN, "0", "0..255", "min='0' max='255' step='1'");
IotWebConfNumberParameter config_g_param = IotWebConfNumberParameter("G Wert", "g", config_g, NUMBER_LEN, "0", "0..255", "min='0' max='255' step='1'");
IotWebConfNumberParameter config_b_param = IotWebConfNumberParameter("B Wert", "b", config_b, NUMBER_LEN, "0", "0..255", "min='0' max='255' step='1'");
IotWebConfNumberParameter config_w_param = IotWebConfNumberParameter("W Wert", "w", config_w, NUMBER_LEN, "255", "0..255", "min='0' max='255' step='1'");
IotWebConfNumberParameter config_br_param = IotWebConfNumberParameter("Helligkeit", "br", config_br, NUMBER_LEN, "150", "0..255", "min='0' max='255' step='1'");

IotWebConfSelectParameter config_td1_param = IotWebConfSelectParameter("Variante 1", "td1", config_td1, STRING_LEN, (char*)config_td1_values, (char*)config_td1_names, sizeof(config_td1_values) / STRING_LEN, STRING_LEN);
IotWebConfSelectParameter config_td2_param = IotWebConfSelectParameter("Variante 2", "td2", config_td2, STRING_LEN, (char*)config_td2_values, (char*)config_td2_names, sizeof(config_td2_values) / STRING_LEN, STRING_LEN);
IotWebConfSelectParameter config_td3_param = IotWebConfSelectParameter("Variante 3", "td3", config_td3, STRING_LEN, (char*)config_td3_values, (char*)config_td3_names, sizeof(config_td3_values) / STRING_LEN, STRING_LEN);
IotWebConfSelectParameter config_td4_param = IotWebConfSelectParameter("Variante 4", "td4", config_td4, STRING_LEN, (char*)config_td4_values, (char*)config_td4_names, sizeof(config_td4_values) / STRING_LEN, STRING_LEN);

IotWebConfNumberParameter config_to_param = IotWebConfNumberParameter("UTC Verschiebung", "to", config_to, NUMBER_LEN, "12", "0..24", "min='0' max='24' step='1'");

IotWebConfSelectParameter config_st_param = IotWebConfSelectParameter("Sommerzeit", "st", config_st, STRING_LEN, (char*)config_st_values, (char*)config_st_names, sizeof(config_st_values) / STRING_LEN, STRING_LEN);
IotWebConfSelectParameter config_dim_param = IotWebConfSelectParameter("Dimmeffekt", "dim", config_dim, STRING_LEN, (char*)config_dim_values, (char*)config_dim_names, sizeof(config_dim_values) / STRING_LEN, STRING_LEN);

IotWebConfSelectParameter config_sm_act_param = IotWebConfSelectParameter("Schafmodus aktiv", "sm_act", config_st, STRING_LEN, (char*)config_sm_act_values, (char*)config_sm_act_names, sizeof(config_sm_act_values) / STRING_LEN, STRING_LEN);
IotWebConfNumberParameter config_ss_param = IotWebConfNumberParameter("Beginn", "ss", config_ss, NUMBER_LEN, "0", "0..23", "min='0' max='23' step='1'");
IotWebConfNumberParameter config_se_param = IotWebConfNumberParameter("Ende", "se", config_se, NUMBER_LEN, "0", "0..23", "min='0' max='23' step='1'");
IotWebConfNumberParameter config_sm_param = IotWebConfNumberParameter("Helligkeit", "sm", config_sm, NUMBER_LEN, "100", "0..255", "min='0' max='255' step='1'");

//IotWebConfNumberParameter intParam = IotWebConfNumberParameter("Int param", "intParam", intParamValue, NUMBER_LEN, "20", "1..100", "min='1' max='100' step='1'");
//IotWebConfSelectParameter chooserParam = IotWebConfSelectParameter("Choose param", "chooseParam", chooserParamValue, STRING_LEN, (char*)chooserValues, (char*)chooserNames, sizeof(chooserValues) / STRING_LEN, STRING_LEN);

// -- We can add a legend to the separator
//IotWebConfParameterGroup group2 = IotWebConfParameterGroup("c_factor", "Calibration factor");

//IotWebConfNumberParameter floatParam = IotWebConfNumberParameter("Float param", "floatParam", floatParamValue, NUMBER_LEN,  nullptr, "e.g. 23.4", "step='0.1'");
//IotWebConfCheckboxParameter checkboxParam = IotWebConfCheckboxParameter("Check param", "checkParam", checkboxParamValue, STRING_LEN,  true);


void setup() {
  strip.begin();
  strip.clear();

  enableLeds();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  group_color.addItem(&config_r_param);
  group_color.addItem(&config_g_param);
  group_color.addItem(&config_b_param);
  group_color.addItem(&config_w_param);
  group_color.addItem(&config_br_param);

  group_display.addItem(&config_td1_param);
  group_display.addItem(&config_td2_param);
  group_display.addItem(&config_td3_param);
  group_display.addItem(&config_td4_param);

  group_settings.addItem(&config_to_param);
  group_settings.addItem(&config_st_param);
  group_settings.addItem(&config_dim_param);

  group_sleepmode.addItem(&config_sm_act_param);
  group_sleepmode.addItem(&config_ss_param);
  group_sleepmode.addItem(&config_se_param);
  group_sleepmode.addItem(&config_sm_param);

  /**group1.addItem(&intParam);
  group2.addItem(&floatParam);
  group2.addItem(&checkboxParam);
  group2.addItem(&chooserParam**/

  //iotWebConf.setStatusPin(STATUS_PIN);
  iotWebConf.setConfigPin(CONFIG_PIN);
  //iotWebConf.addSystemParameter(&stringParam);

  iotWebConf.addParameterGroup(&group_color);
  iotWebConf.addParameterGroup(&group_display);
  iotWebConf.addParameterGroup(&group_settings);
  iotWebConf.addParameterGroup(&group_sleepmode);

  iotWebConf.setConfigSavedCallback(&configSaved);
  iotWebConf.setFormValidator(&formValidator);
  iotWebConf.getApTimeoutParameter()->visible = true;

  // -- Initializing the configuration.
  iotWebConf.init();

  // -- Set up required URL handlers on the web server.
  server.on("/", handleRoot);
  server.on("/config", [] {
    iotWebConf.handleConfig();
  });
  server.onNotFound([]() {
    iotWebConf.handleNotFound();
  });

  Serial.println("Ready.");
  timeClient.begin();
  delay(1000);
}

int currentMinute;
int currentHour;
int currentYear;

int temp;
boolean synced = false;

void updateNTPTime() {
  timeClient.update();
  //Serial.println("try to get time");
  //Serial.println(timeClient.getFormattedTime());

  time_t epochTime = timeClient.getEpochTime();
  //Serial.print("Epoch Time: ");
  //Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  //Serial.print("Formatted Time: ");
  //Serial.println(formattedTime);

  currentHour = timeClient.getHours();
  //Serial.print("Hour: ");
  //Serial.println(currentHour);

  currentMinute = timeClient.getMinutes();
  //Serial.print("Minutes: ");
  //Serial.println(currentMinute);

  /**int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  **/

  struct tm* ptm = gmtime((time_t*)&epochTime);

  /**int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);**/

  currentYear = ptm->tm_year + 1900;
  //Serial.print("Year: ");
  //Serial.println(currentYear);

  if (currentYear >= 2000) {
    synced = true;
  }
}

void loop() {
  // -- doLoop should be called as frequently as possible.
  iotWebConf.doLoop();

  //temp = millis();
  //if(millis() - temp > 60000){
  updateNTPTime();

  if (synced) {
    updateStripe();
  } else {
    enableLeds();
  }
  delay(1000);


  //if (nextUpdateMinute == -1 || nextUpdateMinute >= getMinute()){
  //  updateStripe();
  //  nextUpdateMinute = floor(getMinute() + 5)%60;
  //}
}

/**
 * Handle web requests to "/" path.
 */
void handleRoot() {
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal()) {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>WordClock</title></head><body>Hello world!";
  s += "<ul>";
  /*s += "<li>String param value: ";
  s += stringParamValue;
  s += "<li>Int param value: ";
  s += atoi(intParamValue);
  s += "<li>Float param value: ";
  s += atof(floatParamValue);
  s += "<li>CheckBox selected: ";
//  s += checkboxParam.isChecked();
  s += "<li>Option selected: ";
  s += chooserParamValue;
  s += "</ul>";*/
  s += "Go to <a href='config'>configure page</a> to change values.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}

void configSaved() {
  Serial.println("Configuration was updated.");
}

bool formValidator(iotwebconf::WebRequestWrapper* webRequestWrapper) {
  Serial.println("Validating form.");
  bool valid = true;

  /*
  int l = webRequestWrapper->arg(stringParam.getId()).length();
  if (l < 3)
  {
    stringParam.errorMessage = "Please provide at least 3 characters for this test!";
    valid = false;
  }
*/
  return valid;
}


/** CUSTOM PART **/

int getHour() {
  return (currentHour + atoi(config_to) + atoi(config_st)) % 12;
}

int getRealHour(){
  return (currentHour + atoi(config_to) + atoi(config_st)) % 24;
}

int getMinute() {
  return currentMinute;
}

int getBrightness() {
  int brightness = String(config_br).toInt();

  if (atoi(config_sm_act) == 1){
    int rhour = getRealHour();
    if(atoi(config_ss) < atoi (config_se)){
      //a nach b
    
      if(rhour >= atoi(config_ss) && rhour <= atoi(config_se)){
        brightness = atoi(config_sm);
      }
    }else{
      // b nach a (über nacht)
      if(rhour >= atoi(config_se) || rhour <= atoi(config_ss)){
        brightness = atoi(config_sm);
      }
    }
  }

  if (atoi(config_sm_act) == 1 && ((getHour() <= atoi(config_ss) || getHour() >= atoi(config_se)))) {
    brightness = atoi(config_sm);
  }

  return brightness;
}

void updateStripe() {

  Serial.println("Stripe wird geupdated");

  for (int i = 0; i < 26; i++) {
    enabledWords[i] = false;
  }

  enabledWords[0] = atoi(config_td4);  //ES
  enabledWords[1] = atoi(config_td4);  //IST

  Serial.println(String(getHour()) + ":" + String(getMinute()));

  int minute = getMinute();

  int hourAdjustment = 0;
  if (minute < 5) {
    enabledWords[24] = true;  //UHR
  } else if (minute < 10) {
    enabledWords[2] = true;   //FÜNF
    enabledWords[10] = true;  //NACH
  } else if (minute < 15) {
    enabledWords[3] = true;   //ZEHN
    enabledWords[10] = true;  //NACH
  } else if (minute < 20) {
    enabledWords[7] = true;   //VIERTEL
    enabledWords[10] = true;  //NACH
  } else if (minute < 25) {
    if (atoi(config_td1)) {
      enabledWords[4] = true;   //ZWANZIG
      enabledWords[10] = true;  //NACH
    } else {
      enabledWords[3] = true;   //ZEHN
      enabledWords[8] = true;   //VOR
      enabledWords[11] = true;  //HALB
      hourAdjustment = 1;
    }
  } else if (minute < 30) {
    enabledWords[2] = true;   //FÜNF
    enabledWords[8] = true;   //VOR
    enabledWords[11] = true;  //HALB
    hourAdjustment = 1;
  } else if (minute < 35) {
    enabledWords[11] = true;  //HALB
    hourAdjustment = 1;
  } else if (minute < 40) {
    enabledWords[2] = true;   //FÜNF
    enabledWords[10] = true;  //NACH
    enabledWords[11] = true;  //HALB
    hourAdjustment = 1;
  } else if (minute < 45) {
    if (atoi(config_td2) == 1) {
      enabledWords[4] = true;  //ZWANZIG
      enabledWords[8] = true;  //VOR
    } else {
      enabledWords[3] = true;   //ZEHN
      enabledWords[10] = true;  //NACH
      enabledWords[11] = true;  //HALB
    }
    hourAdjustment = 1;
  } else if (minute < 50) {
    if (atoi(config_td3) == 1) {
      enabledWords[6] = true;  //DREI
      enabledWords[7] = true;  //VIERTEL
    } else {
      enabledWords[7] = true;  //VIERTEL
      enabledWords[8] = true;  //VOR
    }
    hourAdjustment = 1;
  } else if (minute < 55) {
    enabledWords[3] = true;  //ZEHN
    enabledWords[8] = true;  //VOR
    hourAdjustment = 1;
  } else if (minute < 60) {
    enabledWords[2] = true;  //FÜNF
    enabledWords[8] = true;  //VOR
    hourAdjustment = 1;
  }


  switch ((getHour() + hourAdjustment) % 12) {
    case 0:
      enabledWords[21] = true;  //ZWÖLF
      break;
    case 1:
      if (enabledWords[24]) {
        enabledWords[25] = true;
      } else {
        enabledWords[14] = true;  //EINS
      }
      break;
    case 2:
      enabledWords[15] = true;  //ZWEI
      break;
    case 3:
      enabledWords[16] = true;  //DREI
      break;
    case 4:
      enabledWords[17] = true;  //VIER
      break;
    case 5:
      enabledWords[13] = true;  //FÜNF
      break;
    case 6:
      enabledWords[18] = true;  //SECHS
      break;
    case 7:
      enabledWords[20] = true;  //SIEBEN
      break;
    case 8:
      enabledWords[19] = true;  //ACHT
      break;
    case 9:
      enabledWords[23] = true;  //NEUN
      break;
    case 10:
      enabledWords[22] = true;  //ZEHN
      break;
    case 11:
      enabledWords[12] = true;  //ELF
      break;
  }
  Serial.println("");
  enableLeds();
}

void enableLeds() {
  strip.clear();
  strip.setBrightness(getBrightness());

  for (int i = 0; i < 26; i++) {
    boolean newValue = enabledWords[i];
    //Serial.print(String(newValue));
    for (int ii = 0; ii < 16; ii++) {
      int led = wordDefinitions[i][ii];

      if (led != -1) {
        if (newValue) {
          strip.setPixelColor(led, strip.Color(atoi(config_g), atoi(config_r), atoi(config_b), atoi(config_w)));
          //Serial.print(String(atoi(config_r)) + "," + String(atoi(config_g)) + "," + String(atoi(config_b)) + "," + String(atoi(config_g)));
        } else {
          //strip.setPixelColor(led, strip.Color(0,0,0,0));
        }
      }
    }
  }
  strip.show();
}