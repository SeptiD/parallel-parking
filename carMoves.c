#include "carMoves.h"
#include "header.h"
#include "Arduino.h"
void driveForward()
{
  analogWrite(MOTOR1_PIN1_FRONT, WORKING_MOTOR_SPEED);
  analogWrite(MOTOR1_PIN2_BACK, 0);
}

void driveBackward()
{
  analogWrite(MOTOR1_PIN1_FRONT, 0);
  analogWrite(MOTOR1_PIN2_BACK, WORKING_MOTOR_SPEED);
}

void stopCar()
{
  analogWrite(MOTOR1_PIN1_FRONT, 0);
  analogWrite(MOTOR1_PIN2_BACK, 0);
}

void steerRight()
{
  digitalWrite(MOTOR2_PIN1_RIGHT,STEERING_MOTOR_SPEED);
  digitalWrite(MOTOR2_PIN2_LEFT,0);
}

void steerLeft()
{
  digitalWrite(MOTOR2_PIN1_RIGHT,0);
  digitalWrite(MOTOR2_PIN2_LEFT,STEERING_MOTOR_SPEED);
}

void straightenWheels()
{
  digitalWrite(MOTOR2_PIN1_RIGHT,0);
  digitalWrite(MOTOR2_PIN2_LEFT,0);
}
