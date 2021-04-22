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
int dbg = 0;
WiFiClient net;
MQTTClient client;

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

// =============================================================================
// SETUP
// ==============================================================================
void setup() {
  //Initialize serial and wait for port to open only when debugging
  if (dbg == 1) {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
      Serial.println("Serial port OK");
    }
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    if (dbg == 1) Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
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
    connect();
  }
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
      if (dbg == 1)  Serial.println("  idx = " + String(idx));
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
    if (dbg == 1) Serial.println("Button: " + String(ant) );
    currentSwitch = ant;
    debounce(currentSwitch);
    SetLED(currentSwitch);
    SetRelay(currentSwitch);
  }
}
void SetSwitch(String toAddress, int ant)  {
  SetSwitch(ant);
  send_msg("SW", String(currentSwitch));
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
    if (dbg == 1) Serial.print("Attempting to connect to SSID: ");
    if (dbg == 1) Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  if (dbg == 1)Serial.println("Connected to WiFi");
  if (dbg == 1)Serial.println("connecting to broker...");
  while (!client.connect(device, key, secret)) {
    if (dbg == 1) Serial.print(".");
    delay(1000);
  }

  if (dbg == 1) Serial.println("Connected to MQTT");

  client.subscribe(TOPIC_SUB);
}

// ==============================================================================
// ===== END OF PROGRAM =====
// ==============================================================================
