/*
 This code is based on the RF24 library from maniac bug.
 http://maniacbug.github.io/RF24/index.html


 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Defining pins for arduino
// 3,5,6,9,10,11 PWM
// OUTPUT PINS
int led_personal  = 3;
int led_channel_A = 5;
int led_channel_B = 6;
int led_channel_C = 9;

// INPUT PINS
int pulse_input  = A0;
int dip_switch_1 = A1;
int dip_switch_2 = A2;
int dip_switch_3 = A3;
int dip_switch_4 = A4;

// PRERESERVED PINS FOR NRF24L01 ADAPTER
int CE = 8; // std conf for adapter
int CS = 7; // std conf for adapter

int outputs[] = {
  led_personal, 
  led_channel_A,
  led_channel_B,
  led_channel_C
};

int inputs[] = {
  dip_switch_1,
  dip_switch_2,
  dip_switch_3,
  dip_switch_4,
  pulse_input
};

int outputs_count = sizeof(outputs)/sizeof(int);
int inputs_count = sizeof(inputs)/sizeof(int);


RF24 radio(CE,CS); // CE, CS
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[4] = { 
	0xF0F0F0F0E1LL, 
	0xF0F0F0F0D2LL,
	0xF0F0F0F0C3LL, 
	0xF0F0F0F0B4LL
};

void setup() {                
  for (int i=0; i<outputs_count; i++) {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i],  HIGH);  
  }
  for (int i=0; i<inputs_count; i++) {
    pinMode(inputs[i], INPUT);
  }
  initialize();

  radio.begin();
  radio.setRetries(15,15);
  int channel = get_channel();
  int other = 1;
  radio.openWritingPipe(pipes[channel]);

  for (int i=0; i<4; i++) {
  	if (i != channel) radio.openReadingPipe(other,pipes[other++]);
  }

  radio.startListening();
}

void loop() {
  	radio.stopListening();
	int reading;
	reading = analogRead(A0);
	radio.write( &reading, sizeof(int) );
	delay(1000);
	radio.startListening();


  for (int i=0; i<4; i++) {
    int temp = digitalRead(inputs[i]);
    digitalWrite(outputs[i], temp);
  }
  delay(200);
}

void initialize () {
  fader(200);
  fader(100);
  blink(200);
}

void fader (int ms) {
  for (int i=0; i<outputs_count; i++) {
    digitalWrite(outputs[i],  HIGH);
    delay(ms);
    digitalWrite(outputs[i],  LOW);
  }
}

void blink (int ms) {
  for (int i=0; i<outputs_count; i++) digitalWrite(outputs[i],  HIGH);
  delay(ms);
  for (int i=0; i<outputs_count; i++) digitalWrite(outputs[i],  LOW);
  delay(ms);
}

int get_channel () {
	for (int i=0; i<inputs_count; i++) {
		int val = digitalRead(inputs[i]);
		if (val == HIGH) return i;
	}
}