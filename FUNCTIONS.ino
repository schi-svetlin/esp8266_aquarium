void readJsonValues() {
  jsonWrite(configSetup, "ver", ver);
  saveConfigSetup();
  temp_koef = jsonReadToFloat(configSetup, "input", 0);
  max_day   = jsonReadToInt(configSetup,   "input", 1);
  max_night = jsonReadToInt(configSetup,   "input", 2);
  fan_start = jsonReadToFloat(configSetup, "input", 3);
  fan_stop  = jsonReadToFloat(configSetup, "input", 4);
  ten_start = jsonReadToFloat(configSetup, "input", 5);
  ten_stop  = jsonReadToFloat(configSetup, "input", 6);
  port      = jsonReadToInt(configSetup,   "input", 11);
  timeZone  = jsonReadToInt(configSetup,   "input", 14);
  //                  map(Value, fromLow, fromHigh, toLow, toHigh);
  max_day_percent   = map(max_day, 0, 100, 0, 1024);
  max_night_percent = map(max_night, 100, 0, 1024, 0);

  for (int a = 0; a <= 1; a++) {
    for (int b = 0; b <= 6; b++) {
      for (int c = 0; c <= 3; c++) {
//                 row col
        schedule[a][b][c]= jsonReadToStr(configSetup, "schedule", a, b, c);
      }
    }
  }
}

void led_schedule() {
  if (schedule[0][nedelya][0] != "" || schedule[0][nedelya][1] != "" || schedule[0][nedelya][2] != "" || schedule[0][nedelya][3] != "") {
      hour_0[nedelya] = schedule[0][nedelya][0].substring(0, 2).toInt();
      min_0[nedelya]  = schedule[0][nedelya][0].substring(3, 5).toInt();
      hour_1[nedelya] = schedule[0][nedelya][1].substring(0, 2).toInt();
      min_1[nedelya]  = schedule[0][nedelya][1].substring(3, 5).toInt();
      hour_2[nedelya] = schedule[0][nedelya][2].substring(0, 2).toInt();
      min_2[nedelya]  = schedule[0][nedelya][2].substring(3, 5).toInt();
      hour_3[nedelya] = schedule[0][nedelya][3].substring(0, 2).toInt();
      min_3[nedelya]  = schedule[0][nedelya][3].substring(3, 5).toInt();
//Serial.printf("ned %d | nedelya %d | от %02d:%02d | до %02d:%02d  |  от %02d:%02d | до %02d:%02d\n", ned, nedelya, hour_0[nedelya], min_0[nedelya], hour_1[nedelya], min_1[nedelya], hour_2[nedelya], min_2[nedelya], hour_3[nedelya], min_3[nedelya]);
      sec_0   = hour_0[nedelya] * 60 * 60 + min_0[nedelya] * 60;
      sec_1   = hour_1[nedelya] * 60 * 60 + min_1[nedelya] * 60;
      sec_2   = hour_2[nedelya] * 60 * 60 + min_2[nedelya] * 60;
      sec_3   = hour_3[nedelya] * 60 * 60 + min_3[nedelya] * 60;
      sec_now = ds_hour * 60 * 60 + ds_min * 60 + ds_sec;
      if (sec_now < sec_0 || sec_now > sec_3) {
        analogWrite(LEDPIN, max_night_percent);
        led_bright = max_night;
        led_working = 0;
      }
      if (sec_now >= sec_0 && sec_now <= sec_1) {     // Рассвет
        val_rassvet = map(sec_now, sec_0, sec_1, max_night_percent, max_day_percent);
        led_bright = map(sec_now, sec_0, sec_1, max_night, max_day);
        analogWrite(LEDPIN, val_rassvet);
        led_working = 1;
      }
      if (sec_now >= sec_2 && sec_now <= sec_3) { // Закат
        val_zakat = map(sec_now, sec_2, sec_3, max_day_percent, max_night_percent);
        led_bright = map(sec_now, sec_2, sec_3, max_day, max_night);
        analogWrite(LEDPIN, val_zakat);
        led_working = 1;
      } else if (sec_now > sec_1 && sec_now < sec_2) {
        analogWrite(LEDPIN, max_day_percent);
        led_bright = max_day;
        led_working = 1;
      }
  }
}
void relay_schedule() {
  if (schedule[1][nedelya][0] != "" || schedule[1][nedelya][1] != "" || schedule[1][nedelya][2] != "" || schedule[1][nedelya][3] != "") {    
    hour_0[nedelya] = schedule[1][nedelya][0].substring(0, 2).toInt();
    min_0[nedelya]  = schedule[1][nedelya][0].substring(3, 5).toInt();
    hour_1[nedelya] = schedule[1][nedelya][1].substring(0, 2).toInt();
    min_1[nedelya]  = schedule[1][nedelya][1].substring(3, 5).toInt();
    hour_2[nedelya] = schedule[1][nedelya][2].substring(0, 2).toInt();
    min_2[nedelya]  = schedule[1][nedelya][2].substring(3, 5).toInt();
    hour_3[nedelya] = schedule[1][nedelya][3].substring(0, 2).toInt();
    min_3[nedelya]  = schedule[1][nedelya][3].substring(3, 5).toInt();
    //Serial.printf("ned %d | nedelya %d | от %02d:%02d | до %02d:%02d  |  от %02d:%02d | до %02d:%02d\n", ned, nedelya, hour_0[nedelya], min_0[nedelya], hour_1[nedelya], min_1[nedelya], hour_2[nedelya], min_2[nedelya], hour_3[nedelya], min_3[nedelya]);
    sec_0    = hour_0[nedelya] * 60 * 60 + min_0[nedelya] * 60;
    sec_1    = hour_1[nedelya] * 60 * 60 + min_1[nedelya] * 60;
    sec_2    = hour_2[nedelya] * 60 * 60 + min_2[nedelya] * 60;
    sec_3    = hour_3[nedelya] * 60 * 60 + min_3[nedelya] * 60;
    sec_now  = ds_hour * 60 * 60 + ds_min * 60 + ds_sec;
    if (sec_now < sec_0 || sec_now > sec_1) {
      digitalWrite(RELAY1PIN, LOW);
      relay1_working = 0;
    }
    if (sec_now >= sec_0 && sec_now <= sec_1) {
      digitalWrite(RELAY1PIN, HIGH);
      relay1_working = 1;
    }
    if (sec_now < sec_2 || sec_now > sec_3) {
      digitalWrite(RELAY2PIN, LOW);
      relay2_working = 0;
    }
    if (sec_now >= sec_2 && sec_now <= sec_3) {
      digitalWrite(RELAY2PIN, HIGH);
      relay2_working = 1;
    }
  }
}
void temp_fan_regulation() {
  if (tempC1 != 0) {
    if (tempC1 >= fan_start) {
      digitalWrite(FANPIN, HIGH);
      fan_working = 1;
    } else if (tempC1 <= fan_stop) {
      digitalWrite(FANPIN, LOW);
      fan_working = 0;
    }
  } else {
    digitalWrite(FANPIN, LOW);
    fan_working = 0;
  }
}
void temp_ten_regulation() {
  if (tempC1 != 0) {
    if (tempC1 <= ten_start) {
      digitalWrite(TENPIN, HIGH);
      ten_working = 1;
    } else if (tempC1 >= ten_stop) {
      digitalWrite(TENPIN, LOW);
      ten_working = 0;
    }
  } else {
    digitalWrite(TENPIN, LOW);
    ten_working = 0;
  }
}
