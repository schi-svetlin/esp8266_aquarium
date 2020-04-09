void updateValues(String newDate, float temp, float newValues_filtered) {
  String date[40];
  String value[40];
  String values_filtered[40];
  for (int i = 0; i <= 38; i++) {
    date[i]   = jsonReadToStr(configChart, "date", i + 1);
    values_filtered[i]  = jsonReadToStr(configChart, "values_filtered", i + 1);
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


void update_chart_values() {
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
        values_day += jsonReadToStr(configChart, "values_day", i).toFloat();
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
