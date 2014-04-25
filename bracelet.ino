// Defining pins for arduino
// 3,5,6,9,10,11 PWM
// OUTPUT PINS
int led_personal  = 3;
int led_channel_A = 5;
int led_channel_B = 6;
int led_channel_C = 9;
// int led_onboard  = 13; // std arduino onboard led needed on sck

// INPUT PINS
int dip_switch_1 = A1;
int dip_switch_2 = A2;
int dip_switch_3 = A3;
int dip_switch_4 = A4;
int pulse_input  = A0;

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

int temp = 0; // for reading the switches

void setup() {                
  for (int i=0; i<outputs_count; i++) {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i],  HIGH);  
  }
  for (int i=0; i<inputs_count; i++) {
    pinMode(inputs[i], INPUT);
  }
  initialize();
}

void loop() {
  for (int i=0; i<4; i++) {
    temp = digitalRead(inputs[i]);
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