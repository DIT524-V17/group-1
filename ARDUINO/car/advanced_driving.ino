#include <Smartcar.h>

char rData;

const int TRIGGER_PIN = 6;
const int ECHO_PIN = 5;

Car car;
SR04 US;

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
      moveFor(20, 50);
    }
    if(rData == 'e'){       //diagonal forward right turn
      moveFor(50, 20);
    }
    if(rData == 'c'){      //diagonal backward right turn
      moveBack(-50, -20);
    }
    if(rData == 'z'){     //diagonal backward left turn
      moveBack(-20, -50);
    }
    if(rData == 'w'){                  //move forward
      moveFor(50, 50);
    }
    else if(rData == 's'){            //move backward
      moveBack(-50, -50);
    }
    else if(rData == 'a'){            //turn in-place to the left (more of a drift in place)
      turn(-50, 50);
    }
    else if(rData == 'd'){            //turn in-place to the right (more of a drift in place)
      turn(50, -50);
    }
    else if(rData == 'x'){            //turn in-place to the right (more of a drift in place)
      turn(0, 0);
    }
  }
}
