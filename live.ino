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
    ned = now.dayOfTheWeek(); 
    if (ned==0) {nedelya = 6;} else
    {nedelya = ned-1;}      
    rssi =  WiFi.RSSI();
    //randomize = random(1500, 3100);
    //tempC = (float)randomize / 100.00;
    ds18b20_measure();
    send_values_by_websocket();
    tempC1 = tempC + temp_koef;
    temp_filtered = avg.update((float)tempC1);
    if (tempC == 0.0 || tempC == 85.0) { 
      sprintf(line1, "%02i:%02i   t: -ERR- ",ds_hour, ds_min); 
    } else { 
      sprintf(line1, "%02i:%02i  t:%2.1f\2C",  ds_hour, ds_min, atof(send_temp)); 
    }
    sprintf(line2_1, "\1%i",rssi);
    sprintf(line2_2, "\3%i",led_bright);    
    printLCD(2,0,0,line1,line2_1,0); 
    printLCD(1,0,6,"",line2_2,0);        
    updateZnak(7, relay1_working, 11, 1);
    updateZnak(8, relay2_working, 12, 1);
    updateZnak(4, fan_working, 13, 1);
    updateZnak(5, ten_working, 14, 1);
    updateZnak(6, ws_working, 15, 1);
    led_schedule();
    relay_schedule();    
    temp_fan_regulation();
    temp_ten_regulation();
    update_chart_values();
    previousMillis = realMillis;
  }
}
