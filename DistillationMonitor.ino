/**************************************************************************
 *                             Adafruit IO
 **************************************************************************/

/************************ Adafruit IO Config ******************************/
#include "AdafruitIO_WiFi.h"
#define IO_USERNAME  "ENGR499Capstone"
#define IO_KEY       "e4059d00f5f44d5b8666d5ac5a42cef2"

/******************************* WIFI *************************************/
#define WIFI_SSID       "capstone"
#define WIFI_PASS       "DistiMon"

/************************ Adafruit IO Instance ****************************/
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *analog = io.feed("Frequency");

/**************************************************************************
 *                          Frequencey Counter
 **************************************************************************/

/***************************** Definitions ********************************/
extern "C" { 
  #include "user_interface.h"
  }
#define stdio.h
#define types.h

/****************************** Constants *********************************/
#define NUM_US_PER_SEC (1000000)
#define TIMER_MS (5000)                  // interrupt timer of 2ms kills wifi Connection
#define PULSE_IN_TIMEOUT_US (1*NUM_US_PER_SEC) // numMs used to calculate frequency
#define FREQUENCY_INPUT_PIN (5)
#define LED_PIN (2)                            //Green=12 blue = 13

/****************************** Variables *********************************/
os_timer_t frequencyTimer;
bool frequencyAvailable;
float pulseDuration;
int calculatedFrequency;    //can be int?
bool pinValue;
int current = 0;
int last = -1;

/**************************************************************************
 *                              Serial LCD
 **************************************************************************/
 
/***************************** Definition *********************************/
//#include <SoftwareSerial.h>
//
///****************************** Instance **********************************/
//SoftwareSerial lcd(SW_SERIAL_UNUSED_PIN, 4);  //RX, TX


/**************************************************************************
 *                                  Setup 
 **************************************************************************/

void setup() {

  Serial.begin(115200);
  delay(100);
  //while(!Serial);   // wait for serial monitor to open

/**************************************************************************
 *                              Serial LCD
 **************************************************************************/
//  lcd.begin(9600);
//  delay(500);
//  lcdClear();

/**************************************************************************
 *                             Adafruit IO
 **************************************************************************/
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
//  lcd.write("Connecting:     ");
//  lcdCursorLineTwo();
//  lcd.write("Adafruit IO     ");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
//  lcdClear();
//  lcd.write("Connected:      ");
//  lcdCursorLineTwo();
//  lcd.write("Adafruit IO     ");
  delay(1500);

/**************************************************************************
 *                          Frequency Counter
 **************************************************************************/  
  Serial.println("Initializing");   
//  lcdClear();                           //clear lcd screen
//  lcd.write("Initializing    ");            //write to lcd line one
//  lcdCursorLineTwo();                   //set lcd cursor to start of line two
//  lcd.write("Freq Counter    ");            //write to lcd line two
  frequencyAvailable = false;           //initialize frequency variables 
  calculatedFrequency = 0;              //
  TimerSetup();                         //
  pinMode(FREQUENCY_INPUT_PIN, INPUT);  //declare frequency input pin as input
//  lcdClear();                           //clear lcd screen
//  lcd.write("Initialized     ");            //write to lcd line one
//  lcdCursorLineTwo();                   //set lcd cursor to start of line two
//  lcd.write("Freq Counter    ");             //write to lcd line one
  delay(1500);                          //delay for readability
//  lcdClear();                           //clear lcd
//  lcd.write("Freq(Hz):       ");            //write to lcd line one
}

char freqstring[10];


/**************************************************************************
 *                                  Loop
 **************************************************************************/

void loop() {
  io.run();                               //io.run(); is required at the top of loop()
  if(frequencyAvailable){
    analog->save(calculatedFrequency);    //send current value to AdafruitIO feed
    frequencyAvailable = false;           //reset frequencyAvailable flag
    sprintf(freqstring, "%7d", calculatedFrequency);//format frequency for lcd display
//    lcdClear();                           //clear lcd
//    lcd.write("Freq(Hz):       ");            //write to lcd line one
//    lcd.write(254);
//    lcd.write(201);                     //set cursor to tenth position of line two
//    lcd.write(freqstring);              //write current freq value to line two    
    delay(5000);
  }
}

void calculateFrequency(void *pArg)
{
  Serial.println("Timer Elapesed");
  pulseDuration = pulseIn(FREQUENCY_INPUT_PIN, HIGH); //TODO: add time out?
  Serial.println(pulseDuration);
  Serial.printf("Pulse Length seconds = %.3f \r\n", pulseDuration); 
  if(pulseDuration && frequencyAvailable == false)
  {
    calculatedFrequency = NUM_US_PER_SEC/pulseDuration;
    frequencyAvailable = true;
  }
  else
  {
    Serial.println("PulseIn timed out");
    Serial.printf("PulseDuration: %.3f", pulseDuration);
    Serial.println(frequencyAvailable ? "TRUE" : "FALSE");
  }
}

void TimerSetup(void)
{
  bool timerRepeat = true;
  os_timer_setfn(&frequencyTimer, calculateFrequency, NULL);
  os_timer_arm(&frequencyTimer,TIMER_MS, timerRepeat);
}

//void lcdClear(){
//  lcd.write(254);
//  lcd.write(128);
//  lcd.write("                ");
//  lcd.write("                ");
//  lcd.write(254);
//  lcd.write(128);
//}
//
//void lcdCursorLineOne(){
//  lcd.write(254);
//  lcd.write(128);
//}
//
//void lcdCursorLineTwo(){
//  lcd.write(254);
//  lcd.write(192);
//}
//
//void lcdClearLineOne(){
//  lcd.write(254);
//  lcd.write(128);
//  lcd.write("                ");
//  lcd.write(254);
//  lcd.write(128);
//}
//
//void lcdClearLineTwo(){
//  lcd.write(254);
//  lcd.write(192);
//  lcd.write("                ");
//  lcd.write(254);
//  lcd.write(192);
//}

/* write 254 to lcd to set cursor, then write cursor position code
   * pos       1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16   
   * Line1   128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143
   * Line2   192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207
   */
