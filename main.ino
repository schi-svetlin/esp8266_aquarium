// ------------- Чтение значения json
String jsonRead(String &json, String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name].as<String>();
}

String jsonRead(String &json, String name, int i) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name][i].as<String>();
}

/*
  // ------------- Чтение значения json
  int jsonReadtoInt(String &json, String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name];
  }
*/
// ------------- Запись значения json String
String jsonWrite(String &json, String name, String volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}

String jsonWrite(String &json, String name, int i, String volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name][i] = volume;
  json = "";
  root.printTo(json);
  return json;
}

// ------------- Запись значения json int
String jsonWrite(String &json, String name, int volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}

String jsonWrite(String &json, String name, int i, int volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name][i] = volume;
  json = "";
  root.printTo(json);
  return json;
}

// ------------- Запись значения json float
String jsonWrite(String &json, String name, float volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}
String jsonWrite(String &json, String name, int i, float volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name][i] = volume;
  json = "";
  root.printTo(json);
  return json;
}

void saveConfigSetup () {
  writeFile("configSetup.json", configSetup );
}

void saveConfigChart () {
  writeFile("configChart.json", configChart );
}

// ------------- Запись строки в файл
String writeFile(String fileName, String strings ) {
  File configFile = SPIFFS.open("/" + fileName, "w");
  if (!configFile) {
    return "Failed to open config file";
  }
  configFile.print(strings);
  //strings.printTo(configFile);
  configFile.close();
  return "Write success";
}

// ------------- Чтение файла в строку
String readFile(String fileName, size_t len ) {
  File configFile = SPIFFS.open("/" + fileName, "r");
  if (!configFile) {
    return "Failed";
  }
  size_t size = configFile.size();
  if (size > len) {
    configFile.close();
    return "Large";
  }
  String temp = configFile.readString();
  configFile.close();
  return temp;
}
