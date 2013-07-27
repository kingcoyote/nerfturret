#include <Servo.h>
#include <NerfTurret.h>
#include <Wire.h>
#include <WiiChuck.h>

WiiChuck controller;
NerfTurret turret;

// 0-255 amount of dead space at the center position of the joystick to be ignored
#define DEAD_ZONE 10

void setup() {
  controller.begin();
  controller.update();
}

void loop() {
  controller.update();
  
  if(controller.buttonZ) {
    turret.setGunMotors(true);
  }
  
  if(controller.buttonC) {
    turret.fire();  
  }
  
  int el_speed = controller.joyY - 128;
  int az_speed = controller.joyX - 128;
  
  if (el_speed < DEAD_ZONE && el_speed > 0 - DEAD_ZONE) {
    el_speed = 0;
  }
  
  if(az_speed < DEAD_ZONE && az_speed > 0 - DEAD_ZONE) {
    az_speed = 0;
  }
  
  turret.setElevationSpeed(el_speed);
  turret.setAzimuthSpeed(az_speed);
}

