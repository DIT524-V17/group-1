#include <Smartcar.h>

const int TRIGER_ODOL_PIN = 2;
const int TRIGER_ODOR_PIN = 3;

//Analogue pins below
const int TRIGGER_PIN1 = A11;
const int ECHO_PIN1 = A12;
const int TRIGGER_PIN2 = A13;
const int ECHO_PIN2 = A14;
#define LED A8
Car car;
Odometer odoLeft;
Odometer odoRight;
SR04 US1;                                 //Front US sensor
SR04 US2;                                 //Back US sensor
int lSpeed;
int rSpeed;
String rData;
char dir;

/*
   A method to split up the string and set 1. and 2. char of it as speed
*/
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

/*
   A method for move the car forward
*/
void moveFor(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}

/*
   A method to move the car backward
*/
void moveBack(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}

/*
   A method to turn the car to left and right
*/
void turn(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}

/*
   A method to stop the car.
*/
void eStop(int lSpeed, int rSpeed) {
  car.setMotorSpeed(-lSpeed, -rSpeed);
  delay(100);
  car.setMotorSpeed(0, 0);
}

/*
   A method to send current speed to controller
*/
void avarageSpeed() {
  float avarage = (odoLeft.getSpeed() + odoRight.getSpeed()) / 2;
  Serial.println(avarage);
}

void setup() {
  Serial.begin(9600);
  US1.attach(TRIGGER_PIN1, ECHO_PIN1);
  US2.attach(TRIGGER_PIN2, ECHO_PIN2);
  car.begin();
  odoLeft.attach(TRIGER_ODOL_PIN);
  odoRight.attach(TRIGER_ODOR_PIN);
  odoLeft.begin();
  odoRight.begin();
  pinMode(LED, OUTPUT);
}

void loop() {

  //get current distance from frontal US sensor
  int d1 = US1.getDistance();
  int d2 = US2.getDistance();

  Serial.print("The average speed: " + avarageSpeed());

  /*
     A switch case that makes sure that the car
     will stop according to the car's directions.
     Also, it will turn on the red led if there is any obstacle
  */
  switch (dir) {

    case 'w':

      if (d1 > 2 && d1 < 30) {
        digitalWrite(LED, HIGH);
        eStop(extract(), extract());
      } else {
        digitalWrite(LED, LOW);
        moveFor(lSpeed, rSpeed);
      }
      break;

    case 's':

      if (d2 > 2 && d2 < 30) {
        digitalWrite(LED, HIGH);
        eStop(extract(), extract());
      } else {
        digitalWrite(LED, LOW);
        moveBack(lSpeed, rSpeed);
      }
      break;
  }

  if (Serial.available() > 0) {

    while (rData.length() < 3) {
      char m = Serial.read();
      rData += String(m);
    }
    char y = rData.charAt(0);

    /*
          A switch case that controls the car movement
    */
    switch (y) {

      case 't' :                                                  //Turn collision detection on/off
        colToggle(collisionControl);

      case 'w' :                                                 //Move forward

        lSpeed = extract();
        rSpeed = extract();
        dir = 'w';
        break;

      case 'a' :                                                //Turn in-place to the left (more of a drift in place)

        lSpeed = -extract();
        rSpeed = extract();
        turn(lSpeed, rSpeed);
        break;

      case 's' :                                               //Move backward

        lSpeed = -extract();
        rSpeed = -extract();
        dir = 's';
        break;

      case 'd' :                                              //Turn in-place to the right (more of a drift in place)

        lSpeed = extract();
        rSpeed = -extract();
        turn(lSpeed, rSpeed);
        break;

      case 'q' :                                             //Diagonal forward left turn

        lSpeed = extract() / 2;
        rSpeed = extract();
        moveFor(lSpeed, rSpeed);
        break;

      case 'e' :                                            //Diagonal forward right turn

        lSpeed = extract();
        rSpeed = extract() / 2;
        moveFor(lSpeed, rSpeed);
        break;

      case 'z' :                                           //Diagonal backward left turn

        lSpeed = -(extract() / 2);
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);
        break;

      case 'c' :                                          //Diagonal backward right turn

        lSpeed = -extract();
        rSpeed = -(extract() / 2);
        moveBack(lSpeed, rSpeed);
        break;

      case 'x' :                                        //Stop the car
        eStop(extract(), extract());
        dir = 'x';
        break;

      default :

        Serial.print("The smartCar doesn't move!");
        break;
    }
    rData = "";
  }
}
