#ifndef ESCPROPULSION_H
#define ESCPROPULSION_H

#include "Servo.h"

class EscPropulsion{
  public: 
  EscPropulsion(int _in1, int _in2);
  void initPins();

  void stop();

  void setSpeed(int newSpeed);
  void setDirection(int newDirection);
  void runTestSequence();
  void moveLeft(int speedPercentage);
  void moveRight(int speedPercentage);
  
  
private:

  void translateToMotors(int speed, int direction);
  void moveMotor(Servo& esc, int speedPercentage,int minPwm, int maxPwm);

  private:
    int in1;
    int in2;

    Servo leftEsc;
    Servo rightEsc;

    int currentSpeed = 0;
    int currentDirection = 0;

    bool leftReversed = true;
    bool rightReversed = false;

    //void moveMotor(Servo& esc, int speed);
};

#endif
