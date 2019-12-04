#include <Arduino.h>
#include "../include/pi10000.h"
// #include "Arduino_LCD.h"

#define digit_00          0
#define digit_01          1
#define digit_02          2
#define digit_03          3
#define digit_04          4
#define digit_05          5
#define digit_06          6
#define digit_07          7
#define digit_08          8
#define digit_09          9
#define drive_pin_a       10
#define drive_pin_b       11
#define trigger_pin       12
#define release_arms_pin  13
#define drive_forward     255

void activateSolenoid(int Pin) {
  // send a HIGH Signal to Pin
  digitalWrite(Pin, HIGH);
}

void deactivateSolenoid(int Pin) {
  // send a HIGH Signal to Pin
  digitalWrite(Pin, LOW);
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(digit_00, OUTPUT);
  pinMode(digit_01, OUTPUT);
  pinMode(digit_02, OUTPUT);
  pinMode(digit_03, OUTPUT);
  pinMode(digit_04, OUTPUT);
  pinMode(digit_05, OUTPUT);
  pinMode(digit_06, OUTPUT);
  pinMode(digit_07, OUTPUT);
  pinMode(digit_08, OUTPUT);
  pinMode(digit_09, OUTPUT);
  pinMode(Error_pin, OUTPUT);
  pinMode(start_pin, INPUT);
  pinMode(left_trigger_pin, INPUT);
  pinMode(right_trigger_pin, INPUT);
  pinMode(drive_pin_a, OUTPUT);
  pinMode(drive_pin_b, OUTPUT);
  pinMode(release_arms_pin, OUTPUT);
}

const unsigned long SPACING = 75;  // ms between presses
const unsigned long DURATION = 75; // duration of a press

bool act = true; // if act is true some solenoid needs to toggle
bool extended = false; // if extended is true one solenoid is extended
int piIndex = 0; 
unsigned long beginDelay;
int pinNumber = 0;
bool startPressed = false;
bool atWall = false;
int drivePins[2] = {drive_pin_a, drive_pin_b};

void loop() {

  while (!startPressed) {
    if(digitalRead(trigger_pin)) {
      startPressed = true;
      delay(1000);
    }
  }

  // drive forward until both switches hit.
  while (!atWall) {
    drive(drivePins, drive_forward);
    if (digitalRead(trigger_pin)) {
      atWall = true;
      delay(200);
      brake(drive_pin_a, drive_pin_b);
      deactivateSolenoid(release_arms_pin);
    }
  }

  if(act) {
    pinNumber = pgm_read_byte_near(bigPI + piIndex);
    

    //Serial.println(piIndex);
    Serial.println(pinNumber - '0');
    switch(pinNumber)
    {
      case '0':
        pinNumber = digit_00;
        break;
      case '1':
        pinNumber = digit_01;
        break;
      case '2':
        pinNumber = digit_02;
        break;
      case '3':
        pinNumber = digit_03;
        break;
      case '4':
        pinNumber = digit_04;
        break;
      case '5':
        pinNumber = digit_05;
        break;
      case '6':
        pinNumber = digit_06;
        break;
      case '7':
        pinNumber = digit_07;
        break;
      case '8':
        pinNumber = digit_08;
        break;
      case '9':
        pinNumber = digit_09;
        break;
      default:
        pinNumber = Error_pin;
    }
    //If statements to toggle between extend/retract
    if (extended) {
      deactivateSolenoid( pinNumber );
      extended = false;
      Serial.print("deactivate ");
      Serial.println(pinNumber );
    } else {
      activateSolenoid( pinNumber );
      extended = true;
      Serial.print("activate ");
      Serial.println( pinNumber );
    }

    beginDelay = millis();
    act = false;
  } 
  else 
  {
    if (millis() - beginDelay > (extended ? DURATION : SPACING))
      {
        act = true;
        if (!extended) 
        {
          // only iterate pi index when we have extended and unextended
          piIndex++;
        }
      }
  }

  // put your main code here, to run repeatedly:
}
