#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>
#include <SimpleMovingAverage.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266FtpServer.h>

//================================================================================================================================
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
//================================================================================================================================
#define SCL           D1  // SCL
#define SDA           D2  // SDA
#define ONE_WIRE_BUS  D3  // DS18B20
#define TENPIN        D5
#define LEDPIN        D6
#define FANPIN        D7
String f_ver = "4.2";
const char* ver = "2.8";

static const char successResponse[] PROGMEM =
  "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...\n";

LiquidCrystal_I2C lcd(0x27, 16, 2);

SimpleMovingAverage avg;
ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

WebSocketsServer webSocket = WebSocketsServer(81);
FtpServer ftpSrv;
File fsUploadFile;
DNSServer dnsServer;
OneWire  ds(ONE_WIRE_BUS);

byte tries = 11;

IPAddress ip;
String configSetup = "{}";
String configChart = "{}";
String jsonLive = "{\"temp\":\"--\",\"now\":\"--.--.----\",\"led\":\"-\",\"fan\":\"-\",\"ten\":\"-\",\"rssi\":\"-\",\"graph_changing\":\"-\"}";

String clientId = "ESP8266";
float avTemp, tempC1, tempC, temp_filtered, values_day, temp_koef, fan_stop, fan_start, ten_start, ten_stop;
byte led_working = 0;
byte fan_working = 0;
byte ten_working = 0;
byte wifi_working = 0;
byte ws_working = 0;
byte graph_changing = 0;
int val_rassvet = 0;
int val_zakat = 0;
int led_bright = 0;
int old_ds_min = 0;
int old_ds_sec = 0;
int randomize, rssi;
byte nedelya;
String date_man, time_man;
int brightness, max_day, max_day_percent, max_night, max_night_percent, year_man;
unsigned long previousMillis = 0;
int uploadProc = 0;
int flag, hour_alarm;
char line1[16], line2[16];
//floor() — округление вниз
//ceil() — округление вверх
//round() — округление в ближайшую сторону
