// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

const int led_pin = 11;
const int transmit_pin = 10;
const int receive_pin = 2;
const int transmit_en_pin = 3;

int pwmvalue = 0;
int pwmvalue2 = 0;
int pwmvalue3 = 0;
int focusvalue = 0;

int incomingByte;  

void setup()
{
 Serial.begin(57600);  
  // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(4000);       // Bits per sec
    pinMode(led_pin, OUTPUT);
}

byte count = 1;

void loop()
{
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

    char msg[4] = {pwmvalue, pwmvalue2, pwmvalue3, focusvalue};

  
  vw_send((uint8_t *)msg, 4);
  vw_wait_tx(); // Wait until the whole message is gone
  }
  
}
