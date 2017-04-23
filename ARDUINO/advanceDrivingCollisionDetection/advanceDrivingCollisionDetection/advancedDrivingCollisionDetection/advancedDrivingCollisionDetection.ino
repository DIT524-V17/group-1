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
bool collisionControl = true;             //toggle variable for collision control true by default
char dir;

// to split up the string and set 1. and 2. char of it as speed
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

void colToggle(bool collisionCotrol) {
  collisionControl = !collisionControl;
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

  int d1 = US1.getDistance();               //get current distance from frontal US sensor
  int d2 = US2.getDistance();

  boolean frontStop = false;
  boolean backStop = false;

  char direction = 'n';

  //avarageSpeed();
  Serial.print(d1);
  Serial.print(":");
  Serial.println(d2);


  switch (dir) {                            // A switch case that makes sure that the car will stop according to the car's directions

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

    switch (y) {
      case 't' :                         //turn collision detection on/off
        colToggle(collisionControl);

      case 'w' :                         //move forward

        lSpeed = extract();
        rSpeed = extract();
        //moveFor(lSpeed, rSpeed);

        dir = 'w';

        break;

      case 'a' :                       //turn in-place to the left (more of a drift in place)

        lSpeed = -extract();
        rSpeed = extract();
        turn(lSpeed, rSpeed);

        break;

      case 's' :                        //move backward

        lSpeed = -extract();
        rSpeed = -extract();
        //moveBack(lSpeed, rSpeed);

        dir = 's';

        break;



      case 'd' :                      //turn in-place to the right (more of a drift in place)

        lSpeed = extract();
        rSpeed = -extract();
        turn(lSpeed, rSpeed);

        dir = 'd';

        break;

      case 'q' :                     //diagonal forward left turn

        lSpeed = extract() / 2;
        rSpeed = extract();
        moveFor(lSpeed, rSpeed);

        dir = 'q';

        break;

      case 'e' :                    //diagonal forward right turn

        lSpeed = extract();
        rSpeed = extract() / 2;
        moveFor(lSpeed, rSpeed);

        dir = 'e';

        break;

      case 'z' :                    //diagonal backward left turn

        lSpeed = -(extract() / 2);
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);

        dir = 'z';

        break;

      case 'c' :                   //diagonal backward right turn

        lSpeed = -extract();
        rSpeed = -(extract() / 2);
        moveBack(lSpeed, rSpeed);

        dir = 'c';

        break;

      case 'x' :                  //Stop
        eStop(extract(), extract());
        dir = 'x';

        break;

      default :
        direction = 'n';
        Serial.print("The smartCar doesn't move!");


        break;
    }

    rData = "";

  }
}
