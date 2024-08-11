#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

class MotorDriver
{
protected:
  int en;
  int inA;
  int inB;

  bool changeDirection = false;

  int minOut = 0;
  int maxOut = 100;

public:
  MotorDriver(int _inA, int _inB = -1, int _en = -1)
  {
    inA = _inA;
    inB = _inB;
    en = _en;
  }

  void setReversed(bool reversed)
  {
    changeDirection = reversed;
  }

  void init()
  {
    if (en != -1)
    {
      pinMode(en, OUTPUT);
    }
    pinMode(inA, OUTPUT);
    if (inB != -1)
    {
      pinMode(inB, OUTPUT);
    }
  }

  virtual void setSpeed(int newSpeed);
  virtual void stop();
  void update(){}
};

class HBridgeDriver : public MotorDriver
{
public:
  HBridgeDriver(int _inA, int _inB, int _en = -1) : MotorDriver(_inA, _inB, _en) {}

  void stop()
  {
    if (en != -1)
    {
      digitalWrite(en, LOW);
    }
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }

  void setSpeed(int speedPercentage)
  {
    if (changeDirection)
    {
      speedPercentage = -speedPercentage;
    }

    int minPwm = (minOut * 255) / 100;
    int maxPwm = (maxOut * 255) / 100;
    uint8_t pwm = 0;

    if (en != -1)
    {
      digitalWrite(en, HIGH);
    }

    if (speedPercentage == 0)
    {
      pwm = 0;
      digitalWrite(inA, LOW);
      analogWrite(inB, LOW);
    }
    else if (speedPercentage > 0)
    {
      pwm = ((maxPwm - minPwm) * speedPercentage) / 100;
      digitalWrite(inA, LOW);
      analogWrite(inB, pwm);
    }
    else if (speedPercentage < 0)
    {
      pwm = ((maxPwm - minPwm) * (-speedPercentage)) / 100;
      analogWrite(inA, pwm);
      digitalWrite(inB, LOW);
    }

    Serial.println("Moving motor");
    Serial.println(speedPercentage);
    Serial.println(minPwm);
    Serial.println(maxPwm);
    Serial.println(pwm);
  }
};

#include "Servo.h"

class EscDriver : public MotorDriver
{
private:
  Servo esc;

public:
  EscDriver(int _inA) : MotorDriver(_inA), esc() {}

  void init()
  {
    esc.attach(inA);
    stop();
  }

  void stop()
  {
    esc.writeMicroseconds(1500);
  }

  void setSpeed(int speedPercentage)
  {
    if (changeDirection)
    {
      speedPercentage = -speedPercentage;
    }

    int pwm = 1500;
    if (speedPercentage == 0)
    {
      pwm = 1500;
    }
    else if (speedPercentage > 0)
    {
      pwm = 1500 + (speedPercentage * 500) / 100;
    }
    else if (speedPercentage < 0)
    {
      pwm = 1500 + (speedPercentage * 500) / 100;
    }
    esc.writeMicroseconds(pwm);
  }
};
#endif