//==========================================================================================================
//                                                 FILE SYSTEM OPTIONS
//==========================================================================================================
void FS_init(void) {
  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
  }
  server.on("/readValues", handleValues);
  server.on("/restart", handle_restart);
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");

  });
  //  jsonWrite(configSetup, "flashChip", String(ESP.getFlashChipId(), HEX));
  server.on("/configSetup.json", HTTP_GET, []() {
    server.send(200, "application/json", configSetup);
  });

  //==========================================================================================================
  //                                                 WIFI
  //==========================================================================================================
  server.on("/ssid", HTTP_GET, []() {
    jsonWrite(configSetup, "ssid", String(server.arg("ssid")));
    jsonWrite(configSetup, "password", String(server.arg("password")));
    saveConfigSetup();
    server.send(200, "text/plain", "");
  });
  server.on("/ssidAP", HTTP_GET, []() {
    jsonWrite(configSetup, "ssidAP", String(server.arg("ssidAP")));
    jsonWrite(configSetup, "passwordAP", String(server.arg("passwordAP")));
    saveConfigSetup();
    server.send(200, "text/plain", "");
  });
  //==========================================================================================================
  //                                                 PORT
  //==========================================================================================================
  server.on("/portNumber", HTTP_GET, []() {
    jsonWrite(configSetup, "portNumber", String(server.arg("portNumber")));
    saveConfigSetup();
    server.send(200, "text/plain", "");
  });
  //==========================================================================================================
  //                                                 DEFAULT LANG SAVE
  //==========================================================================================================
  server.on("/lang", HTTP_GET, []() {
    jsonWrite(configSetup, "defaultLang", String(server.arg("defaultLang")));
    saveConfigSetup();
    server.send(200, "text/plain", "");
  });  
  //==========================================================================================================
  //                                                 UPDATE FS
  //==========================================================================================================
  server.on("/update", HTTP_POST, [&]() {
    if (Update.hasError()) {
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    } else {
      server.client().setNoDelay(true);
      server.send_P(200, PSTR("text/html"), successResponse);
      delay(100);
      server.client().stop();
      Serial.printf("Rebooting...\n");
      printLCD(2, 0, 0, "   RESTARTING   ", " PLEASE WAIT... ", 10);
      printLCD(2, 0, 0, "                ", "                ", 0);
      ESP.restart();
    }
  }, [&]() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      printLCD(2, 0, 0, "                ", "                ", 0);
      //Serial.setDebugOutput(true);
      WiFiUDP::stopAll();
      Serial.printf("--UPLOAD_FILE_START-- \nUpdate filename: %s \n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      uploadProc += upload.currentSize;
      int perc = map(uploadProc, 0, 400000, 0, 100);
      Serial.printf("Writing: %i bytes (%i %%) \n", uploadProc, perc);
      initBar2();
      lcd.setCursor(0, 0);
      lcd.print("UPLOADING FILE: ");
      fillBar2(0, 1, 16, perc);
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) {
        Serial.printf("--UPLOAD_FILE_END-- \nUpdate-- \nSuccess: %u\n", upload.totalSize);
        String line2 = upload.totalSize + " bytes";
        printLCD(2, 0, 0, "SUCCESS...TOTAL:", line2, 1000);
        printLCD(2, 0, 0, "                ", "                ", 0);
      } else {
        Update.printError(Serial);
      }
      //Serial.setDebugOutput(false);
    } else if (upload.status == UPLOAD_FILE_ABORTED) {
      Serial.printf("UPLOAD_FILE_ABORTED \n");
      Update.end();
      Serial.println("--UPLOAD_FILE_ABORTED-- \n");
    }
    yield();
  });

  //==========================================================================================================
  //                                                 SAVE LED SCHEDULE
  //==========================================================================================================
  server.on("/save_schedule", HTTP_GET, []() {
    for (int i = 0; i <= 6; i++) {
      time_vkl[i]      = server.arg("time_vkl_" + String(i));
      time_rassvet[i]  = server.arg("time_rassvet_" + String(i));
      time_zakat[i]    = server.arg("time_zakat_" + String(i));
      time_otkl[i]     = server.arg("time_otkl_" + String(i));
      jsonWrite(configSetup, "time_vkl",     i, time_vkl[i]);
      jsonWrite(configSetup, "time_rassvet", i, time_rassvet[i]);
      jsonWrite(configSetup, "time_zakat",   i, time_zakat[i]);
      jsonWrite(configSetup, "time_otkl",    i, time_otkl[i]);
    }
    saveConfigSetup();
    printLCD(1, 0, 0, "", " SAVE SCHEDULE  ", 1000);
    printLCD(1, 0, 0, "", "                ", 0);
    server.send(200, "text/plain", "");
  });

  //==========================================================================================================
  //                                                 AUTO SYNCHRONISATION
  //==========================================================================================================
  server.on("/auto_sync", HTTP_GET, []() {
    timeZone   = server.arg("timeZone").toInt();
    jsonWrite(configSetup, "timeZone",   server.arg("timeZone"));
    saveConfigSetup();
    updateTimeNTP();
    if (years < 2019) {
      printLCD(2, 0, 0, "  NO INTERNET   ", "   CONNECTION   ", 2000);
      printLCD(2, 0, 0, "                ", "                ", 0);
      server.send(404, "text/plain", "");
    } else {
      rtc.adjust(DateTime(years, months, days, hours, minutes, seconds));
      Serial.printf("Autosync button pressed \nNTP time now: %02d.%02d.%02i | %02d:%02d:%02d \n", days, months, years, hours, minutes, seconds);
      printLCD(1, 0, 0, "", "AUTOSYNC PRESSED", 2000);
      printLCD(1, 0, 0, "", "                ", 0);
      server.send(200, "text/plain", "");
    }
  });

  //==========================================================================================================
  //                                                 MANUAL SYNCHRONISATION
  //==========================================================================================================
  server.on("/save_date", HTTP_GET, []() {
    date_man   = server.arg("date_man");
    time_man   = server.arg("time_man");
    jsonWrite(configSetup, "date_man",   date_man);
    jsonWrite(configSetup, "time_man", time_man);
    Serial.print("date_man: ");
    Serial.print(date_man);        
    Serial.print(" | time_man: ");
    Serial.println(time_man); 
    years = date_man.substring(0, 4).toInt();
    months = date_man.substring(5, 7).toInt();
    days = date_man.substring(8, 10).toInt();
    hours = time_man.substring(0, 2).toInt();
    minutes = time_man.substring(3, 5).toInt();
    saveConfigSetup();
    rtc.adjust(DateTime(years, months, days, hours, minutes, 50));
    Serial.printf("Manual button pressed \nEntered time: %02d.%02d.%02i | %02d:%02d \n", days, months, years, hours, minutes);
    printLCD(1, 0, 0, "", " MANUAL PRESSED ", 2000);
    printLCD(1, 0, 0, "", "                ", 0);
    server.send(200, "text/plain", "");
  });

  //==========================================================================================================
  //                                                 SAVE TRANSMITTERS
  //==========================================================================================================
  server.on("/save", HTTP_GET, []() {
    fan_start = atof(server.arg("fan_start").c_str());
    fan_stop  = atof(server.arg("fan_stop").c_str());
    ten_start = atof(server.arg("ten_start").c_str());
    ten_stop  = atof(server.arg("ten_stop").c_str());
    brightness   = server.arg("brightness").toInt();
    temp_koef    = atof(server.arg("temp_koef").c_str());
    max_day      = server.arg("max_day").toInt();
    max_night    = server.arg("max_night").toInt();
    jsonWrite(configSetup, "fan_start",  fan_start);
    jsonWrite(configSetup, "fan_stop",   fan_stop);
    jsonWrite(configSetup, "ten_start",  ten_start);
    jsonWrite(configSetup, "ten_stop",   ten_stop);
    jsonWrite(configSetup, "brightness", brightness);
    jsonWrite(configSetup, "temp_koef",  temp_koef);
    jsonWrite(configSetup, "max_day",    max_day);
    jsonWrite(configSetup, "max_night",  max_night);
    saveConfigSetup();
    printLCD(1, 0, 0, "", "  SAVE OPTIONS  ", 1000);
    printLCD(1, 0, 0, "", "                ", 0);
    server.send(200, "text/plain", "");
  });
  httpUpdater.setup(&server);
  server.begin();
}

