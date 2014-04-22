/*

 Mimics the fade example but with an extra parameter for frequency. It should dim but with a flicker 
 because the frequency has been set low enough for the human eye to detect. This flicker is easiest to see when 
 the LED is moving with respect to the eye and when it is between about 20% - 60% brighness. The library 
 allows for a frequency range from 1Hz - 2MHz on 16 bit timers and 31Hz - 2 MHz on 8 bit timers. When 
 SetPinFrequency()/SetPinFrequencySafe() is called, a bool is returned which can be tested to verify the 
 frequency was actually changed.
 
 This example runs on mega and uno.
 */
#include <AH_Pololu.h>
//#include <PWM.h>
#include <VirtualWire.h>

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP, int ENABLE, int RESET);
AH_Pololu stepper(200,9,10,4,5,10,7,8,9);   // init with all functions
//AH_Pololu stepper(200,2,3);

//use pin 11 on the Mega instead, otherwise there is a frequency cap at 31 Hz
const int led_pin = 13;
const int receive_pin = 12;
const int endstop = 2;
//const int transmit_pin = 12;
//const int transmit_en_pin = 7;
int led = 3;  
int led2 = 11;
int led3 = 6;// the pin that the LED is attached to

         // how bright the LED is


int ledlow = 15;
int ledhigh = 110;
int ledpwm = 0;
int ledpwm2 = 0;
int ledpwm3 = 0;

int stepspeed = 800;
int steprange = 28000;
int movepwm = 110;

int rmtfocus = 0;
int focus = 0;
int focdiff = 0;

void setup()
{
  Serial.begin(9600);	// Debugging only
  
  attachInterrupt(0, zeropos, FALLING);
  
  
  
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(4000);
  vw_rx_start();
  pinMode(led_pin, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  analogWrite(led , 0);
  analogWrite(led2 , 0);
  analogWrite(led3 , 0);
  pinMode(endstop, INPUT);
  digitalWrite(endstop, HIGH);
  
  TCCR2B = TCCR2B & 0b11111000 | 0x01;
  TCCR0B = TCCR0B & 0b11111000 | 0x02;
  
 
 stepper.setSpeedRPM(stepspeed);
// //Stepper

    if (digitalRead(endstop) == 0){
    analogWrite(led, movepwm);
    stepper.enableDriver(); 
    
    stepper.move(1000);
    }
    
    while (digitalRead(endstop) == 1){
    analogWrite(led, movepwm);
    stepper.enableDriver(); 
    
    stepper.move(-1);
  //delay(1000);  
    }

 stepper.disableDriver();   
 analogWrite(led, ledpwm);
 analogWrite(led, ledpwm2);
 analogWrite(led, ledpwm3);
//  
  
  
  
  //initialize all timers except for 0, to save time keeping functions
//  InitTimersSafe(); 
//
//  //sets the frequency for the specified pin
//  bool success = SetPinFrequencySafe(led, frequency);
//  
//  //if the pin frequency was set successfully, turn pin 13 on
//  if(success) {
//    pinMode(13, OUTPUT);
//    digitalWrite(13, HIGH);    
//  }
}


void zeropos(){
  
  focus = 0;
  rmtfocus = 0;
  //stepper.disableDriver();
  stepper.resetDriver();   
  stepper.disableDriver();
 
  }
  

void chngpos(){
  if (ledpwm > movepwm) analogWrite(led, movepwm);
  if (ledpwm2 > movepwm) analogWrite(led2, movepwm);
  if (ledpwm3 > movepwm) analogWrite(led3, movepwm);
  focdiff = rmtfocus - focus;
  stepper.enableDriver();       
  stepper.move(focdiff);
  stepper.disableDriver();
  focus = rmtfocus;  
  analogWrite(led, ledpwm);
  analogWrite(led2, ledpwm2);
  analogWrite(led3, ledpwm3);
}
void loop()
{
  
//  //use this functions instead of analogWrite on 'initialized' pins
//  //if (brightness >= high - fadeAmount) brightness = 255;
//  pwmWrite(led, brightness);
//
//  brightness = brightness + fadeAmount;
//
////   if (brightness >= high) {
////   brightness = low; 
//    
//  if (brightness == low || brightness == high - fadeAmount) {
//  fadeAmount = -fadeAmount ; 
//  }     
  
 
  //Serial.print(analogRead(endstop));
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;

        digitalWrite(led_pin, HIGH);
       delay(10); // Flash a light to show received good message
	// Message with a good checksum received, dump it.
//	Serial.print("Got: ");
//	
//	
//	    Serial.print(buf[0], HEX);
//	    Serial.print(' ');
//	    Serial.print(buf[1], HEX);
//
//	Serial.println();
        digitalWrite(led_pin, LOW);
        
        ledpwm = map(buf[0], -1, 100, ledlow, ledhigh);
        if (ledpwm >= ledhigh) ledpwm = 255;
        if (ledpwm <= ledlow) ledpwm = 0;
        analogWrite(led, ledpwm);
        
        ledpwm2 = map(buf[1], -1, 100, ledlow, ledhigh);
        if (ledpwm2 >= ledhigh) ledpwm2 = 255;
        if (ledpwm2 <= ledlow) ledpwm2 = 0;
        analogWrite(led2, ledpwm2);
        
        ledpwm3 = map(buf[2], -1, 100, ledlow, ledhigh);
        if (ledpwm3 >= ledhigh) ledpwm3 = 255;
        if (ledpwm3 <= ledlow) ledpwm3 = 0;
        analogWrite(led3, ledpwm3);
        
        rmtfocus = map(buf[3], 0, 100, 0, steprange);
        
        Serial.print(buf[0]);
        Serial.print(' ');
        Serial.print(ledpwm);
        Serial.print(' ');
        Serial.print(buf[2]);
        Serial.print(' ');
        Serial.println(buf[3]);
        
        
        
        
        
    }
    
    if (focus != rmtfocus) chngpos();
//  Serial.println(focus);
}

