//--- COMMAND PROCESSING  ----------------------------------------
void ProcessCommand() {
  if (newData == true) {
      String str(receivedChars);
      str.toUpperCase(); if (dbg==1)  Serial.println("str = " + str);
      stringlen = str.length();
      command = str;
      x = command.indexOf(" "); if (dbg==1)  Serial.println("x = " + String(x));
      cmd = command.substring(0,x);
      arg = command.substring(x, command.length()); if (dbg==1)  Serial.println("arg = " + arg);
    
    //=== COMMAND TABLE =========================================================
      if (dbg==1)  Serial.println(Outdata + "Command >" + cmd + "<");
      if (cmd.equals("GET")) { send_msg("SW:" + String(currentSwitch)); }
      if (cmd.equals("SET")) { SetSwitch(arg.toInt()); }
      command = "";
      newData = false;
  }
}

//-------------------------------------------------------------------------------
void send_msg(String outtext) {
  Serial.println(Outdata + outtext);
return;
}
