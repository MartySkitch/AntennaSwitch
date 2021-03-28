//---------------------------------------------------------------
//--- RECEIVE Serial DATA ---------------------------------------
void CheckSerial() {
  const char cPacketStart = 47; // "/"
  const char cPacketEnd = 13; 
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char rc;
  while (Serial.available() > 0 && newData == false) {
      rc = Serial.read();
      if (dbg==1)  Serial.println(rc);
      if (recvInProgress == true) {
          if (rc != cPacketEnd)  {
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) {
                  ndx = numChars - 1;
              }
          }
          else {
              receivedChars[ndx] = '\0'; // terminate the string
              recvInProgress = false;
              ndx = 0;
              newData = true;
          }
      }
      else if (rc == cPacketStart) {
          recvInProgress = true;
      }
  }
}
