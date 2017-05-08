#include "header.h"

int car_det_global = 0;
int th_spot  = 15; //cm
int th_wall = 21 cm;

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;
int blinkingLedState = LOW;

int getActualAdvancingState()
{
  int temp_S1_value = DISTANCE_FROM_INFRARED(analogRead(IR_S1)) ;
  int temp_S2_value = DISTANCE_FROM_INFRARED(analogRead(IR_S2)) ;
  if (temp_S1_value <= th_spot & temp_S2_value <= th_spot)
  {
    return 1;
  }
  if (temp_S1_value <= th_spot & temp_S2_value >= th_spot)
  {
    return 2;
  }
  if (temp_S1_value >= th_spot & temp_S2_value <= th_spot)
  {
    return 3;
  }
  if (temp_S1_value >= th_spot & temp_S2_value >= th_spot)
  {
    return 4;
  }

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
          initialState = newState;
        }
        break;
      case 2:
        //wait for state1
        while ((newState = getActualAdvancingState()) != 1)
        {
          delay(5);
        }
        initialState = newState;
        break;
      case 3:
        car_det_global++;
        //wait for state4
        while ((newState = getActualAdvancingState()) != 4)
        {
          delay(5);
        }
        initialState = newState;
        break;
      case 4:
        //spot detected
        //wait for state2
        while ((newState = getActualAdvancingState()) != 2)
        {
          delay(5);
        }
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

void driveBackward()
{
  analogWrite(MOTOR1_PIN1_FRONT, 0);
  analogWrite(MOTOR1_PIN2_BACK, WORKING_MOTOR_SPEED);
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
  //  digitalWrite(GREEN_LED,HIGH);
  //  digitalWrite(RED_LED,LOW );
  //  delay(1000);
  //  digitalWrite(RED_LED,HIGH);
  //  digitalWrite(GREEN_LED,LOW);
  //  delay(1000);
  //  analogWrite(MOTOR1_PIN1,60);
  //  analogWrite(MOTOR1_PIN2,0);
  //  digitalWrite(GREEN_LED,HIGH);
  //  digitalWrite(RED_LED,LOW );
  //  delay(1000);
  //  analogWrite(MOTOR1_PIN1,0);
  //  analogWrite(MOTOR1_PIN2,0);
  //  delay(1000);
  //  analogWrite(MOTOR1_PIN1,0);
  //  analogWrite(MOTOR1_PIN2,60);
  //   digitalWrite(RED_LED,HIGH);
  //  digitalWrite(GREEN_LED,LOW);
  //  delay(1000);
  //  analogWrite(MOTOR1_PIN1,0);
  //  analogWrite(MOTOR1_PIN2,0);
  //  delay(1000);
  //
  //  Serial.print(32);

  //  analogWrite(MOTOR2_PIN1,250);
  //  analogWrite(MOTOR2_PIN2,0);
  //  digitalWrite(GREEN_LED,HIGH);
  //  digitalWrite(RED_LED,LOW );
  //  delay(1000);
  //  analogWrite(MOTOR2_PIN1,0);
  //  analogWrite(MOTOR2_PIN2,0);
  //  delay(1000);
  //  analogWrite(MOTOR2_PIN1,0);
  //  analogWrite(MOTOR2_PIN2,250);
  //   digitalWrite(RED_LED,HIGH);
  //  digitalWrite(GREEN_LED,LOW);
  //  delay(1000);
  //  analogWrite(MOTOR2_PIN1,0);
  //  analogWrite(MOTOR2_PIN2,0);
  //  delay(1000);


  //Serial.println(DISTANCE_FROM_INFRARED(analogRead(IR_S1)));
  //delay(1000);

  pinMode(US_S3, OUTPUT);
  digitalWrite(US_S3, 0);
  delayMicroseconds(2);
  digitalWrite(US_S3, 1);
  delayMicroseconds(10);
  digitalWrite(US_S3, 0);
  pinMode(US_S3, INPUT);
  int value = pulseIn(US_S3, 1);
  Serial.println(value);
  delay(500);


}
