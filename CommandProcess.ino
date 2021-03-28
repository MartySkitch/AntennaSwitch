// ==============================================================================
// Constants
//==============================================================================
  const String packetStart = "/";
  const String packetEnd = "\r";
  const String delimiter = ":";
  const int numOfElements = 3;
//==============================================================================

  
//--- COMMAND PROCESSING  ----------------------------------------
void ProcessCommand() {
  String cmd;
  int x = 0;
  String arg;  
  int packetLen; 
  String packet; 
  int firstDelim;
  int secondDelim;
  int delim;
  String elements[numOfElements];
  String fromAddress;
  String toAddress;

  
  if (newData == true) {
      String packet(receivedChars);
      packet.toUpperCase(); if (dbg==1)  Serial.println("packet = " + packet);
      packetLen = packet.length();
      firstDelim = packet.indexOf(":");
      secondDelim = packet.indexOf(":", firstDelim + 1);
      elements[0] = packet.substring(0, firstDelim);  //address
      elements[1] = packet.substring(firstDelim + 1, secondDelim); //command 
        if (dbg==1)  Serial.println("cmd = " + elements[1]);
      elements[2] = packet.substring(secondDelim, packetLen);
      // Get the addresses
      fromAddress = elements[0].substring(0,2); if (dbg==1)  Serial.println("fromAddr = " + fromAddress);
      toAddress = elements[0].substring(2,4); if (dbg==1)  Serial.println("toAddr = " + toAddress);  
      // Get the command
      x = elements[1].indexOf(" ");
      cmd =  elements[1].substring(delim, x);  if (dbg==1)  Serial.println("cmd = " + cmd);
      arg = elements[1].substring(x, elements[1].length()); if (dbg==1)  Serial.println("arg = " + arg);
    
    //=== COMMAND TABLE =========================================================
      if (toAddress == AntSwAddress) {
        if (dbg==1)  Serial.println(packetStart + "packet >" + cmd + "<");
        if (cmd.equals("GET")) { send_msg(fromAddress, "SW " + String(currentSwitch)); }
        if (cmd.equals("SET")) { SetSwitch(arg.toInt()); }
        if (cmd.equals("VER")) { send_msg(fromAddress, "Ver " + String(sw_version)); }
      }
      packet = "";
      newData = false;
  }
}
//-------------------------------------------------------------------------------
void send_msg(String toAddress, String payLoad) {
  Serial.println(packetStart + AntSwAddress + toAddress 
    + delimiter + payLoad + delimiter + CRC + packetEnd);
return;
}
