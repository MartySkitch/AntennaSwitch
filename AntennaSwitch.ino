
// ==============================================================================
// ==============================================================================
// Title:  Antenna Switch
// Author:  Marty Squicciarini NR3Z
// Based on BenchDuino Example 
           char sw_version[10] = "1.0";
           int dbg = 0;
// ==============================================================================
// ==============================================================================
// Physical pin assignments
// ==============================================================================
  const int button1 = 5;
  const int button2 = 6;
  const int button3 = 7;
  const int button4 = 8;
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
  unsigned long freq = 7100;
  unsigned long lastFreq = freq;
  unsigned long freqStep = 1;
  int button1_val;  int button2_val;  int button3_val;  int button4_val;
// ==============================================================================
// Define global variables
// ==============================================================================
  int i = 0;
  int j = 0;
  int x = 0;
  int y = 0;
  String cmd;
  String arg;
  int parser_stringlen = 0;
  int parser_strstart = 0;
  int parser_strstop = 0;
  int parser_numargs = 0;
  int parser_argstart[10];
  int parser_argend[10];
  int parser_argnum = 0;
  String str = ""; // create a string
  byte inbyte;
  const byte numChars = 101;
  char receivedChars[numChars];
  boolean newData = false;
  int data =0;
  String outdata;
  String next_arg;
  String payload;
  char boardtype[3];
  int packetlen = 0;
  int stringlen; // String length
  int delimiter1_match = 0;
  int delimiter2_match = 0;
  int fromaddress_match = 0;
  //int goodpacket = 0;
  //int directpacket = 0;
  String command;
//  int diditflag = 1;
  int val = 0;
  int my_address_value = 0;
  int lastCount = 0;
  volatile int virtualPosition = 0;
  uint8_t draw_state = 2;
  float freqfloat = 0;
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
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    pinMode(button4, INPUT);
// ==============================================================================
  Serial.print("STARTUP  Version ");
  Serial.println(sw_version);
  outdata = "/";
  next_arg = "";
  payload = "";
//=== LCD SETUP =================================================================
//MFS//  lcd.setBacklightPin(3,POSITIVE);
  lcd.init();                      // initialize the lcd 
  lcd.setBacklight(HIGH);  
  lcd.begin(16, 2);  
  lcd.clear();
  do_update_lcd(currentSwitch);  delay(5000);       // SPLASH SCREEN
  currentSwitch = 1;  
  do_update_lcd(currentSwitch);  // Jump to first screen
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

  button1_val = digitalRead(button1);  if (button1_val==LOW ) { do_button(1); }
  button2_val = digitalRead(button2);  if (button2_val==LOW ) { do_button(2); }
  button3_val = digitalRead(button3);  if (button3_val==LOW ) { do_button(3); }
  button4_val = digitalRead(button4);  if (button4_val==LOW ) { do_button(4); }
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

//------ MENU BUTTON  --------------------------------------------------------
  void do_button(int btn)  { 
    send_msg("Button: " + String(btn) );
    currentSwitch = btn;
    debounce(currentSwitch);
    lcd.clear();  
    do_update_lcd(currentSwitch);
  }

//-------------------------------------------------------------------
void debounce(int x)  {
    if (x==1) {do {delay(button_debounce_time);} while (digitalRead(button1)==LOW); } // Debounce the button
    if (x==2) {do {delay(button_debounce_time);} while (digitalRead(button2)==LOW); } // Debounce the button
    if (x==3) {do {delay(button_debounce_time);} while (digitalRead(button3)==LOW); } // Debounce the button
    if (x==4) {do {delay(button_debounce_time);} while (digitalRead(button4)==LOW); } // Debounce the button
}

// ==============================================================================
// ==============================================================================
// ===== END OF PROGRAM =====
// ==============================================================================
// ==============================================================================