//==========================================================================================================
//                                                 RESTART ESP
//==========================================================================================================
void handle_restart() {
  String restart = server.arg("device");
  if (restart == "1") {
    server.send(200, "text/plain", "Restarting. Please wait...");
    printLCD(2, 0, 0, "   RESTARTING   ", "PLEASE WAIT...  ", 500);
    printLCD(2, 0, 0, "                ", "                ", 0);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "ERROR Restarting");
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload() {
  if (server.uri() != "/edit") { return; }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) { filename = "/" + filename; }
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (fsUploadFile) { fsUploadFile.write(upload.buf, upload.currentSize); }
  } else if (upload.status == UPLOAD_FILE_END) { 
      if (fsUploadFile) { fsUploadFile.close(); }
  }
}

void handleFileDelete() {
  if (server.args() == 0) { return server.send(500, "text/plain", "BAD ARGS"); }
  String path = server.arg(0);
  if (path == "/") { return server.send(500, "text/plain", "BAD PATH"); }
  if (!SPIFFS.exists(path)) { return server.send(404, "text/plain", "FileNotFound"); }
  SPIFFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate() {
  if (server.args() == 0) { return server.send(500, "text/plain", "BAD ARGS"); }
  String path = server.arg(0);
  if (path == "/") { return server.send(500, "text/plain", "BAD PATH"); }
  if (SPIFFS.exists(path)) { return server.send(500, "text/plain", "FILE EXISTS"); }
  File file = SPIFFS.open(path, "w");
  if (file) { file.close(); } 
  else { return server.send(500, "text/plain", "CREATE FAILED"); }
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileList() {
  if (!server.hasArg("dir")) { server.send(500, "text/plain", "BAD ARGS"); return;  }
  String path = server.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") { output += ','; }
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    if (entry.name()[0] == '/') { output += &(entry.name()[1]); } 
    else { output += entry.name(); }
    output += "\"}";
    entry.close();
  }
  output += "]";
  server.send(200, "text/json", output);
}
