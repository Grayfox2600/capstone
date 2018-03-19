
/**************************************************************************
Distilation Frequency
This sketch uses software timer1 to calculated the output frequency of on a GPIO
Displays on Serial monitor
 **************************************************************************/
 
/**************************************************************************
 *                                  Defines
 **************************************************************************/
extern "C" 
{ 
  #include "user_interface.h"
}

#define stdio.h
#define types.h

/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define NUM_US_PER_SEC (1000000)
#define TIMER_MS (1500) // interrupt timer of 2ms kills wifi Connection
#define PULSE_IN_TIMEOUT_US (1*NUM_US_PER_SEC) // numMs used to calculate frequency
#define FREQUENCY_INPUT_PIN (5)
#define LED_PIN (2) //Green=12 blue = 13
/**************************************************************************
 *                                  Variables
 **************************************************************************/
os_timer_t frequencyTimer;
bool frequencyAvailable;
float pulseDuration;
float calculatedFrequency;
bool pinValue;
/**************************************************************************
 *                                  Setup 
 **************************************************************************/

void setup()
{
 Serial.begin(115200);
 delay(10);
 Serial.println("Initializing"); 
 frequencyAvailable = false;
 calculatedFrequency = 0;
 TimerSetup();
 pinMode(FREQUENCY_INPUT_PIN, INPUT);
 //WifiSetup();
}

void CalculateFrequency(void *pArg)
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
  }
}

void TimerSetup(void)
{
  bool timerRepeat = true;
  os_timer_setfn(&frequencyTimer, CalculateFrequency, NULL);
  os_timer_arm(&frequencyTimer,TIMER_MS, timerRepeat);
}

void WifiSetup(void)
{
/*
  const char* ssid ="ubcvisitor";
  const char* password ="";
  
  const char* host = "192.168.0.153";
  const char* streamId   = "....................";
  const char* privateKey = "....................";
 */  
}

/**************************************************************************
 *                                  Main
 **************************************************************************/
void loop()
{
  
  if(frequencyAvailable)
  {
    Serial.printf("Frequency = %.3f \r\n", calculatedFrequency);
    frequencyAvailable = false;
    //send frequency out via WiFi
  }

}
