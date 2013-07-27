#include "Arduino.h"
#include "Servo.h"

#ifndef _NERFTURRET_H_
#define _NERFTURRET_H_

class NerfTurret {
  public:
    NerfTurret();
	int setElevationSpeed(int);
	int setAzimuthSpeed(int);
	bool setGunMotors(bool);
	void fire();
  private:
    Servo _trigger;
	unsigned long _gunSpoolTime;
	bool readPositionSwitch(int);
	void setMotorSpeed(int, int, int, int);
};

#endif