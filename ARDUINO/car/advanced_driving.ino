#include <Smartcar.h>

char rData;

const int TRIGGER_PIN = 6;
const int ECHO_PIN = 5;

Car car;
SR04 US;
int lSpeed;
int rSpeed;

void setup() {
  Serial.begin(9600);
  US.attach(TRIGGER_PIN, ECHO_PIN);
  car.begin();
}

void moveFor(int lSpeed, int rSpeed){
  car.setMotorSpeed(lSpeed, rSpeed);
}

void moveBack(int lSpeed, int rSpeed){
  car.setMotorSpeed(lSpeed, rSpeed);
}
void turn(int lSpeed, int rSpeed){
  car.setMotorSpeed(lSpeed, rSpeed);
}
void eStop(int lSpeed, int rSpeed){
  car.setMotorSpeed(-lSpeed, -rSpeed);
  delay(100);
  car.setMotorSpeed(0, 0);
}

void loop() {
  
  if(Serial.available() > 0) {
    
    rData = Serial.read();
  
    if(rData == 'q'){         //diagonal forward left turn
      int lSpeed = 20;
      int rSpeed = 50;
      moveFor(lSpeed,rSpeed);
    }
    if(rData == 'e'){       //diagonal forward right turn
      int lSpeed = 50;
      int rSpeed = 20;
      moveFor(lSpeed, rSpeed);
    }
    if(rData == 'c'){      //diagonal backward right turn
      int lSpeed = -50;
      int rSpeed = -20;
      moveBack(lSpeed, rSpeed);
    }
    if(rData == 'z'){     //diagonal backward left turn
      int lSpeed = -20;
      int rSpeed = -50;
      moveBack(lSpeed, rSpeed);
    }
    if(rData == 'w'){                  //move forward
      int lSpeed = 50;
      int rSpeed = 50;
      moveFor(lSpeed, rSpeed);
    }
    else if(rData == 's'){            //move backward
      int lSpeed = -50;
      int rSpeed = -50;
      moveBack(lSpeed, rSpeed);
    }
    else if(rData == 'a'){            //turn in-place to the left (more of a drift in place)
      int lSpeed = -50;
      int rSpeed = 50;
      turn(lSpeed, rSpeed);
    }
    else if(rData == 'd'){            //turn in-place to the right (more of a drift in place)
      int lSpeed = 50;
      int rSpeed = -50;
      turn(lSpeed, rSpeed);
    }
    else if(rData == 'x'){            //turn in-place to the right (more of a drift in place)
      eStop(lSpeed, rSpeed);
    }
  }
}
