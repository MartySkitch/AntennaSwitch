
//--- COMMAND PROCESSING  ----------------------------------------
void processpayload() {
  if (newData == true) {
  String str(receivedChars);
  str.toUpperCase();
  stringlen = str.length();
  command = str;
  parser_stringlen = command.length();
  x = command.indexOf(",");
  cmd = command.substring(0,x);
  // Determine how many arguments there are and store it in numargs
  parser_numargs = 0;
  for (int i=0;i<=parser_stringlen;i++) {
    if (command.substring(i,i+1).equals(",")) {
      parser_numargs++;
      parser_argstart[parser_numargs] = i+1;
      parser_argend[parser_numargs-1] = i;
    }
  }

  for (int i=1;i<=parser_numargs-1;i++) {
    x=i;
    arg[i] = command.substring(parser_argstart[i],parser_argend[i]);
  }

    x++;
    arg[x] = command.substring(command.lastIndexOf(",")+1);

//=== COMMAND TABLE =========================================================
  if (dbg==1)  Serial.println("Command >" + command + "<");
  if (cmd.equals("PING")) { diditflag=1; send_msg("PING!");}
  if (cmd.equals("LCD"))   { do_set_lcd(); }
  if (cmd.equals("MENU"))  { do_set_menu(); }
  
  if (diditflag == 1) {
  //--- ASSEMBLE PAYLOAD --------------------------------------------
  payload = "-";
  //--- ASSEMBLE PACKET ---------------------------------------------
  outdata = payload;                  // Start of packet
  //--- SENT PACKET -------------------------------------------------
  send_msg(outdata);
  digitalWrite(statuspin,HIGH);
  delay (250);
  digitalWrite(statuspin,LOW);
  diditflag = 0;
  }
  diditflag = 0;
  command = "";
  goodpacket = 0;
  directpacket = 0;
  newData = false;
    }
}

//-------------------------------------------------------------------
void do_set_lcd() {
  // command format:  LCD,1,2,TEXT  LCD,Line number 1..2, character position 1..16, text
  if (dbg==1)  Serial.println("arg 1 = " + arg[1] + ", arg 2 = " + arg[2]);
  lcd.setCursor (arg[2].toInt()-1,arg[1].toInt()-1);
  lcd.print(arg[3]);
}

//-------------------------------------------------------------------
void do_set_menu() {
  // menu[A][B]  A=Slot, B=Row
  if (arg[1]=="1" and arg[2]=="1") {menu[1][2]=arg[3];}
  if (arg[1]=="2" and arg[2]=="2") {menu[1][2]=arg[3];}
  do_update_lcd(currentSwitch);
}

//-------------------------------------------------------------------------------
void send_msg(String outtext) {
  Serial.println(outtext);
return;
}
