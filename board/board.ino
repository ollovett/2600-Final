
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t recvPin = 13; // Infrared receiving pin
IRrecv irrecv(recvPin);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

//each led pin represents the postion on the tictactoe board
int ledPin1 = 15;
int ledPin2 = 2;
int ledPin3 = 0;
int ledPin4 = 4;
int ledPin5 = 5;
int ledPin6 = 18;
int ledPin7 = 19;
int ledPin8 = 21;
int ledPin9 = 22;

void setup() {
  //set the channel,frequency,esolution_bits
  ledcSetup(0,1000,8);
  ledcSetup(1,1000,8);
  ledcSetup(2,1000,8);
  ledcSetup(3,1000,8);
  ledcSetup(4,1000,8);
  ledcSetup(5,1000,8);
  ledcSetup(6,1000,8);
  ledcSetup(7,1000,8);
  ledcSetup(8,1000,8);

  //attach the channel to GPIO pin
  ledcAttachPin(ledPin1,0);
  ledcAttachPin(ledPin2,1);
  ledcAttachPin(ledPin3,2);
  ledcAttachPin(ledPin4,3);
  ledcAttachPin(ledPin5,4);
  ledcAttachPin(ledPin6,5);
  ledcAttachPin(ledPin7,6);
  ledcAttachPin(ledPin8,7);
  ledcAttachPin(ledPin9,8);
  irrecv.enableIRIn();        // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {        // Waiting for decoding
    handleControl(results.value);       // Handle the commands from remote control
    irrecv.resume();                    // Receive the next value
  }
}

void handleControl(unsigned long value) {
  // Handle the commands
  switch (value) {
    case 0xFF30CF:              // Receive the number '1'
      ledcWrite(0, 255);
      break;
    case 0xFF18E7:              // Receive the number '2'
      ledcWrite(1, 255);
      break;
    case 0xFF7A85:              // Receive the number '3'
      ledcWrite(2, 255);
      break;
    case 0xFF10EF:              // Receive the number '4'
      ledcWrite(3, 255);
      break;
    case 0xFF38C7:              // Receive the number '5'
      ledcWrite(4, 255);
      break;
    case 0xFF5AA5:              // Receive the number '6'
      ledcWrite(5, 255);
      break;
    case 0xFF42BD:              // Receive the number '7'
      ledcWrite(6, 255);  
      break;
    case 0xFF4AB5:              // Receive the number '8'
      ledcWrite(7, 255);  
      break;
    case 0xFF52AD:              // Receive the number '9'
      ledcWrite(8, 255);  
      break;
  }
}
