#include <Smartcar.h>

const int TRIGER_ODOL_PIN = 2;
const int TRIGER_ODOR_PIN = 3;
const int TRIGGER_PIN = 6;
const int ECHO_PIN = 5;

Car car;

Odometer odoLeft;
Odometer odoRight;
SR04 US;

int lSpeed;
int rSpeed;
String rData;

// to split up the string and set 1. and 2. char of it as speed
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

void moveFor(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}

void moveBack(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}
void turn(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}
void eStop(int lSpeed, int rSpeed) {
  car.setMotorSpeed(-lSpeed, -rSpeed);
  delay(100);
  car.setMotorSpeed(0, 0);
}

// to send current speed to controller
// maybe it needs to be improved
void avarageSpeed() {
  float avarage = (odoLeft.getSpeed() + odoRight.getSpeed()) / 2;
  Serial.println(avarage);
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

  avarageSpeed();

  if (Serial.available() > 0) {

    while (rData.length() < 3) {
      char m = Serial.read();
      rData += String(m);
    }


    char y = rData.charAt(0);

    switch (y) {
      case 'w' :                         //move forward
        lSpeed = extract();
        rSpeed = extract();
        moveFor(lSpeed, rSpeed);
        break;

      case 's' :                        //move backward
        lSpeed = -extract();
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);
        break;

      case 'a' :                       //turn in-place to the left (more of a drift in place)
        lSpeed = -extract();
        rSpeed = extract();
        turn(lSpeed, rSpeed);
        break;

      case 'd' :                      //turn in-place to the right (more of a drift in place)
        lSpeed = extract();
        rSpeed = -extract();
        turn(lSpeed, rSpeed);
        break;

      case 'q' :                     //diagonal forward left turn
        lSpeed = extract() / 2;
        rSpeed = extract();
        moveFor(lSpeed, rSpeed);
        break;

      case 'e' :                    //diagonal forward right turn
        lSpeed = extract();
        rSpeed = extract() / 2;
        moveFor(lSpeed, rSpeed);
        break;

      case 'z' :                  //diagonal backward left turn
        lSpeed = -(extract() / 2);
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);
        break;

      case 'c' :                 //diagonal backward right turn
        lSpeed = -extract();
        rSpeed = -(extract() / 2);
        moveBack(lSpeed, rSpeed);
        break;

      case 'x' :                //Stop
        eStop(extract(), extract());
        break;

      default :
        Serial.print("The smartCar doesn't move!");
        break;
    }
    rData = "";
  }
}

