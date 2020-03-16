void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("Websocket-соединение с [%u] разорвано!\n", num);
      ws_working = 0;
      break;
    case WStype_CONNECTED:
      //IPAddress ip = webSocket.remoteIP(num);
      ws_working = 1;
      Serial.printf("Установлено Websocket-соединение с [%u]!\n", num);
      //  Serial.printf("Сообщение от [%u] IP: %d.%d.%d.%d: ", num, ip[0], ip[1], ip[2], ip[3], payload);
      //  webSocket.sendTXT(num, " connected");
      break;
    case WStype_TEXT:
      //Serial.printf("Сообщение от [%u]: %s\n", num, payload);
      ws_working = 1;
      graph_changing = num;
      //webSocket.sendTXT(num, jsonLive);
      //webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("get binary length from [%u]: %u\n", num, length);
      hexdump(payload, length);
      // webSocket.sendBIN(num, payload, length);
      break;
  }
}
