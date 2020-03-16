void readJsonValues() {
  jsonWrite(configSetup, "ver", ver);
  saveConfigSetup();
  brightness  = jsonRead(configSetup, "brightness").toInt();
  max_day     = jsonRead(configSetup, "max_day").toInt();
  max_night   = jsonRead(configSetup, "max_night").toInt();
  //                  map(Value, fromLow, fromHigh, toLow, toHigh);
  max_day_percent   = map(max_day, 0, 100, 0, 1024);
  max_night_percent = map(max_night, 100, 0, 1024, 0);

  timeZone    = jsonRead(configSetup, "timeZone").toInt();
  temp_koef   = atof(jsonRead(configSetup, "temp_koef").c_str());
  fan_start   = atof(jsonRead(configSetup, "fan_start").c_str());
  fan_stop    = atof(jsonRead(configSetup, "fan_stop").c_str());
  ten_start   = atof(jsonRead(configSetup, "ten_start").c_str());
  ten_stop    = atof(jsonRead(configSetup, "ten_stop").c_str());
  for (int i = 0; i <= 6; i++) {
    time_vkl[i]      = jsonRead(configSetup, "time_vkl", i);
    time_rassvet[i]  = jsonRead(configSetup, "time_rassvet", i);
    time_zakat[i]    = jsonRead(configSetup, "time_zakat", i);
    time_otkl[i]     = jsonRead(configSetup, "time_otkl", i);
  }
}

void handleValues() {
  sprintf(date_now, "%02i.%02i.%02i | %02i:%02i:%02i",  ds_day, ds_month, ds_year, ds_hour, ds_min, ds_sec);
  String send_temp = String(round(tempC1 * 100) / 100);
  jsonWrite(jsonLive, "temp", send_temp);
  jsonWrite(jsonLive, "now", date_now);
  jsonWrite(jsonLive, "led", led_bright);
  jsonWrite(jsonLive, "fan", fan_working);
  jsonWrite(jsonLive, "ten", ten_working);
  jsonWrite(jsonLive, "rssi", rssi);  
  jsonWrite(jsonLive, "graph_changing", graph_changing);  
  webSocket.broadcastTXT(jsonLive);
}

void updateValuesPeriod() {
  float values_filtered = 0;
  if (ds_hour > 21 || ds_hour < 3)   {
    flag = 0;
    hour_alarm = 3;
  }
  if (ds_hour > 3 && ds_hour < 9)   {
    flag = 1;
    hour_alarm = 9;
  }
  if (ds_hour > 9  && ds_hour < 15)  {
    flag = 2;
    hour_alarm = 15;
  }
  if (ds_hour > 15 && ds_hour < 21)  {
    flag = 3;
    hour_alarm = 21;
  }
  if (ds_hour == hour_alarm && tempC1 != 0.00 && flag <= 3) {
    jsonWrite(configChart, "values_day", flag, temp_filtered);
    saveConfigChart();
    if (hour_alarm == 21) {
      jsonWrite(configChart, "values_day", flag, temp_filtered);
      saveConfigChart();
      sprintf(time_now, "%02d.%02d.%02i",  ds_day, ds_month, ds_year);
      for (int i = 0; i <= 3; i++) {
        values_day += atof(jsonRead(configChart, "values_day", i).c_str());
      }
      values_filtered = values_day / 4;
      updateValues(time_now, tempC1, values_filtered); 
      graph_changing = 1;    
    }
    flag = 4;
    values_day = 0;
    values_filtered = 0;
  }
}

void measure_t() {
  unsigned long realMillis = millis() / 1000;
  if (realMillis - previousMillis >= 1) {
    DateTime now = rtc.now();
    ds_day = now.day();
    ds_month = now.month();
    ds_year = now.year();
    ds_hour = now.hour();
    ds_min = now.minute();
    ds_sec = now.second();
    nedelya = now.dayOfTheWeek();
    rssi =  WiFi.RSSI();
    //randomize = random(1500, 3100);
    //tempC = (float)randomize / 100.00;
//    sensors.requestTemperatures();
//    tempC = sensors.getTempC(t1);
    ds18b20_measure();
    readJsonValues();
    handleValues();
    sprintf(line1, "\2:%2.2f\1C",  tempC1);   
    if (tempC == 0.00) {
      sprintf(line1, "\2: -ERR-");       
    } 
    sprintf(line2, "\6%3idBm   \3%3i%%", rssi, led_bright);
    tempC1 = tempC + temp_koef;
    temp_filtered = avg.update((float)tempC1);
    printLCD(2,0,0,line1,line2,0);    
    updateZnak(4, fan_working,  12, 0);
    updateZnak(5, ten_working,  13, 0);
    //updateZnak(6, wifi_working, 14, 0);
    updateZnak(7, ws_working,   15, 0);
    led_schedule();
    temp_fan_regulation();
    temp_ten_regulation();
    updateValuesPeriod();
    previousMillis = realMillis;
  }
}
