// ==============================================================================
// Title:  Antenna Switch
// Author:  Marty Squicciarini NR3Z
// Based on BenchDuino Example

// =============================================================================
// Import libraries
// ==============================================================================
#include <SPI.h>
#include <WiFiNINA.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include "Secrets.h"

// ==============================================================================
// Define global variables
// =============================================================================
String sw_version = "1.0";
int dbg = 1;
WiFiClient net;
MQTTClient client;

#define TOPIC_SUB "Ham/AntSwitch/Cmd"
#define TOPIC_PUB "Ham/AntSwitch/Resp"
#define SWITCH "SWITCH"

// ==============================================================================
// Pin assignments for buttons
// ==============================================================================
const int NumOfButtons = 4;
const int button[] = {0, 1, 2, 3};
int buttonVal[NumOfButtons];
int currentSwitch = 0;
int button_debounce_time = 500;

// ==============================================================================
// Pin assignments for LEDs
// ==============================================================================
const int led[] = {4, 5, 6, 7};

// ==============================================================================
// Pin assignment for relay
// ==============================================================================
const int relay[] = {8, 9, 10, 11};

// ==============================================================================
const int connectLED = 12; 

// =============================================================================
// SETUP
// ==============================================================================
void setup() {
  // check for the WiFi module:
  digitalWrite(connectLED, LOW);
  if (WiFi.status() == WL_NO_MODULE) {
    // don't continue
    while (true);
  }
  digitalWrite(connectLED, HIGH);
  WiFi.begin(ssid, pass);
  client.begin(mqttServer, mqttServerPort, net);
  client.onMessage(messageReceived);
  connect();

  // ==============================================================================
  // Set pin modes
  // ==============================================================================
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    pinMode(button[idx], INPUT_PULLUP);
    pinMode(led[idx], OUTPUT);
    pinMode(relay[idx], OUTPUT);
    digitalWrite(relay[idx], HIGH);
  }
  pinMode(connectLED, OUTPUT);
  currentSwitch = 1;
  SetLED(currentSwitch);
  SetRelay(currentSwitch);
}

// ==============================================================================
//  MAIN LOOP
// ==============================================================================
void loop() {
  client.loop();
  // delay(1000); // helps eventually
  if (!client.connected()) {
    digitalWrite(connectLED, LOW);
    connect();
  }
  digitalWrite(connectLED, HIGH);  
  //  Need to get Mqtt message
  CheckSwitches();
}
// ==============================================================================
// ===== SUBROUTINES =====
// ==============================================================================
void CheckSwitches() {
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    buttonVal[idx] = digitalRead(button[idx]);
    if (buttonVal[idx] == LOW ) {
      SetSwitch(idx + 1);
    }
  }
}

// ==============================================================================
//----- LED SUBROUTINES ---------------------------------------------------------
void SetLED(int ant) {
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    digitalWrite(led[idx], LOW);
  }
  digitalWrite(led[ant - 1], HIGH);
}
//----- Relay SUBROUTINES ---------------------------------------------------------
void SetRelay(int ant) {
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    digitalWrite(relay[idx], HIGH);
  }
  digitalWrite(relay[ant - 1], LOW);
}
//------ BUTTONS  --------------------------------------------------------
void SetSwitch(int ant)  {
  if (ant >= 1 && ant <= NumOfButtons) {
    currentSwitch = ant;
    debounce(currentSwitch);
    SetLED(currentSwitch);
    SetRelay(currentSwitch);
    send_msg(SWITCH, String(currentSwitch));
  }
}

void debounce(int x)  {
  do {
    delay(button_debounce_time);
  } while (digitalRead(button[x - 1]) == LOW); // Debounce the button
}

//------ WiFi MQTT  --------------------------------------------------------
void connect() {
  // attempt to connect to WiFi network:
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  while (!client.connect(device, key, secret)) {
    delay(1000);
  }

  client.subscribe(TOPIC_SUB);
}

// ==============================================================================
// ===== END OF PROGRAM =====
// ==============================================================================
