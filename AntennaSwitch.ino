
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
const int button[] = {4, 5, 6, 7};
int buttonVal[NumOfButtons];
int currentSwitch = 0;
int button_debounce_time = 500;
volatile int newSwitch = 0;   //The interrupt will update this 

// ==============================================================================
// Pin assignments for LEDs
// ==============================================================================
const int led[] = {0, 1, 2, 3};

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
  // ==============================================================================
  // Set pin modes
  // ==============================================================================
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    pinMode(button[idx], INPUT_PULLUP);
    pinMode(led[idx], OUTPUT);
    pinMode(relay[idx], OUTPUT);
    digitalWrite(relay[idx], HIGH);
    attachInterrupt(digitalPinToInterrupt(button[idx]), CheckSwitches, LOW);
  }
  pinMode(connectLED, OUTPUT);
  currentSwitch = 1;
  SetLED(currentSwitch);
  SetRelay(currentSwitch);
  
  // check for the WiFi module:
  digitalWrite(connectLED, LOW);
  if (WiFi.status() == WL_NO_MODULE) {
    // don't continue
    while (true);
  }
  WiFi.begin(ssid, pass);
  client.begin(mqttServer, mqttServerPort, net);
  client.onMessage(messageReceived);
  connect();
}

// ==============================================================================
//  MAIN LOOP
// ==============================================================================
void loop() {
  client.loop();
  // delayMicroseconds(1000); // helps eventually
  if (!client.connected()) {
    digitalWrite(connectLED, LOW);
    connect();
  }
  //  Need to get Mqtt message
    SetSwitch();
}
// ==============================================================================
// ===== SUBROUTINES =====
// ==============================================================================
void CheckSwitches() {
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    buttonVal[idx] = digitalRead(button[idx]);
    if (buttonVal[idx] == LOW ) {
      newSwitch = idx + 1;
//      SetSwitch(idx + 1);
//      debounce(idx + 1);
    }
  }
}

// ==============================================================================
//----- LED SUBROUTINES ---------------------------------------------------------
void SetLED(int swtch) {
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    digitalWrite(led[idx], LOW);
  }
  digitalWrite(led[swtch - 1], HIGH);
}
//----- Relay SUBROUTINES ---------------------------------------------------------
void SetRelay(int swtch) {
  for (int idx = 0; idx < NumOfButtons; ++idx) {
    digitalWrite(relay[idx], HIGH);
  }
  digitalWrite(relay[swtch - 1], LOW);
}
//------ BUTTONS  --------------------------------------------------------
void SetSwitch()  {
  if (currentSwitch != newSwitch) {
    currentSwitch = newSwitch;
    SetLED(currentSwitch);
    SetRelay(currentSwitch);
    send_msg(SWITCH, String(currentSwitch));
  }
}

void debounce(int x)  {
  do {
    delayMicroseconds(button_debounce_time);
  } while (digitalRead(button[x - 1]) == LOW); // Debounce the button
}

//------ WiFi MQTT  --------------------------------------------------------
void connect() {
  // attempt to connect to WiFi network:
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    digitalWrite(connectLED, LOW);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delayMicroseconds(10000);
  }
  while (!client.connect(device, key, secret)) {
    delayMicroseconds(1000);
  }
  digitalWrite(connectLED, HIGH);
  client.subscribe(TOPIC_SUB);
}

// ==============================================================================
// ===== END OF PROGRAM =====
// ==============================================================================
