void WIFIinit() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  wifi_set_sleep_type(NONE_SLEEP_T);

  String _ssid = jsonRead(configSetup, "ssid");
  String _password = jsonRead(configSetup, "password");
  if (_ssid != "" && _password != "") {
    WiFiMulti.addAP(_ssid.c_str(), _password.c_str());
  }
  Serial.println("");
  while (--tries && WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    sprintf(line1, "CONNECTING: %02d", tries);
    sprintf(line2, "       VER: %s", ver);
    printLCD(2, 0, 0, line1, line2, 1000);
  }
  if (WiFiMulti.run() != WL_CONNECTED)  {
    StartAPMode();
  }
  else {
    printLCD(2, 0, 0, "                ", "                ", 0);    
    //lcd_clear(2);      
    wifi_working = 1;
    ip =  WiFi.localIP();
    Serial.printf("WIFI CONNECTED \nIP ADDRESS: %d.%d.%d.%d \n", ip[0], ip[1], ip[2], ip[3]);
    printLCD(2, 0, 0, "CONNECTED...IP: ", ip.toString(), 1000);
    printLCD(2, 0, 0, "                ", "                ", 0);
    //lcd_clear(2);    
    jsonWrite(configSetup, "ip", ip.toString());
  }
}

bool StartAPMode() {
  String _ssidAP = jsonRead(configSetup, "ssidAP");
  String _passwordAP = jsonRead(configSetup, "passwordAP");
  
  //String _ssidAP = "ESP8266_AQUA";
  //String _passwordAP = "";
  wifi_working = 0;
  Serial.println("");
  Serial.printf("AP CREATED SUCCESSFULLY \nSSID: %s | PASSWORD: EMPTY \nIP ADDRESS: 192.168.4.1 \n", _ssidAP.c_str(), _passwordAP.c_str());
  printLCD(2, 0, 0, "   AP CREATE    ", "                ", 1500);
  printLCD(2, 0, 0, _ssidAP, "IP: 192.168.4.1", 2000);
  //lcd_clear(2);   
  printLCD(2, 0, 0, "                ", "                ", 0);  
  IPAddress apIP(192, 168, 4, 1);
  IPAddress staticGateway(192, 168, 4, 1);
  IPAddress staticSubnet(255, 255, 255, 0);
  jsonWrite(configSetup, "ip", apIP.toString());
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
  dnsServer.start(53, "*", apIP);
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  return true;
}
