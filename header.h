#ifndef HEADER_H
#define HEADER_H

# define RED_LED 11
# define GREEN_LED 10
# define MOTOR1_PIN1_FRONT 5
# define MOTOR1_PIN2_BACK 3
# define MOTOR2_PIN1_RIGHT 6
# define MOTOR2_PIN2_LEFT 9
# define IR_S1 0
# define IR_S2 1
# define US_S3 7
# define INFRARED_FACTOR (double)1286.5
# define INFRARED_OFFSET (double)1.0
# define DISTANCE_FROM_INFRARED(x) (2650/(x+95))-3
# define DISTANCE_FROM_ULTRASOUND(x) (((int)(x/29))/2)
# define MAX_DETECTED_CARS 2
# define WORKING_MOTOR_SPEED 55 //90 e ok
# define STEERING_MOTOR_SPEED 250
# define INFRARED_SENSOR_ACCEPTED_DIFFERECE 1 //cm

#endif
