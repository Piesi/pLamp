
/********************************************************
** Download from:                                      **
** http://www.alhin.de/arduino/index.php?n=48          **
********************************************************/

#include <VirtualWire.h>



unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 12;  // pin 12
const int pin_B = 8;  // pin 11
const int b1 = 2;  // pin 12

long interval = 1000; 
long previousMillis = 0;

unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
unsigned char encoder_B_prev=0;


int led1 = 4;
int led2 = 5;
int led3 = 6;
int led4 = 7;

int debounce = 0;

int gang = 0;
int achse = 0;
int spd = 30;
int pwmmulti = 10;
int pwmvalue = 0;
int pwmvalue2 = 0;
int pwmvalue3 = 0;
int para = 0;

int focusmulti = 10;
int focusvalue = 0;

const int transmit_pin = 10;

int incomingByte; 

//const int transmit_en_pin = 3;



void setup() {
  
  Serial.begin(57600);  
 
  pinMode(b1, INPUT);
  digitalWrite(b1, HIGH);
  
  //attachInterrupt(1, changepara, FALLING);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  digitalWrite(pin_A, HIGH);
  digitalWrite(pin_B, HIGH);
  currentTime = millis();
  loopTime = currentTime;
  
  vw_set_tx_pin(transmit_pin);
    
 //   vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(4000);       // Bits per sec
   
  
 byte count = 1;
}

void changepara(){
  para = para + 1;
  if (para > 4) para = 0;
  
  if (para == 0) {
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  }
  if (para == 1) {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, LOW);
  }
  if (para == 2) {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, HIGH);
  }
  if (para == 3) {
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  }
  if (para == 4) {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  }
delay(50);
}

void chval(int amnt){
  if (para == 0){
  pwmvalue = pwmvalue + (amnt * pwmmulti);
  if (pwmvalue == pwmmulti) pwmvalue = 1; 
  pwmvalue = constrain(pwmvalue, 0, 100);
    }

if (para == 1){
  pwmvalue2 = pwmvalue2 + (amnt * pwmmulti);
  if (pwmvalue2 == pwmmulti) pwmvalue2 = 1; 
  pwmvalue2 = constrain(pwmvalue2, 0, 100);
    } 
   
 if (para == 2){
  pwmvalue3 = pwmvalue3 + (amnt * pwmmulti);
  if (pwmvalue3 == pwmmulti) pwmvalue3 = 1; 
  pwmvalue3 = constrain(pwmvalue3, 0, 100);
    }  
    
  if (para == 3){
  
  pwmvalue = pwmvalue + (amnt * pwmmulti);
  if (pwmvalue == pwmmulti) pwmvalue = 1; 
  pwmvalue = constrain(pwmvalue, 0, 100);  
    
  pwmvalue2 = pwmvalue2 + (amnt * pwmmulti);
  if (pwmvalue2 == pwmmulti) pwmvalue2 = 1; 
  pwmvalue2 = constrain(pwmvalue2, 0, 100);
    
  pwmvalue3 = pwmvalue3 + (amnt * pwmmulti);
  if (pwmvalue3 == pwmmulti) pwmvalue3 = 1; 
  pwmvalue3 = constrain(pwmvalue3, 0, 100);
    }  
    
    
     
  if (para == 4){
  focusvalue = focusvalue + (amnt * focusmulti);
  focusvalue = constrain(focusvalue, 0, 100);
 }
 
//  Serial.print(pwmvalue);
//  Serial.print(' ');
//  Serial.print(pwmvalue2);
//  Serial.print(' ');
//  Serial.print(pwmvalue3);
//  Serial.print(' ');
//  Serial.println(focusvalue);
  sendlamp();
}

void sendlamp(){
  char msg[4] = {pwmvalue, pwmvalue2, pwmvalue3, focusvalue};
  vw_send((uint8_t *)msg, 4);
  vw_wait_tx(); // Wait until the whole message is gone
}
  
void loop() {
// get the current elapsed time
  
   if (Serial.available() > 0) {

    incomingByte = Serial.read();

    if (incomingByte < 64)
    {
      pwmvalue = map(incomingByte, 0, 63, 0, 100);
    } 
    if ((incomingByte >= 64) && (incomingByte < 128))
    {
      pwmvalue2 = map(incomingByte, 64, 127, 0, 100);
    } 
    if ((incomingByte >= 128) && (incomingByte < 192))
    {
      pwmvalue3 = map(incomingByte, 128, 191, 0, 100);
    } 
     if ((incomingByte >= 192) && (incomingByte < 256))
    {
      focusvalue = map(incomingByte, 192, 255, 0, 100);
    } 

    sendlamp();
  }
  
  
  if ((digitalRead(b1)) == LOW && debounce == 0){
    changepara();
    debounce = 1;
    }
  if ((digitalRead(b1)) == HIGH){
    debounce = 0;
    }
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
  
  sendlamp();

  }
  
  
  currentTime = millis();
  if(currentTime >= (loopTime + 1)){
    // 5ms since last check of encoder = 200Hz  
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      if(encoder_B) {
        // B is high so clockwise
        // increase the brightness, dont go over 255
        chval(1); 
           
      }   
      else {
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
        
        chval(-1);       
      }   

    }   
    
    if((encoder_B) && (!encoder_B_prev)){
      // A has gone from high to low 
      if(!encoder_A) {
        // B is high so clockwise
        // increase the brightness, dont go over 255
        chval(-1); 
          
      }   
      else {
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
       
        chval(1);       
      }   

    }   
    
    
    encoder_A_prev = encoder_A;     // Store value of A for next time    
     encoder_B_prev = encoder_B;     // Store value of A for next time    
    
 
   
    loopTime = currentTime;  // Updates loopTime
  }
  
}


