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
const int SDpin = 53;                      // this is the CS pin on the SD card breakout, change it if you choose a different pin.
#define LED1 A9                             // Obstacle detection LED (RED)
#define LED2 A7                             // Forward LED (GREEN)
#define LED3 A8                             // Backward LED (RED) 
#define LED4 A5                             // Turn left LED (BLUE)
#define LED5 A10                            // Turn right LED (BLUE)

Car car;

Odometer odoLeft;
Odometer odoRight;

SR04 US1;                                 //Front US sensor
SR04 US2;                                 //Back US sensor

int lSpeed;
int rSpeed;
String rData;
char dir;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  US1.attach(TRIGGER_PIN1, ECHO_PIN1);
  US2.attach(TRIGGER_PIN2, ECHO_PIN2);
  car.begin();
  odoLeft.attach(TRIGER_ODOL_PIN);
  odoRight.attach(TRIGER_ODOR_PIN);
  odoLeft.begin();
  odoRight.begin();

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  SD.begin(SDpin);
  SD.remove("datalog.txt");

}
void loop() {
  int d1 = US1.getDistance();               //get current distance from frontal US sensor
  int d2 = US2.getDistance();
  if (Serial2.available() > 0) {
    while (rData.length() < 3) {
      char m = Serial2.read(); //add clause for using both serials for debugging
      rData += String(m);
    }

  }
  else if (Serial.available() > 0) {
    while (rData.length() < 1) {
      char m = Serial.read(); //add clause for using both serials for debugging
      if ( m == 'x') {
        rData = String(m) + "00";
      }
      else {
        rData = String(m) + "70";
      }
    }
  }

  /*
      A switch case that makes sure that the car
      will stop according to the car's directions.
      Also, it will turn on the red led if there is any obstacle
  */
  switch (dir) {
    case 'w':
      if (d1 > 2 && d1 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(0, 0);


      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(lSpeed, rSpeed);

      }

      break;

    case 's':


      if (d2 > 2 && d2 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(0, 0);

      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(lSpeed, rSpeed);

      }

      break;

    case 'q':

      if (d1 > 2 && d1 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(0, 0);


      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(lSpeed, rSpeed);

      }

      break;

    case 'e':

      if (d1 > 2 && d1 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(0, 0);


      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, HIGH);
        car.setMotorSpeed(lSpeed, rSpeed);

      }

      break;

    case 'z':

      if (d2 > 2 && d2 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(0, 0);


      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(lSpeed, rSpeed);

      }

      break;

    case 'c':
      if (d2 > 2 && d2 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        car.setMotorSpeed(0, 0);


      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, HIGH);
        car.setMotorSpeed(lSpeed, rSpeed);

      }

      break;
  }

  /*
       A switch case that controls the car movement
  */

  char y = rData.charAt(0);

  if (y != '\0' && y != 'h') {
    writeSD(String(y), distanceTraveled());
  }

  switch (y) {

    case 'w' :                                             //move forward

      lSpeed = extract();
      rSpeed = extract();
      dir = 'w';
      car.setMotorSpeed(extract(), extract());
      break;

    case 'a' :                                             //turn in-place to the left (more of a drift in place)

      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      dir = 'a';
      car.setMotorSpeed(-extract(), extract());
      break;

    case 's' :                                             //move backward

      lSpeed = -extract();
      rSpeed = -extract();
      dir = 's';
      car.setMotorSpeed(-extract(), -extract());
      break;

    case 'd' :                                             //turn in-place to the right (more of a drift in place)

      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      dir = 'd';
      car.setMotorSpeed(extract(), -extract());
      break;

    case 'q' :                                             //diagonal forward left turn

      lSpeed = extract() / 2;
      rSpeed = extract();
      dir = 'q';
      car.setMotorSpeed(extract() / 2, extract());
      break;

    case 'e' :                                             //diagonal forward right turn


      lSpeed = extract();
      rSpeed = extract() / 2;
      dir = 'e';
      car.setMotorSpeed(extract(), extract() / 2);
      break;

    case 'z' :                                             //diagonal backward left turn


      lSpeed = -extract() / 2;
      rSpeed = -extract();
      dir = 'z';
      car.setMotorSpeed(-extract() / 2, -extract());
      break;

    case 'c' :                                            //diagonal backward right turn


      lSpeed = -extract();
      rSpeed = -extract() / 2;
      dir = 'c';
      car.setMotorSpeed(-extract(), -extract() / 2);
      break;

    case 'x' :                                            //Stop

      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      car.setMotorSpeed(0, 0);
      dir = 'x';
      break;

    case 'h' :                  //BackTracking

      BT();
      break;

    default :
      break;
  }
  rData = "";
}

