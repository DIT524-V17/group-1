#include <Smartcar.h>

//char rData;

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
char rData[3];
int S;

void increaseSpeed() {
  while (lSpeed < 99 && rSpeed < 99) {
    lSpeed += 5;
    rSpeed += 5;
    break;
  }
}

void decreaseSpeed() {
  while (lSpeed > 15 && rSpeed > 15) {
    lSpeed -= 5;
    rSpeed -= 5;
    break;
  }
}
void extract() {
  String Stemp = rData[1] + rData[2] + "";
  int S = Stemp.toInt();
  //return S;
  //for (int i = 0; i < 3; i = i ++) {
  //rData[i] = Serial.read();
}


void avarageSpeed() {
  //float avarage = (odoLeft.getSpeed() + odoRight.getSpeed())/2;
  //Serial.print(avarage);
}
void moveFor(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
  avarageSpeed();

}

void moveBack(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
  avarageSpeed();
}
void turn(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
  avarageSpeed();
}
void eStop(int lSpeed, int rSpeed) {
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

  if (Serial.available() > 0) {


    for (int i = 0; i < 3; i = i ++) {
      rData[i] = Serial.read();
    }
    char y = rData[0];
    extract();
    switch (y) {
      case 'w' :                         //move forward
        lSpeed = S;
        rSpeed = S;
        moveFor(lSpeed, rSpeed);
        break;

      case 's' :                        //move backward
        lSpeed = -S;
        rSpeed = -S;
        moveBack(lSpeed, rSpeed);
        break;

      case 'a' :                       //turn in-place to the left (more of a drift in place)
        lSpeed = -S;
        rSpeed = S;
        turn(lSpeed, rSpeed);
        break;

      case 'd' :                      //turn in-place to the right (more of a drift in place)
        lSpeed = S;
        rSpeed = -S;
        turn(lSpeed, rSpeed);
        break;

      case 'q' :                     //diagonal forward left turn
        lSpeed = S / 2;
        rSpeed = S;
        moveFor(lSpeed, rSpeed);
        break;

      case 'e' :                    //diagonal forward right turn
        lSpeed = S;
        rSpeed = S / 2;
        moveFor(lSpeed, rSpeed);
        break;


      case 'z' :                  //diagonal backward left turn
        lSpeed = -(S / 2);
        rSpeed = -S;
        moveBack(lSpeed, rSpeed);
        break;

      case 'c' :                 //diagonal backward right turn
        lSpeed = -S;
        rSpeed = -(S / 2);
        moveBack(lSpeed, rSpeed);
        break;

      case 'x' :                //Stop
        eStop(lSpeed, rSpeed);
        break;

      default :
        Serial.print("The smartCar doesn't move!");
        break;



    }
  }
}
/* if(rData[0] == 'q'){         //diagonal forward left turn
   lSpeed = S/2;
   rSpeed = S;
   moveFor(lSpeed,rSpeed);

  }
  if(rData[0] == 'e'){       //diagonal forward right turn
   lSpeed = S;
   rSpeed = S/2;
   moveFor(lSpeed, rSpeed);
  }
  if(rData[0] == 'c'){      //diagonal backward right turn
   lSpeed = -S;
   rSpeed = -(S/2);
   moveBack(lSpeed, rSpeed);
  }
  if(rData[0] == 'z'){     //diagonal backward left turn
   lSpeed = -(S/2);
   rSpeed = -S;
   moveBack(lSpeed, rSpeed);
  }
  if(rData[0]== 'w'){                  //move forward
   lSpeed = S;
   rSpeed = S;
   moveFor(lSpeed, rSpeed);
  }
  else if(rData[0] == 's'){            //move backward
   lSpeed = -S;
   rSpeed = -S;
   moveBack(lSpeed, rSpeed);
  }
  else if(rData[0] == 'a'){            //turn in-place to the left (more of a drift in place)
   lSpeed = -S;
   rSpeed = S;
   turn(lSpeed, rSpeed);
  }
  else if(rData[0] == 'd'){            //turn in-place to the right (more of a drift in place)
   lSpeed = S;
   rSpeed = -S;
   turn(lSpeed, rSpeed);
  }
  else if(rData[0] == 'x'){            //stops
   eStop(lSpeed, rSpeed);
  }
  }
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
  }*/

