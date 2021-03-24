
// ==============================================================================
// ==============================================================================
// Title:  Antenna Switch
// Author:  Marty Squicciarini NR3Z
// Based on BenchDuino Example 
           char sw_version[10] = "1.0";
           int dbg = 0;
// ==============================================================================
// ==============================================================================
// Pin assignments for buttons
// ==============================================================================
  const int NumOfButtons = 4;
  const int button[] = {5, 6, 7, 8};
  int buttonVal[NumOfButtons];
// ==============================================================================
// Pin assignments for LEDs
// ============================================================================== 
  const int leds[] = {9, 10, 11, 12}; 
// ==============================================================================
// Pin assignment for relay
// ==============================================================================
  const int Relay[] = {23, 24, 25, 26};
// ==============================================================================
// Define Menu System text and control variables
// ==============================================================================
// LCD Menu array is 5 different windows with 2 lines.  Addressing starts at 0.
  int maxmenu = 5;
  String menu[6][2] {    
    {"Welcome         ","Ant Switch NR3Z "}, // Menu item #0 - Welcome splash screen
    {"Hex beam        ","Line 2 Btn 1    "}, // Menu item #1 - Demo items
    {"OCF dipole      ","Line 2 Btn 2    "}, // Menu item #2 - Demo items
    {"EF wire         ","Line 2 Btn 3    "}, // Menu item #3 - Demo items  
    {"Future          ","Line 2 Btn 4    "}}; // Menu item #4 - Demo items          
    int currentSwitch = 0;  

// ==============================================================================
// Define control head operating parameters
// ==============================================================================
  int button_debounce_time = 500;
// ==============================================================================
// Define global variables
// ==============================================================================
  int idx = 0;
  int x = 0;
  String cmd;
  String arg;
  String str = ""; // create a string
  const byte numChars = 101;
  char receivedChars[numChars];
  boolean newData = false;
  String outdata;
  char boardtype[3];
  int packetlen = 0;
  int stringlen; // String length
  int delimiter1_match = 0;
  int delimiter2_match = 0;
  int fromaddress_match = 0;
  String command;
  int val = 0;
  int my_address_value = 0;
  int lastCount = 0;
  volatile int virtualPosition = 0;
  uint8_t draw_state = 2;
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
  for(idx=0; idx < NumOfButtons; ++idx) {
    pinMode(button[idx], INPUT);
  }
    for(idx=0; idx < NumOfButtons; ++idx) {
    pinMode(leds[idx], OUTPUT);
  }
// ==============================================================================
  Serial.print("STARTUP  Version ");
  Serial.println(sw_version);
  outdata = "/";
//=== LCD SETUP =================================================================
//MFS//  lcd.setBacklightPin(3,POSITIVE);
  lcd.init();                      // initialize the lcd 
  lcd.setBacklight(HIGH);  
  lcd.begin(16, 2);  
  lcd.clear();
  do_update_lcd(currentSwitch);  delay(5000);       // SPLASH SCREEN
  currentSwitch = 1;  
  do_update_lcd(currentSwitch);  // Jump to first screen
  setLEDs(currentSwitch);
}

// ==============================================================================
// ==============================================================================
// ===== MAIN LOOP =====
// ==============================================================================
// ==============================================================================
void loop() 
{
  recvWithStartEndMarkers();
  processpayload();

  for(idx=0; idx < NumOfButtons; ++idx) {
    buttonVal[idx] = digitalRead(button[idx]);
    if (buttonVal[idx] == LOW ) { 
      if (dbg==1)  Serial.println("  idx = " + idx);
      do_button(idx + 1); 
      }
  }
}
// ==============================================================================
// ==============================================================================
// ===== SUBROUTINES =====
// ==============================================================================
// ==============================================================================
//----- LCD SUBROUTINES ---------------------------------------------------------
void do_update_lcd(int menu_item) {
  lcd.setCursor (0,0);  lcd.print(menu[menu_item][0]);
  lcd.setCursor (0,1);  lcd.print(menu[menu_item][1]);
  }
//----- LED SUBROUTINES ---------------------------------------------------------
  void setLEDs(int btn) {
    for(idx = 0; idx < NumOfButtons; ++idx) {
       digitalWrite(leds[idx],LOW);
    }
    digitalWrite(leds[btn-1], HIGH); 
  }

//------ MENU BUTTON  --------------------------------------------------------
  void do_button(int btn)  { 
    if (btn >= 1 && btn <= NumOfButtons) {
      send_msg("Button: " + String(btn) );
      currentSwitch = btn;
      debounce(currentSwitch);
      lcd.clear();  
      do_update_lcd(currentSwitch);
      setLEDs(currentSwitch);
    }
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