/*
   A method that splits up the string and set 1. and 2. char of it as speed
*/
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

/*
   A method that returns the average traveled distance of the car
*/

int distanceTraveled() {
  int carDistance = (odoRight.getDistance() + odoLeft.getDistance()) / 2;
  return carDistance;
}

/*
   A method that sends current speed to controller
*/
void avarageSpeed() {
  float avarage = (odoLeft.getSpeed() + odoRight.getSpeed()) / 2;
  Serial.println(avarage);
}

void writeSD(String command, int distanceTraveled) {

  /* use this command when you want to write to the textfile.
     open the file. note that only one file can be open at a time,
     so you have to close this one before opening another.
  */

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:

  if (dataFile) {
    dataFile.println(command + distanceTraveled);
    dataFile.close();
  }

  // if the file isn't open, pop up an error:
  else {

    Serial.println("error opening datalog.txt");
  }
}

void BT() {
  // create a object of file
  File myFile = SD.open("datalog.txt");
  // declare a variable to get number of lines in text file
  int i = 0;
  // declare a variable to save total distance during the travel
  int tatalDistance;
  // open the file for count of lines
  while (myFile.available()) {
    String list = myFile.readStringUntil('\n');
    tatalDistance = list.substring(1).toInt() + 43;
    i++;
  }
  myFile.close();

  // create 2 arrays of length of count of text file lines save commands and distance should be taken
  char BTcommands[i];
  int BTdistance[i];

  // decrease "i" because i am not intrested in the last value anymore
  i--;
  File myFile1 = SD.open("datalog.txt");
  // put everything into arrays in reverse order..
  while (myFile1.available()) {
    String list = myFile1.readStringUntil('\n');
    BTcommands[i] = list.charAt(0);
    // to find the distance has been taken just for this command after the command before this
    BTdistance[i] = tatalDistance - list.substring(1).toInt();
    i--;
  }
  myFile1.close();

  // move the car 180 degrees
  while (true) {
    if (distanceTraveled() < tatalDistance) {
      car.setMotorSpeed(80, -80);
    }
    else {
      car.stop();
      break;
    }
  }
  delay(1000);
  // read everythings in arrays
  for (int m = 0; m < sizeof(BTcommands); m++) {
    // since odometer values increas so i am resetting it by take modulu by total distance have saved before
    while ( (distanceTraveled() % tatalDistance) < BTdistance[m] && BTcommands[m] != 'x') {
      char tmpCMD = BTcommands[m];
      if ((distanceTraveled() % tatalDistance) + 1 >= (BTdistance[sizeof(BTcommands) - 1] - 43) && BTcommands[0] == 'x') {
        car.stop();
        SD.remove("datalog.txt");
        break;

      }

      switch (tmpCMD) {
        case 'w' :
          car.setMotorSpeed(50, 50);
          break;
        case 'a' :
          car.setMotorSpeed(50, -50);
          break;
        case 's' :
          car.setMotorSpeed(-50, -50);
          break;
        case 'd' :
          car.setMotorSpeed(-50, 50);
          break;
        case 'q' :
          car.setMotorSpeed(25, 50);
          break;
        case 'e' :
          car.setMotorSpeed(50, 25);
          break;
        case 'z' :
          car.setMotorSpeed(-25, -50);
          break;
        case 'c' :
          car.setMotorSpeed(-50, -25);
          break;
        default :
          break;
      }
    }
  }
  SD.remove("datalog.txt");
}
