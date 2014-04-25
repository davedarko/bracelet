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

int led_channel[4]; // index is pipe, value is led

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

  radio.begin();
  radio.setRetries(15,15);
  initialize();  
  radio.startListening();

}

void loop() {
  	uint8_t pipe_num;
    unsigned long started_waiting_at = millis();
    bool timeout = false;

    radio.startListening();

    while ( ! radio.available(&pipe_num) && ! timeout ) {
    	if (millis() - started_waiting_at > 250 ) timeout = true;	
    }
    if ( !timeout ) {
    	int value;
        radio.read( &value, sizeof(int) );
        set_LED(pipe_num, value);
	}

	radio.stopListening();
	unsigned long reading;
	reading = analogRead(A0);
	radio.write( &reading, sizeof(int));
	radio.startListening();
}

int get_channel () {
	for (int i=0; i<inputs_count; i++) {
		int val = digitalRead(inputs[i]);
		if (val == HIGH) return i;
	}
}

void set_LED (uint8_t pipe_num, int value) {
	int channel = get_channel();
	int other = 1;
	for (int i=0; i<4; i++) {
		if (pipe_num == i) outputs[i];
	}
}

void initialize () {
  
	int channel = get_channel();
	int other = 1;
	led_channel[0] = channel; // led_personal

	radio.openWritingPipe(pipes[channel]);

	for (int i=0; i<4; i++) {
		if (i != channel) {
			radio.openReadingPipe(other,pipes[i]);
			other++;
		}
	}

	fader(100);
	fader(100);
	fader(100);
	fader(100);
	
	fader(100);
	blink(100);
	blink(100);
	buttons2leds(1000);
}

void buttons2leds(int ms) {
	for (int i=0; i<4; i++) {
		int temp = digitalRead(inputs[i]);
		digitalWrite(outputs[i], temp);
	}
	delay (ms);
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
