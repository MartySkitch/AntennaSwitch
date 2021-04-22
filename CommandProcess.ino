

void messageReceived(String &topic, String &payload) {
  StaticJsonDocument<75> doc;
  if (dbg == 1) Serial.println("incoming: " + topic + " - " + payload);

  if (topic.equals(TOPIC_SUB))
  {
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, payload);
    JsonObject obj = doc.as<JsonObject>();

    // Test if parsing succeeds.
    if (error) {
      if (dbg == 1) Serial.print(F("deserializeJson() failed: "));
      if (dbg == 1) Serial.println(error.f_str());
      return;
    }

    String cmd = obj["cmd"];
    cmd.toUpperCase();
    char charCmd[cmd.length() + 2];
    cmd.toCharArray(charCmd, cmd.length() + 1 );
    int sw = obj["arg"];

    if (strcmp(charCmd, "VER") == 0) {
      send_msg(cmd, sw_version);
    }
    else if (strcmp(charCmd, "GET") == 0) {
      send_msg("SW", String(currentSwitch));
    }
    else if (strcmp(charCmd, "SET") == 0) {
      SetSwitch(sw);
      send_msg(cmd, String(currentSwitch));
    }

    if (dbg == 1) Serial.println("Topic is hello and payload is - " + payload);
    if (dbg == 1) Serial.println("charCmd is - " + String(charCmd));
    if (dbg == 1) Serial.println("Cmd is - " + cmd);
    if (dbg == 1) Serial.println("arg is - " + String(sw) );
    delay(1000);
  }
}

//-------------------------------------------------------------------------------
void send_msg(String cmd, String payLoad) {
  StaticJsonDocument<75> doc;
  char buffer[75];
  doc[cmd] = payLoad;
  size_t n = serializeJson(doc, buffer);
  client.publish(TOPIC_PUB, buffer, n);
  if (dbg == 1) serializeJson(doc, Serial);
  return;
}
