#include <Smartcar.h>

char rData;

const int TRIGER_ODOL_PIN = 2;
const int TRIGER_ODOR_PIN = 3;
const int TRIGGER_PIN = 6;
const int ECHO_PIN = 5;

Car car;

Odometer odoLeft;
Odometer odoRight;

SR04 US;
int lSpeed = 15;
int rSpeed = 15;

void increaseSpeed(){
  while(lSpeed < 99 && rSpeed < 99){
    lSpeed += 5;
    rSpeed += 5;
    break;
  }
}

void decreaseSpeed(){
  while(lSpeed > 15 && rSpeed > 15){
    lSpeed -= 5;
    rSpeed -= 5;
    break;
  }
}

void avarageSpeed(){
  //float avarage = (odoLeft.getSpeed() + odoRight.getSpeed())/2;
  //Serial.print(avarage);
}
void moveFor(int lSpeed, int rSpeed){
  car.setMotorSpeed(lSpeed, rSpeed);
  avarageSpeed();
  
}

void moveBack(int lSpeed, int rSpeed){
  car.setMotorSpeed(lSpeed, rSpeed);
  avarageSpeed();
}
void turn(int lSpeed, int rSpeed){
  car.setMotorSpeed(lSpeed, rSpeed);
  avarageSpeed();
}
void eStop(int lSpeed, int rSpeed){
  car.setMotorSpeed(-lSpeed, -rSpeed);
  delay(100);
  car.setMotorSpeed(0, 0);
  avarageSpeed();
}

void setup() {
  Serial.begin(9600);
  US.attach(TRIGGER_PIN, ECHO_PIN);
  car.begin();
  odoLeft.attach(TRIGER_ODOL_PIN);
  odoRight.attach(TRIGER_ODOR_PIN);
  odoLeft.begin();
  odoRight.begin();
}

void loop() {
  
  if(Serial.available() > 0) {
    
    rData = Serial.read();

//    if(rData == 'm'){
//      increaseSpeed();
//      Serial.println(lSpeed);
//      moveFor(lSpeed,rSpeed);
//      
//    }
//
//    if(rData == 'k'){
//      decreaseSpeed();
//      Serial.println(lSpeed);
//      moveFor(lSpeed,rSpeed);
//    }
  
    if(rData == 'q'){         //diagonal forward left turn
      lSpeed = 20;
      rSpeed = 50;
      moveFor(lSpeed,rSpeed);
    }
    if(rData == 'e'){       //diagonal forward right turn
      lSpeed = 50;
      rSpeed = 20;
      moveFor(lSpeed, rSpeed);
    }
    if(rData == 'c'){      //diagonal backward right turn
      lSpeed = -50;
      rSpeed = -20;
      moveBack(lSpeed, rSpeed);
    }
    if(rData == 'z'){     //diagonal backward left turn
      lSpeed = -20;
      rSpeed = -50;
      moveBack(lSpeed, rSpeed);
    }
    if(rData == 'w'){                  //move forward
      lSpeed = 50;
      rSpeed = 50;
      moveFor(lSpeed, rSpeed);
    }
    else if(rData == 's'){            //move backward
      lSpeed = -50;
      rSpeed = -50;
      moveBack(lSpeed, rSpeed);
    }
    else if(rData == 'a'){            //turn in-place to the left (more of a drift in place)
      lSpeed = -50;
      rSpeed = 50;
      turn(lSpeed, rSpeed);
    }
    else if(rData == 'd'){            //turn in-place to the right (more of a drift in place)
      lSpeed = 50;
      rSpeed = -50;
      turn(lSpeed, rSpeed);
    }
    else if(rData == 'x'){            //stops
      eStop(lSpeed, rSpeed);
    }
  }
}
