//---------------------------------------------------------------
//--- RECEIVE Serial DATA ---------------------------------------
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '/';
    char endMarker1 = 10;
    char endMarker2 = 13;
    char rc;
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        //if (dbg==1)  Serial.println(rc);
        if (recvInProgress == true) {
            if (rc != endMarker1 and rc != endMarker2)  {
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
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}
