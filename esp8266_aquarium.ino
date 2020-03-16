#include "include_define.h"
#include "rtc.h"
#include "ruswords.h"

//==========================================================================================================
//                                                 INITIAL SETUP
//==========================================================================================================
void setup() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(TENPIN, OUTPUT);
  analogWrite(LEDPIN, 0);
  digitalWrite(FANPIN, LOW);
  digitalWrite(TENPIN, LOW); 
//============================================================================================================================
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args,BYTE);
#endif
  initLCD();
//============================================================================================================================
  FS_init();
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  configSetup  = readFile("configSetup.json", 4096);
  configChart  = readFile("configChart.json", 4096);
  readJsonValues();
  WIFIinit();
  if (WiFi.getAutoConnect() != true) WiFi.setAutoConnect(true);
  rtc.begin();
  avg.begin();
  udp.begin(localPort);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  ftpSrv.begin("esp", "esp");
}

//==========================================================================================================
//                                                 LOOP
//==========================================================================================================
void loop() {
  server.handleClient();
  yield();
  dnsServer.processNextRequest();
  measure_t();
  webSocket.loop();
  ftpSrv.handleFTP();
}
