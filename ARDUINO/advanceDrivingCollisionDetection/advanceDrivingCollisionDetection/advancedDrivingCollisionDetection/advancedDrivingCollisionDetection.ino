#include <Smartcar.h>
#include <SPI.h>
#include <SD.h>

const int TRIGER_ODOL_PIN = 2;
const int TRIGER_ODOR_PIN = 3;

//Analogue pins below
const int TRIGGER_PIN1 = A11;
const int ECHO_PIN1 = A12;
const int TRIGGER_PIN2 = A13;
const int ECHO_PIN2 = A14;

#define LED1 A9                            // ODS LED (RED)
#define LED2 A10                           // Forward LED (BLUE)
#define LED3 A8                            // Backward LED (RED)
#define LED4 A7                            // Turn right LED (GREEN)
#define LED5 A5                            // Turn left LED (GREEN)

Car car;
Odometer odoLeft, odoRight;                //Odometers left and right
SR04 US1, US2;                             //Front and Back US sensors

int lSpeed, rSpeed;
String rData;
char dir;

/*
   A method that splits up the string and set 1. and 2. char of it as speed
*/
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

/*
   A method that moves the car forward
*/
void moveFor(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}
/*
   A method that moves the car backward
*/
void moveBack(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}
/*
   A method that turns the car to left and right
*/
void turn(int lSpeed, int rSpeed) {
  car.setMotorSpeed(lSpeed, rSpeed);
}
/*
   A method that stops the car.
*/
void eStop(int lSpeed, int rSpeed) {
  car.setMotorSpeed(-lSpeed, -rSpeed);
  delay(100);
  car.setMotorSpeed(0, 0);
}
/*
   A method that sends current speed to controller
*/
void avarageSpeed() {
  float avarage = (odoLeft.getSpeed() + odoRight.getSpeed()) / 2;
  Serial.println(avarage);
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600); //For XBOX controller

  US1.attach(TRIGGER_PIN1, ECHO_PIN1);
  US2.attach(TRIGGER_PIN2, ECHO_PIN2);
  car.begin();
  odoLeft.attach(TRIGER_ODOL_PIN);
  odoRight.attach(TRIGER_ODOR_PIN);
  odoLeft.begin();
  odoRight.begin();

  //LED warning for obstacle detection system (ODS)
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

}

void loop() {
  //get current distance from front and back US sensors
  int d1 = US1.getDistance();
  int d2 = US2.getDistance();

  //Get the direction to drive from the Android/PC controller
  if (Serial.available() > 0) {

    while (rData.length() < 3) {
      rData = String(Serial.read()); //add clause for using both serials for debugging
    }
  }

  //Get the direction to drive from the XBOX controller
  if (Serial2.available() > 0) {

    while (rData.length() < 3) {
      rData = String(Serial2.read()); //add clause for using both serials for debugging
    }
  }

  /*
        A switch case that controls the car movement
  */
  char y = rData.charAt(0);
  
  switch (y) {
    case 'w' :                                                 //Move forward
      lSpeed = extract();
      rSpeed = extract();

      if (d1 > 2 && d1 < 30) {
        eStop(extract(), extract());

        //ODS
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
      } else {
        moveFor(lSpeed, rSpeed);

        //ODS        
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
      }
      
      break;

    case 'a' :                                                //Turn in-place to the left (more of a drift in place)
      lSpeed = -extract();
      rSpeed = extract();
      turn(lSpeed, rSpeed);

      //ODS      
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      
      break;

    case 's' :                                               //Move backward
      lSpeed = -extract();
      rSpeed = -extract();

      if (d2 > 2 && d2 < 30) {
        eStop(extract(), extract());

        //ODS
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        
      } else {
        moveBack(lSpeed, rSpeed);

        //ODS
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        
      }
      break;

      break;

    case 'd' :                                              //Turn in-place to the right (more of a drift in place)
      lSpeed = extract();
      rSpeed = -extract();
      turn(lSpeed, rSpeed);

      //ODS      
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      
      break;

    case 'q' :                                             //Diagonal forward left turn
      lSpeed = extract() / 2;
      rSpeed = extract();
      moveFor(lSpeed, rSpeed);

      //ODS
      digitalWrite(LED1, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED4, LOW);
      
      break;

    case 'e' :                                            //Diagonal forward right turn
      lSpeed = extract();
      rSpeed = extract() / 2;
      moveFor(lSpeed, rSpeed);

      //ODS
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      
      break;

    case 'z' :                                           //Diagonal backward left turn
      lSpeed = -(extract() / 2);
      rSpeed = -extract();
      moveBack(lSpeed, rSpeed);

      //ODS
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      
      break;

    case 'c' :                                          //Diagonal backward right turn
      lSpeed = -extract();
      rSpeed = -(extract() / 2);
      moveBack(lSpeed, rSpeed);

      //ODS
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      
      break;

    case 'x' :                                        //Stop the car
      eStop(extract(), extract());

      //ODS
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      
      break;
  }
}


