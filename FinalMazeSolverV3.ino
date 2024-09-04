/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    EncoderMotorTestMoveTo.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/19
 * @brief   Description: This file contains sample code for controlling an Encoder Motor device.
 *
 * Function List:
 *
 *    1. void MeEncoderMotor::begin();
 *    2. boolean MeEncoderMotor::moveTo(float angle, float speed);
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * forfish      2015/11/19    1.0.0          Added some descriptions
 * </pre>
 */

// Header comments providing information about the file, author, version, and brief description

#include "MeAuriga.h" // Includes the MeAuriga library which contains functions to control MakeBlock's Auriga board
#include <Wire.h> // Includes the Wire library for I2C communication
#include <SoftwareSerial.h> // Includes the SoftwareSerial library for serial communication using digital pins

MeEncoderOnBoard motor1(SLOT1);   // Motor at slot1 // Creates an instance of the MeEncoderOnBoard class for motor1, connected to slot 1
MeEncoderOnBoard motor2(SLOT2);   // Motor at slot2 // Creates an instance of the MeEncoderOnBoard class for motor2, connected to slot 2

MeUltrasonicSensor ultraSensor(PORT_10); // Creates an instance of the MeUltrasonicSensor class for an ultrasonic sensor connected to port 10

MeGyro gyro(0, 0x69); // Creates an instance of the MeGyro class for a gyro sensor, initialized with address 0x69

void setup() {
  Serial.begin(9600);   // Begin serial communication at 9600 baud
  gyro.begin();         // Initialize the gyro sensor
}

void stop() {
  motor1.setMotorPwm(0);
  motor2.setMotorPwm(0); // Sets the PWM values of both motors to 0, stopping the robot
}

void fwd() {
  motor1.setMotorPwm(-120);
  motor2.setMotorPwm(120); // Sets the PWM values to move the robot forward
}

void turnRight() {
  int currentAngle = gyro.getAngleZ(); // Gets the current Z-axis angle from the gyro sensor
  int targetAngle = (int)(currentAngle + 90) % 360; // Calculates the target angle 90 degrees to the right
  while (abs(currentAngle - targetAngle) > 5) {
    gyro.update(); // Updates the gyro sensor readings
    currentAngle = gyro.getAngleZ(); // Gets the updated current Z-axis angle from the gyro sensor
    if (targetAngle > currentAngle) {
      motor1.setMotorPwm(195);
      motor2.setMotorPwm(195); // Sets the PWM values to turn the robot to the right
    }
  }
  stop(); // Stops the robot after completing the turn
  gyro.begin(); // Reinitializes the gyro sensor
}

void turnLeft() {
  int currentAngle = gyro.getAngleZ(); // Gets the current Z-axis angle from the gyro sensor
  int targetAngle = (int)(currentAngle - 90) % 360; // Calculates the target angle 90 degrees to the left
  while (abs(currentAngle - targetAngle) > 5) {
    gyro.update(); // Updates the gyro sensor readings
    currentAngle = gyro.getAngleZ(); // Gets the updated current Z-axis angle from the gyro sensor
    if (targetAngle < currentAngle) {
      motor1.setMotorPwm(-195);
      motor2.setMotorPwm(-195);// Sets the PWM values to turn the robot to the left
    }
  }
  stop(); // Stops the robot after completing the turn
  gyro.begin(); // Reinitializes the gyro sensor
}

void turnAround() {
  gyro.begin(); // Initializes the gyro sensor
  turnRight(); // Turns the robot to the right by 90 degrees
  delay(500); // Short delay to ensure the turn completes
  turnRight(); // Turns the robot to the right by another 90 degrees to complete a 180-degree turn
}

void loop() {
  gyro.update(); // Updates the gyro sensor readings
  float angle = gyro.getAngleZ(); // Gets the current Z-axis angle from the gyro sensor
  float dist = ultraSensor.distanceCm(); // Gets the distance measured by the ultrasonic sensor in centimeters
  
  // Checks if an obstacle is detected within 10 cm
  if (dist < 10) {
    stop(); // Stops the robot
    delay(500); // Waits for a moment
    turnLeft(); // Turns left
    delay(500); // Short delay to ensure the turn completes
    dist = ultraSensor.distanceCm(); // Gets updated distance reading from the ultrasonic sensor
    if (dist < 25) {    // If obstacle is still close, turns around
      turnAround();
      delay(500);
    }
  } else {
    fwd(); // Moves forward if no obstacle is detected
  }
}
