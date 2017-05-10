#include "header.h"

int car_det_global = 0;
int th_spot  = 10; //cm
int th_wall = 21; //cm
int th_l3 = 4; //cm

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;
int blinkingLedState = LOW;

double getS1Distance()
{
  return DISTANCE_FROM_INFRARED(analogRead(IR_S1)) ;
}

double getS2Distance()
{
    return DISTANCE_FROM_INFRARED(analogRead(IR_S2)) ;
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

void driveForward()
{
  analogWrite(MOTOR1_PIN1_FRONT, WORKING_MOTOR_SPEED);
  analogWrite(MOTOR1_PIN2_BACK, 0);
}

void lookForParkingSpotInitialization()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH );
  driveForward();
}

void stopCar()
{
  analogWrite(MOTOR1_PIN1_FRONT, 0);
  analogWrite(MOTOR1_PIN2_BACK, 0);
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
//            Serial.print(getS1Distance());
//            Serial.print("   ");
//            Serial.print(getS2Distance());
//            Serial.println();
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
//          Serial.print(getS1Distance());
//            Serial.print("   ");
//            Serial.print(getS2Distance());
//            Serial.println();
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
//          Serial.print(getS1Distance());
//            Serial.print("   ");
//            Serial.print(getS2Distance());
//            Serial.println();
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
//          Serial.print(getS1Distance());
//            Serial.print("   ");
//            Serial.print(getS2Distance());
//            Serial.println();
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
  digitalWrite(RED_LED,LOW);
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

void driveBackward()
{
  analogWrite(MOTOR1_PIN1_FRONT, 0);
  analogWrite(MOTOR1_PIN2_BACK, WORKING_MOTOR_SPEED);
}

double infraredSensorDifference()
{
  double sensorDistance = getS1Distance() - getS2Distance();
  if(sensorDistance < 0)
  {
    sensorDistance = sensorDistance * (-1);
  }
  return sensorDistance;
}

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
  return DISTANCE_FROM_INFRARED(value);
}

void parkingProcedure()
{
  blinkingGreen();
  steerRight();
  delay(5000);
  driveBackward();

  while(getS2Distance() >th_wall)
  {
    delay(50);
    blinkingGreen();
  }

  straightenWheels();
  blinkingGreen();
  steerLeft();

  while(infraredSensorDifference() > INFRARED_SENSOR_ACCEPTED_DIFFERECE & getS3Distance()>= th_l3 )
  {
    delay(50);
    blinkingGreen();    
  }

  if(infraredSensorDifference() <= INFRARED_SENSOR_ACCEPTED_DIFFERECE)
  {
    straightenWheels();
  }

  while(getS3Distance()>= th_l3)
  {
    delay(50);
    blinkingGreen(); 
  }

  if(getS3Distance()<=th_l3)
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

  //straightenWheels();
  lookForParkingSpot();
  parkingProcedure();
  //printState();
  //steerRight();
}
