#ifndef CARMOVES_H
#define CARMOVES_H
#include "Arduino.h"


void driveForward();
void driveBackward();
void driveBackwardSpeed(int speed);
void stopCar();

void steerRight();
void steerLeft();
void straightenWheels();

void bounceToStop();
void bounceBackToStop();




#endif
