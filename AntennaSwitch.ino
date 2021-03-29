
// ==============================================================================
// ==============================================================================
// Title:  Antenna Switch
// Author:  Marty Squicciarini NR3Z
// Based on BenchDuino Example 
   String sw_version = "1.0";
   int dbg = 0;
   const String AntSwAddress = "69";
   const String CRC = "XX";  //Until I need to calculate CRC just dummy data
// ==============================================================================
// Pin assignments for buttons
// ==============================================================================
  const int NumOfButtons = 4;
  const int button[] = {5, 6, 7, 8};
  int buttonVal[NumOfButtons];
// ==============================================================================
// Pin assignments for LEDs
// ============================================================================== 
  const int led[] = {9, 10, 11, 12}; 
// ==============================================================================
// Pin assignment for relay
// ==============================================================================
  const int relay[] = {23, 24, 25, 26};
// ==============================================================================
// Define Menu System text and control variables
// ==============================================================================
// LCD Menu array is 5 different windows with 2 lines.  Addressing starts at 0.
  int maxmenu = 5;
  String menu[6][2] {    
    {"Welcome         ","Ant Switch NR3Z "},  // Menu item #0 - Welcome splash screen
    {"Hex beam        ","Ant Switch NR3Z "},  // Menu item #1 - Demo items
    {"OCF dipole      ","Ant Switch NR3Z "},  // Menu item #2 - Demo items
    {"EF wire         ","Ant Switch NR3Z "},  // Menu item #3 - Demo items  
    {"Future          ","Ant Switch NR3Z "}}; // Menu item #4 - Demo items          
    int currentSwitch = 0;  

// ==============================================================================
// Define control head operating parameters
// ==============================================================================
  int button_debounce_time = 500;
// ==============================================================================
// Define global variables
// ==============================================================================
  const byte numChars = 101;
  char receivedChars[numChars];
  boolean newData = false;
// ==============================================================================
// Import libraries
// ==============================================================================
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
// ==============================================================================
// Device address assignments
// ==============================================================================
  int lcd_addr    = 0x27;  // LCD address.  Typicaly 0x27 or 0x3F
// ==============================================================================
// Initialize displays 
// ==============================================================================
//MFS//  LiquidCrystal_I2C  lcd(lcd_addr,2,1,0,4,5,6,7);
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// ==============================================================================
// ==============================================================================
// ===== SETUP =====
// ==============================================================================
// ==============================================================================
void setup() {
  Serial.begin(9600);
// ==============================================================================
// Set pin modes
// ==============================================================================
  for(int idx=0; idx < NumOfButtons; ++idx) {
    pinMode(button[idx], INPUT);
    pinMode(led[idx], OUTPUT);
    pinMode(relay[idx], OUTPUT);
    digitalWrite(relay[idx], HIGH); 
  }

  send_msg(AntSwAddress, "Ver " + String(sw_version));

//MFS//  lcd.setBacklightPin(3,POSITIVE);
  lcd.init();                      // initialize the lcd 
  lcd.setBacklight(HIGH);  
  lcd.begin(16, 2);  
  lcd.clear();
  SetLCD(currentSwitch);  delay(5000);       // SPLASH SCREEN
  currentSwitch = 1;  
  SetLCD(currentSwitch);  // Jump to first screen
  SetLED(currentSwitch);
  SetRelay(currentSwitch);
}
// ==============================================================================
// ==============================================================================
// ===== MAIN LOOP =====
// ==============================================================================
// ==============================================================================
void loop() {
  CheckSerial();
  ProcessCommand();
  CheckSwitches();
}
// ==============================================================================
// ==============================================================================
// ===== SUBROUTINES =====
// ==============================================================================
void CheckSwitches() {
  for(int idx=0; idx < NumOfButtons; ++idx) {
    buttonVal[idx] = digitalRead(button[idx]);
    if (buttonVal[idx] == LOW ) { 
      if (dbg==1)  Serial.println("  idx = " + idx);
      SetSwitch(idx + 1); 
      }
  }
}

// ==============================================================================
//----- LCD SUBROUTINES ---------------------------------------------------------
void SetLCD(int menuItem) {
  lcd.clear();
  lcd.setCursor (0,0);  lcd.print(menu[menuItem][0]);
  lcd.setCursor (0,1);  lcd.print(menu[menuItem][1]);
  }
//----- LED SUBROUTINES ---------------------------------------------------------
  void SetLED(int ant) {
    for(int idx = 0; idx < NumOfButtons; ++idx) {
       digitalWrite(led[idx],LOW);
    }
    digitalWrite(led[ant-1], HIGH); 
  }
//----- Relay SUBROUTINES ---------------------------------------------------------
  void SetRelay(int ant) {
    for(int idx = 0; idx < NumOfButtons; ++idx) {
       digitalWrite(relay[idx],HIGH);
    }
    digitalWrite(relay[ant-1], LOW); 
  }
//------ MENU BUTTON  --------------------------------------------------------
  void SetSwitch(int ant)  { 
    if (ant >= 1 && ant <= NumOfButtons) {
      if (dbg==1) Serial.println("Button: " + String(ant) );
      currentSwitch = ant;
      debounce(currentSwitch);
      lcd.clear();  
      SetLCD(currentSwitch);
      SetLED(currentSwitch);
      SetRelay(currentSwitch);
    }
  }

  void SetSwitch(String toAddress, int ant)  { 
    SetSwitch(ant);
    send_msg(toAddress, "SW " + String(currentSwitch));
    
  }

//-------------------------------------------------------------------
void debounce(int x)  {
  do {delay(button_debounce_time);} while (digitalRead(button[x-1])==LOW);  // Debounce the button
}


// ==============================================================================
// ==============================================================================
// ===== END OF PROGRAM =====
// ==============================================================================
// ==============================================================================
