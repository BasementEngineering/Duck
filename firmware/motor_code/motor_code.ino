#include "PropulsionSystem.h"
//#include "RudderPropulsion.h"
//#include "EscPropulsion.h"

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

MotorDriver* motorDriver = NULL;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting Setup");

  //setupPropulsionSystem();
  motorDriver = new HBridgeDriver(12,13,14);
  Serial.println("Ready");
}

void loop(){
  for(int i = 0; i < 100; i+=10){
  motorDriver->setSpeed(i);
  delay(1000);
  }
  for(int i = 100; i > -100; i-=10){
  motorDriver->setSpeed(i);
  delay(1000);
  }
  for(int i = -100; i < 0; i+=10){
  motorDriver->setSpeed(i);
  delay(1000);
  }
}