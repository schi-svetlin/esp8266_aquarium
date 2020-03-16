void led_schedule() {
  if (time_vkl[nedelya] != "" || time_otkl[nedelya] != "" || time_rassvet[nedelya] != "" || time_zakat[nedelya] != "") {
    hour_vkl[nedelya]     = time_vkl[nedelya].substring(0, 2).toInt();
    min_vkl[nedelya]      = time_vkl[nedelya].substring(3, 5).toInt();
    hour_rassvet[nedelya] = time_rassvet[nedelya].substring(0, 2).toInt();
    min_rassvet[nedelya]  = time_rassvet[nedelya].substring(3, 5).toInt();
    hour_zakat[nedelya]   = time_zakat[nedelya].substring(0, 2).toInt();
    min_zakat[nedelya]    = time_zakat[nedelya].substring(3, 5).toInt();
    hour_otkl[nedelya]    = time_otkl[nedelya].substring(0, 2).toInt();
    min_otkl[nedelya]     = time_otkl[nedelya].substring(3, 5).toInt();
    sec_start             = hour_vkl[nedelya] * 60 * 60 + min_vkl[nedelya] * 60;
    sec_rassvet           = hour_rassvet[nedelya] * 60 * 60 + min_rassvet[nedelya] * 60;
    sec_zakat             = hour_zakat[nedelya] * 60 * 60 + min_zakat[nedelya] * 60;
    sec_stop              = hour_otkl[nedelya] * 60 * 60 + min_otkl[nedelya] * 60;
    sec_now               = ds_hour * 60 * 60 + ds_min * 60 + ds_sec;

    if (sec_now < sec_start || sec_now > sec_stop) {
      analogWrite(LEDPIN, max_night_percent);
      led_bright = max_night;
      led_working = 0;
    }
    if (sec_now >= sec_start && sec_now <= sec_rassvet) {     // Рассвет
      val_rassvet = map(sec_now, sec_start, sec_rassvet, max_night_percent, max_day_percent);
      led_bright = map(sec_now, sec_start, sec_rassvet, max_night, max_day);
      analogWrite(LEDPIN, val_rassvet);
      led_working = 1;
    }
    if (sec_now >= sec_zakat && sec_now <= sec_stop) { // Закат
      val_zakat = map(sec_now, sec_zakat, sec_stop, max_day_percent, max_night_percent);
      led_bright = map(sec_now, sec_zakat, sec_stop, max_day, max_night);
      analogWrite(LEDPIN, val_zakat);
      led_working = 1;
    } else if (sec_now > sec_rassvet && sec_now < sec_zakat) {
      analogWrite(LEDPIN, max_day_percent);
      led_bright = max_day;
      led_working = 1;
    }
  }
}

void updateValues(String newDate, float temp, float newValues_filtered) {
  String date[40];
  String value[40];
  String values_filtered[40];
  for (int i = 0; i <= 38; i++) {
    date[i]   = jsonRead(configChart, "date", i + 1);
    values_filtered[i]  = jsonRead(configChart, "values_filtered", i + 1);
  }
  if (temp == 0.00) {
    date[39] = newDate;
    values_filtered[39]  = values_filtered[38];
  } else {
    date[39] = newDate;
    values_filtered[39]  = newValues_filtered;
  }
  for (int i = 0; i <= 39; i++) {
    jsonWrite(configChart, "date", i, date[i]);
    jsonWrite(configChart, "values_filtered", i, values_filtered[i]);
  }
  saveConfigChart();
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
