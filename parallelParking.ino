#include "header.h"
extern "C" {
#include "carMoves.h"
}
extern "C" {
#include "sensors.h"
}

int car_det_global = 0;
int th_spot  = 10; //cm
int th_wall = 18; //cm
int th_l3 = 7; //cm
int snap = 160;

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 500;
int blinkingLedState = LOW;


int getS3Distance()
{
  pinMode(US_S3, OUTPUT);
  digitalWrite(US_S3, 0);
  delayMicroseconds(2);
  digitalWrite(US_S3, 1);
  delayMicroseconds(10);
  digitalWrite(US_S3, 0);
  pinMode(US_S3, INPUT);
  int value = pulseIn(US_S3, 1);
  return DISTANCE_FROM_ULTRASOUND(value);
}


void printState()
{
  Serial.print(getS1Distance());
  Serial.print("   ");
  Serial.print(getS2Distance());
  Serial.print("   ");
  Serial.print(getActualAdvancingState());
  Serial.println();
  delay(100);
}

void printSensors()
{
  Serial.print(getS1Distance());
  Serial.print("  ");
  Serial.print(getS2Distance());
  Serial.print("  ");
  Serial.print(getS3Distance());
  Serial.print("  ");
  Serial.println();
  delay(100);
}



int getActualAdvancingState()
{
  int temp_S1_value = getS1Distance();
  int temp_S2_value = getS2Distance();

  if (temp_S1_value < th_spot && temp_S2_value < th_spot)
  {
    return 1;
  }
  if (temp_S1_value < th_spot && temp_S2_value >= th_spot)
  {
    return 2;
  }
  if (temp_S1_value >= th_spot && temp_S2_value < th_spot)
  {
    return 3;
  }
  if (temp_S1_value >= th_spot && temp_S2_value >= th_spot)
  {
    return 4;
  }

}

void lookForParkingSpotInitialization()
{
  straightenWheels();
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH );
  driveForward();
}



void lookForParkingSpot()
{
  int initialState = getActualAdvancingState();
  int newState;

  lookForParkingSpotInitialization();
  Serial.println(initialState);

  while (car_det_global < MAX_DETECTED_CARS)
  {
    switch (initialState)
    {
      case 1:
        car_det_global++;
        if (car_det_global < MAX_DETECTED_CARS)
        {
          //wait for state4
          while ((newState = getActualAdvancingState()) != 4)
          {
            delay(5);
          }
          Serial.println(newState);
          initialState = newState;
        }
        break;
      case 2:
        //wait for state1
        while ((newState = getActualAdvancingState()) != 1)
        {
          delay(5);
        }
        Serial.println(newState);
        initialState = newState;
        break;
      case 3:
        car_det_global++;
        //wait for state4
        while ((newState = getActualAdvancingState()) != 4)
        {
          delay(5);
        }
        Serial.println(newState);
        initialState = newState;
        break;
      case 4:
        //spot detected
        //wait for state2
        while ((newState = getActualAdvancingState()) != 2)
        {
          delay(5);
        }
        Serial.println(newState);
        initialState = newState;
        break;

    }
  }

  stopCar();
}

void blinkingGreen()
{
  digitalWrite(RED_LED, LOW);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    if (blinkingLedState == LOW)
    {
      blinkingLedState = HIGH;
    }
    else
    {
      blinkingLedState = LOW;
    }
  }
  digitalWrite(GREEN_LED, blinkingLedState);
}

void parkingProcedure()
{
  blinkingGreen();
  int counter;
  for (counter = 0; counter < 5; counter++)
  {
    delay(500);
    blinkingGreen();
  }
  steerRight();
  delay(500);
  blinkingGreen();
  driveBackward();
  delay(2000);
  while (getS2Distance() > th_wall)
  {
    delay(10);
    blinkingGreen();
  }

  blinkingGreen();
  driveForward();
  delay(5);
  stopCar();
  //straightenWheels();
  //blinkingGreen();
  steerLeft();
  driveBackwardSpeed(snap);
    blinkingGreen();
  while (infraredSensorDifference() > INFRARED_SENSOR_ACCEPTED_DIFFERECE && getS3Distance() >= th_l3 )
  {
    driveBackwardSpeed(snap);
    delay(5);
    if(snap>=80)
    {
      snap = snap-10;
    }
    blinkingGreen();
  }

  if (infraredSensorDifference() <= INFRARED_SENSOR_ACCEPTED_DIFFERECE)
  {
    straightenWheels();
  }

  while (getS3Distance() >= th_l3)
  {
    delay(50);
    blinkingGreen();
  }

  if (getS3Distance() <= th_l3)
  {
    stopCar();
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW );
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(MOTOR1_PIN1_FRONT, OUTPUT);
  pinMode(MOTOR1_PIN2_BACK, OUTPUT);
  pinMode(MOTOR2_PIN1_RIGHT, OUTPUT);
  pinMode(MOTOR2_PIN2_LEFT, OUTPUT);

  pinMode(IR_S1, INPUT);
  pinMode(IR_S2, INPUT);
  pinMode(US_S3, INPUT);
  Serial.begin(9600);

}


void loop() {
  // put your main code here, to run repeatedly:

  lookForParkingSpot();
  parkingProcedure();
  while(1)
  {
    //do nothing
  }
  //driveForward();
  //printState();
 // steerRight();
  //straightenWheels();
   //steerLeft();
   // printSensors();
}
