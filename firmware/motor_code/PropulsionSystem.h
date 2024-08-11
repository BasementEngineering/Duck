#ifndef PROPULSIONSYSTEM_H
#define PROPULSIONSYSTEM_H

#include <Arduino.h>
#include "MotorDriver.h"

#define DEBUG_PROPULSION

class PropulsionSystem
{
public:
  virtual void init();
  virtual void stop();
  virtual void setReversed(bool reversed1, bool reversed2);
  virtual void setSpeed(int newSpeed);
  virtual void setDirection(int newDirection);

  virtual void moveLeft(int speedPercentage);
  virtual void moveRight(int speedPercentage);

protected:
  MotorDriver *motor1 = nullptr;
  MotorDriver *motor2 = nullptr;
  int currentSpeed = 0;
  int currentDirection = 0;
};

class DifferentialDrive : public PropulsionSystem
{
public:
  DifferentialDrive(int _in1_A, int _in2_A): PropulsionSystem()
  {
    motor1 = new EscDriver(_in1_A);
    motor2 = new EscDriver(_in2_A);
  }

  DifferentialDrive(int _in1_A, int _in1_B, int _in2_A, int _in2_B, int _en = -1): PropulsionSystem()
  {
    motor1 = new HBridgeDriver(_in1_A, _in1_B, _en);
    motor2 = new HBridgeDriver(_in2_A, _in2_B);
  }

  void init()
  {
    motor1->init();
    motor2->init();
  }

  void stop(){
    motor1->stop();
    motor2->stop();
  }

  void setSpeed(int newSpeed){
    currentSpeed = newSpeed;
    translateToMotors(currentSpeed,currentDirection);
  }

  void setDirection(int newDirection){
    currentDirection = newDirection;
    translateToMotors(currentSpeed,currentDirection);
  }

  void setReversed(bool reversed1, bool reversed2)
  {
    motor1->setReversed(reversed1);
    motor2->setReversed(reversed2);
  }

 void translateToMotors(int speedPercentage, int direction){
    
    #ifdef DEBUG_PROPULSION
      Serial.print("Current Speed: ");Serial.println(currentSpeed);
      Serial.print("Current Direction: ");Serial.println(currentDirection);
    #endif

    int leftSpeed = 0;
    int rightSpeed = 0;
  
    if(speedPercentage == 0){
      stop();
    }
    else{
      if(direction == 0){
        leftSpeed = speedPercentage;
        rightSpeed = speedPercentage;
      }
      if(direction < 0){ //Turning Right
        leftSpeed = ((100+direction)*speedPercentage)/100;
        rightSpeed = speedPercentage;
      }
      else if(direction > 0){
        leftSpeed = speedPercentage;
        rightSpeed = ((100-direction)*speedPercentage)/100;
      }
      #ifdef DEBUG_PROPULSION
      Serial.print("Left Speed: ");Serial.println(leftSpeed);
      Serial.print("Right Speed: ");Serial.println(rightSpeed);
      #endif

      moveLeft(leftSpeed);
      moveRight(rightSpeed);
    }
  
  }

  void moveLeft(int speedPercentage){
    motor1->setSpeed(speedPercentage);
  }

  void moveRight(int speedPercentage){
    motor2->setSpeed(speedPercentage);
  }
};

#endif
