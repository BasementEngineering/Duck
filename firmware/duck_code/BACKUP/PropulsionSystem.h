#ifndef PROPULSIONSYSTEM_H
#define PROPULSIONSYSTEM_H

class PropulsionSystem
{
public:
  PropulsionSystem(int _en, int _in1, int _in2, int _in3, int _in4);
  void initPins();

  void stop();

  void setSpeed(int newSpeed);
  void setDirection(int newDirection);
  void runTestSequence();
  void moveLeft(int speedPercentage);
  void moveRight(int speedPercentage);

private:
  void translateToMotors(int speed, int direction);
  void moveMotor(int pin1, int pin2, int speedPercentage, int minPwm, int maxPwm);

private:
  int en;
  int in1;
  int in2;
  int in3;
  int in4;

  int currentSpeed = 0;
  int currentDirection = 0;

  void moveMotor(int pin1, int pin2, int speed);
};

class MotorDriver
{
private:
  int en = -1;
  int inA = -1;
  int inB = -1;

  int minOut = 0;
  int maxOut = 100;

public:
  motorDriver(int _inA, int _inB = -1, int _en = -1)
  {
    inA = _inA;
    inB = _inB;
    en = _en;
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

  void stop();

  void setOutputLimits(int min, int max)
  {
    minOut = min;
    maxOut = max;
  }

  void update();
  void moveMotor(int speed);
};

class HBridgeDriver : public MotorDriver
{
public:
  HBridgeDriver(int _inA, int _inB, int _en) : MotorDriver(_inA, _inB, _en) {}

  void stop()
  {
    if (en != -1)
    {
      digitalWrite(en, LOW);
    }
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }

  void moveMotor(int speedPercentage)
  {
    int minPwm = (minOut * 255)/100;
    int maxPwm = (maxOut * 255)/100;
    uint8_t pwm = 0;

    if (en != -1){
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
      digitalWrite(inA, pwm);
      analogWrite(inB, LOW);
    }

    Serial.println("Moving motor");
    Serial.println(speedPercentage);
    Serial.println(minPwm);
    Serial.println(maxPwm);
    Serial.println(pwm);
  }
};

class ESCDriver : public MotorDriver
{
public:
  ESCDriver(int _inA, int _inB, int _en);
  void setSpeed(int newSpeed);
  void stop();
  void runTestSequence();
  void moveMotor(int speed);
};

#endif
