#include "sensors.h"
#include "header.h"
#include "Arduino.h"

double getS1Distance()
{
  return DISTANCE_FROM_INFRARED(analogRead(IR_S1)) ;
}

double getS2Distance()
{
    return DISTANCE_FROM_INFRARED(analogRead(IR_S2)) ;
}
//int getS3Distance()
//{
//  pinMode(US_S3, OUTPUT);
//  digitalWrite(US_S3, 0);
//  delayMicroseconds(2);
//  digitalWrite(US_S3, 1);
//  delayMicroseconds(10);
//  digitalWrite(US_S3, 0);
//  pinMode(US_S3, INPUT);
//  int value = pulseIn(US_S3, 1);
//  return DISTANCE_FROM_INFRARED(value);
//}


double infraredSensorDifference()
{
  double sensorDistance = getS1Distance() - getS2Distance();
  if(sensorDistance < 0)
  {
    sensorDistance = sensorDistance * (-1);
  }
  return sensorDistance;
}
