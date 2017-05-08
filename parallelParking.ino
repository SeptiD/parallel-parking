# define RED_LED 10
# define GREEN_LED 11
# define MOTOR1_PIN1_FRONT 3
# define MOTOR1_PIN2_BACK 5
# define MOTOR2_PIN1_RIGHT 6
# define MOTOR2_PIN2_LEFT 9
# define IR_S1 0
# define IR_S2 1
# define US_S3 7
# define INFRARED_FACTOR (double)1286.5
# define INFRARED_OFFSET (double)1.0
# define DISTANCE_FROM_INFRARED(x) (double)(INFRARED_FACTOR/(double)x-INFRARED_OFFSET)
# define DISTANCE_FROM_ULTRASOUND(x) (((int)(x/29))/2)
# define MAX_DETECTED_CARS 2

int car_det_global = 0;
int th_spot  = 15; //cm

int getActualAdvancingState()
{
  int temp_S1_value = DISTANCE_FROM_INFRARED(analogRead(IR_S1)) ;
  int temp_S2_value = DISTANCE_FROM_INFRARED(analogRead(IR_S2)) ;
  if(temp_S1_value <= th_spot & temp_S2_value <= th_spot)
  {
    return 1;
  }
  if(temp_S1_value <= th_spot & temp_S2_value >= th_spot)
  {
    return 2;
  }
  if(temp_S1_value >= th_spot & temp_S2_value <= th_spot)
  {
    return 3;
  }
  if(temp_S1_value >= th_spot & temp_S2_value >= th_spot)
  {
    return 4;
  }
  
}

void lookForParkingSpotInitialization()
{
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,HIGH );
  analogWrite(MOTOR1_PIN1_FRONT,60);
  analogWrite(MOTOR1_PIN2_BACK,0);
}

void stopCar()
{
  analogWrite(MOTOR1_PIN1_FRONT,0);
  analogWrite(MOTOR1_PIN2_BACK,0);
}

void lookForParkingSpot()
{
   int initialState = getActualAdvancingState();
   int newState;

   lookForParkingSpotInitialization();

   while(car_det_global < MAX_DETECTED_CARS)
   {
      switch(initialState)
      {
        case 1:
          car_det_global++;
          if(car_det_global < MAX_DETECTED_CARS)
          {
            //wait for state4
            while(getActualAdvancingState() != 4)
            {
              delay(5);  
            }
          }
          break;
        case 2:
          //wait for state1
          while(getActualAdvancingState() != 1)
          {
            delay(5);
          }
          break;
        case 3:
          car_det_global++;
          //wait for state4
          while(getActualAdvancingState() != 4)
          {
            delay(5);  
          }
          break;
        case 4:
          //spot detected
          //wait for state2
          while(getActualAdvancingState() != 2)
          {
            delay(5);  
          }
          break;
          
      }
   }

   stopCar();
}


void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(MOTOR1_PIN1_FRONT,OUTPUT);
  pinMode(MOTOR1_PIN2_BACK,OUTPUT);
  pinMode(MOTOR2_PIN1_RIGHT,OUTPUT);
  pinMode(MOTOR2_PIN2_LEFT,OUTPUT);

  pinMode(IR_S1,INPUT);
  pinMode(IR_S2,INPUT);
  pinMode(US_S3,INPUT);
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

    pinMode(US_S3,OUTPUT);
    digitalWrite(US_S3,0);
    delayMicroseconds(2);
    digitalWrite(US_S3,1);
    delayMicroseconds(10);
    digitalWrite(US_S3,0);
    pinMode(US_S3,INPUT);
    int value = pulseIn(US_S3,1);
    Serial.println(value);
    delay(500);
    
  
}
