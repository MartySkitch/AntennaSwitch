
//--- COMMAND PROCESSING  ----------------------------------------
void processpayload() {
  if (newData == true) {
      String str(receivedChars);
      str.toUpperCase();
      if (dbg==1)  Serial.println("str = " + str);
      stringlen = str.length();
      command = str;
    //  parser_stringlen = command.length();
      x = command.indexOf(" ");
      if (dbg==1)  Serial.println("x = " + String(x));
      cmd = command.substring(0,x);
      arg = command.substring(x, command.length());
      if (dbg==1)  Serial.println("arg = " + arg);
    
    //=== COMMAND TABLE =========================================================
      if (dbg==1)  Serial.println("Command >" + cmd + "<");
      if (cmd.equals("GET")) { send_msg("SW:" + String(currentSwitch));
        }
      if (cmd.equals("SET")) { do_button(arg.toInt()); }
      command = "";
      newData = false;
  }
}

//-------------------------------------------------------------------------------
void send_msg(String outtext) {
  Serial.println(outtext);
return;
}
