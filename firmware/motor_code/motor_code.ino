#include "PropulsionSystem.h"
#include "MotorDriver.h"
/*
PropulsionSystem* propulsionSystem = NULL;

void setupPropulsionSystem(){
  Serial.println("Setting up Propulsion System");
  if(settings.steeringType == "Rudder"){
    Serial.println("Setting up Rudder Propulsion");
    propulsionSystem = new RudderPropulsion(settings.motor1_pinE,settings.motor1_pinA,settings.motor1_pinB,settings.motor2_pinA);
  }
  else{
    if(settings.motor1_driver == "ESC"){
      Serial.println("Setting up ESC Propulsion");
      propulsionSystem = new EscPropulsion(settings.motor1_pinA,settings.motor1_pinB);
    }
    else{
      Serial.println("Setting up regular Propulsion System");
      propulsionSystem = new PropulsionSystem(settings.motor1_pinE,,settings.motor1_pinA,settings.motor1_pinB,settings.motor2_pinA,settings.motor2_pinB);
    }
  }
  
  
  propulsionSystem->initPins();
}*/

//MotorDriver* motorDriver = NULL;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting Setup");

  testDifferentialPropulsionSystem();
  testMotor();
}

void testDifferentialPropulsionSystem(){
  PropulsionSystem* differentialDrive = new DifferentialDrive(15,13,14,16);//
  
  Serial.println("Testing Propulsion System");
  differentialDrive->setReversed(false,false);
  
  Serial.println("Going Forward");
  for(int i = 0; i < 100; i+=10){
    differentialDrive->setSpeed(i);
    delay(500);
  }
  Serial.println("Slowing Down");
  for(int i = 100; i > 0; i-=10){
    differentialDrive->setSpeed(i);
    delay(500);
  }
  Serial.println("Reversing");
  for(int i = 0; i > -100; i-=10){
    differentialDrive->setSpeed(i);
    delay(500);
  }
  Serial.println("Slowing Down");
  for(int i = -100; i < 0; i+=10){
    differentialDrive->setSpeed(i);
    delay(500);
  }
}

void testMotor(){
  MotorDriver* motorDriver = new HBridgeDriver(15,13);
  Serial.println("Ready");
  motorDriver->setReversed(true);
  
  Serial.println("Going Forward");
  for(int i = 0; i < 100; i+=10){
    motorDriver->setSpeed(i);
    delay(500);
  }
  Serial.println("Slowing Down");
  for(int i = 100; i > 0; i-=10){
    motorDriver->setSpeed(i);
    delay(500);
  }
  Serial.println("Reversing");
  for(int i = 0; i > -100; i-=10){
    motorDriver->setSpeed(i);
    delay(500);
  }
  Serial.println("Slowing Down");
  for(int i = -100; i < 0; i+=10){
    motorDriver->setSpeed(i);
    delay(500);
  }
}

void loop(){
  
}