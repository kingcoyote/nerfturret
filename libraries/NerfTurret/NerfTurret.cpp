#include "Arduino.h"
#include "NerfTurret.h"
#include "Servo.h"

// NPN transistor to send power to the Rayven CS-18's motors
#define PIN_GUN_MOTORS	2

// PWM servo that controls the gun's firing piston
#define PIN_TRIGGER 	3

// 4 SPDT switches to limit left/right/up/down movement
#define PIN_SW_LEFT 	4
#define PIN_SW_RIGHT 	5
#define PIN_SW_TOP 		6
#define PIN_SW_BOTTOM 	7

// L293D pins
#define PIN_EL_FOR 		8
#define PIN_EL_REV 		9
#define PIN_EL_SPEED 	10
#define PIN_AZ_SPEED 	11
#define PIN_AZ_FOR 		12
#define PIN_AZ_REV 		13

// trigger servo positions and time needed to move the servo
#define TRIGGER_RETURN 	75
#define TRIGGER_FIRE 	135
#define TRIGGER_DELAY 	250

// time needed for the gun motors to spin up
#define GUN_SPOOL_DELAY	2000

NerfTurret::NerfTurret() {
  _trigger.attach(PIN_TRIGGER);
  _trigger.write(TRIGGER_RETURN);
  
  pinMode(PIN_TRIGGER, OUTPUT);
  
  pinMode(PIN_GUN_MOTORS, OUTPUT);
  
  pinMode(PIN_SW_LEFT, INPUT);
  pinMode(PIN_SW_RIGHT, INPUT);
  pinMode(PIN_SW_TOP, INPUT);
  pinMode(PIN_SW_BOTTOM, INPUT);
  
  pinMode(PIN_EL_FOR, OUTPUT);
  pinMode(PIN_EL_REV, OUTPUT);
  pinMode(PIN_EL_SPEED, OUTPUT);
  
  pinMode(PIN_AZ_FOR, OUTPUT);
  pinMode(PIN_AZ_REV, OUTPUT);
  pinMode(PIN_AZ_SPEED, OUTPUT);
}

int NerfTurret::setElevationSpeed(int speed) {
  if (speed < 0 && readPositionSwitch(PIN_SW_BOTTOM)) {
    speed = 0;
  }
  
  if (speed > 0 && readPositionSwitch(PIN_SW_TOP)) {
	speed = 0;
  }

  setMotorSpeed(speed, PIN_EL_FOR, PIN_EL_REV, PIN_EL_SPEED);
  return speed;
}

int NerfTurret::setAzimuthSpeed(int speed) {
  if (speed < 0 && readPositionSwitch(PIN_SW_LEFT)) {
    speed = 0;
  }
  
  if (speed > 0 && readPositionSwitch(PIN_SW_RIGHT)) {
	speed = 0;
  }
  
  setMotorSpeed(speed, PIN_AZ_FOR, PIN_AZ_REV, PIN_AZ_SPEED);
  return speed;
}

bool NerfTurret::setGunMotors(bool enable) {
  digitalWrite(PIN_GUN_MOTORS, enable);
  
  if(enable && _gunSpoolTime == 0) {
    _gunSpoolTime = millis();
  } else if (enable == false) {
    _gunSpoolTime = 0;
  }
}

void NerfTurret::fire() {
  setGunMotors(true);
  
  
  if (_gunSpoolTime < millis() - GUN_SPOOL_DELAY) {
	_trigger.write(TRIGGER_FIRE);
	delay(TRIGGER_DELAY);
	_trigger.write(TRIGGER_RETURN);
  }
}

bool NerfTurret::readPositionSwitch(int position) {
  return digitalRead(position);
}

void NerfTurret::setMotorSpeed(int speed, int pinForward, int pinReverse, int pinSpeed) {
  if (speed < 0) {
    digitalWrite(pinForward, HIGH);
	digitalWrite(pinReverse, LOW);
	digitalWrite(pinSpeed, abs(speed));
  } else if (speed > 0) {
	digitalWrite(pinReverse, HIGH);
	digitalWrite(pinForward, LOW);
	digitalWrite(pinSpeed, abs(speed));
  } else {
	digitalWrite(pinForward, HIGH);
	digitalWrite(pinReverse, HIGH);
	digitalWrite(pinSpeed, 0);
  }
}