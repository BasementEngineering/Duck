#include "EscPropulsion.h"
#include <Arduino.h>
#include "Config.h"

//#define DEBUG_PROPULSION

#include "Servo.h"

EscPropulsion::EscPropulsion(int _in1, int _in2):
leftEsc(),rightEsc(){
  in1 = _in1;
  in2 = _in2;
}

void EscPropulsion::initPins(){
  //pinMode(in1,OUTPUT);
  //pinMode(in2,OUTPUT);
  leftEsc.attach(in1);
  rightEsc.attach(in2);

  stop();
}

void EscPropulsion::moveLeft(int speedPercentage){
  if(leftReversed){
    speedPercentage = -speedPercentage;
  }
  moveMotor(leftEsc,speedPercentage,MIN_PWM_L,MAX_PWM_L);
}

void EscPropulsion::moveRight(int speedPercentage){
    if(rightReversed){
    speedPercentage = -speedPercentage;
  }
  moveMotor(rightEsc,speedPercentage,MIN_PWM_R,MAX_PWM_R);
}

void EscPropulsion::moveMotor(Servo& esc, int speedPercentage,int minPwm, int maxPwm){
  int pwm = 1500;
  if(speedPercentage == 0){
    pwm=1500;
    
    //digitalWrite(pin1,LOW);
    //analogWrite(pin2,LOW);
  }
  else if(speedPercentage > 0){
    pwm = 1500 + (speedPercentage*500)/100;
    /*pwm = ((maxPwm-minPwm)*speedPercentage)/100;
    digitalWrite(pin1,LOW);
    analogWrite(pin2,pwm);*/
  }
  else if(speedPercentage < 0){
    pwm = 1500 + (speedPercentage*500)/100;

    /*pwm = ((maxPwm-minPwm)*(-speedPercentage))/100;
    digitalWrite(pin1,pwm);
    analogWrite(pin2,LOW);*/
  }
  esc.writeMicroseconds(pwm);
}

void EscPropulsion::stop(){
  //digitalWrite(in1,LOW);
  //digitalWrite(in2,LOW);
  leftEsc.writeMicroseconds(1500);
  rightEsc.writeMicroseconds(1500);
}

void EscPropulsion::setSpeed(int newSpeed){
  currentSpeed = newSpeed;
  translateToMotors(currentSpeed,currentDirection);
}

void EscPropulsion::setDirection(int newDirection){
  currentDirection = newDirection;
  translateToMotors(currentSpeed,currentDirection);
}

void EscPropulsion::translateToMotors(int speedPercentage, int direction){
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
  
}*/
