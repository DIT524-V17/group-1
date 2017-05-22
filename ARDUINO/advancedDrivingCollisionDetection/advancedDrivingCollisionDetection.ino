#include <Smartcar.h>
#include <SPI.h>
#include <SD.h>

const int TRIGER_ODOL_PIN = 2;
const int TRIGER_ODOR_PIN = 3;

/*
   Analogue pins below
*/
const int TRIGGER_PIN1 = A11;              // Front Sensor
const int ECHO_PIN1 = A12;                 // Front Sensor
const int TRIGGER_PIN2 = A13;              // Back Sensor
const int ECHO_PIN2 = A14;                 // Back Sensor
const int SDpin = 53;                      // this is the CS pin on the SD card breakout, change it if you choose a different pin.
#define LED1 A9                            // Obstacle detection LED (RED)
#define LED2 A7                            // Forward LED (GREEN)
#define LED3 A8                            // Backward LED (RED) 
#define LED4 A5                            // Turn left LED (BLUE)
#define LED5 A10                           // Turn right LED (BLUE)

Car car;

/*
  Left and right Odometers
*/
Odometer odoLeft;
Odometer odoRight;

/*
  Front and back US sensors
*/
SR04 US1;
SR04 US2;

int lSpeed;
int rSpeed;
String rData;
char dir;



void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);                         //For XBOX controller
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
  // remove any existing log file to avoid executing the old routes
  SD.remove("datalog.txt");
}

void loop() {

  /*
    Get current distance from front and back US sensors
  */
  int d1 = US1.getDistance();
  int d2 = US2.getDistance();

  avarageSpeed();                           // Display the average speed

  /*
     Get the direction to drive from the XBOX controller
  */
  if (Serial2.available() > 0) {

    while (rData.length() < 3) {
      char m = Serial2.read(); //add clause for using both serials for debugging

      rData += String(m);

    }
  }

  /*
      Get the direction to drive from the Android/PC controller
  */
  else if (Serial.available() > 0) {

    while (rData.length() < 1) {
      char m = Serial.read(); //add clause for using both serials for debugging

      if ( m == 'x') {
        rData = String(m) + "00";

      }
      else {
        rData = String(m) + "50";
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
  }

  /*
       A switch case that controls the car movement
  */
  char y = rData.charAt(0);
  // if y is not null we are saving the commands and the distance has been traveled to the file
  if (y != '\0' && y != 'h') {
    writeSD(String(y), distanceTraveled());
  }

  switch (y) {

    case 'w' :                         //move forward

      lSpeed = extract();
      rSpeed = extract();
      car.setMotorSpeed(extract(), extract());
      dir = 'w';
      break;

    case 'a' :                       //turn in-place to the left (more of a drift in place)

      lSpeed = -extract();
      rSpeed = extract();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      car.setMotorSpeed(-extract(), extract());
      dir = 'a';

      break;

    case 's' :                        //move backward

      lSpeed = -extract();
      rSpeed = -extract();
      car.setMotorSpeed(-extract(), -extract());
      dir = 's';
      break;

    case 'd' :                      //turn in-place to the right (more of a drift in place)

      lSpeed = extract();
      rSpeed = -extract();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      car.setMotorSpeed(extract(), -extract());
      dir = 'd';

      break;

    case 'q' :                     //diagonal forward left turn

      lSpeed = extract() / 2;
      rSpeed = extract();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      car.setMotorSpeed(extract() / 2, extract());
      dir = 'q';


      break;

    case 'e' :                    //diagonal forward right turn

      lSpeed = extract();
      rSpeed = extract() / 2;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      car.setMotorSpeed(extract(), extract() / 2);
      dir = 'e';
      break;

    case 'z' :                    //diagonal backward left turn

      lSpeed = -(extract() / 2);
      rSpeed = -extract();
      digitalWrite(LED1, LOW);
      digitalWrite(LED1, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      car.setMotorSpeed(-extract() / 2, -extract());;
      dir = 'z';
      break;

    case 'c' :                   //diagonal backward right turn

      lSpeed = -extract();
      rSpeed = -(extract() / 2);
      digitalWrite(LED1, LOW);
      digitalWrite(LED1, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      car.setMotorSpeed(-extract(), -extract() / 2);
      dir = 'c';
      break;

    case 'x' :                  //Stop
      car.stop();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      dir = 'x';
      break;

    // backtracking case...
    case 'h' :
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

/*
    A Method that writes the entered commands and the the traveled distances to the textfile.
    Note: Use this command when you want to write to the textfile.
    Open the file. note that only one file can be open at a time,
    so you have to close this one before opening another.
*/
void writeSD(String command, int distanceTraveled) {

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

/*
    A Method that is responsible of reversing the car's track
*/
void BT() {
  File myFile = SD.open("datalog.txt");
  // declare a variable to get the number of lines in the text file
  int i = 0;
  // declare a variable to save the total distance during traveling
  int tatalDistance;
  // open the file to count the lines
  while (myFile.available()) {
    String list = myFile.readStringUntil('\n');
    tatalDistance = list.substring(1).toInt();
    i++;
  }
  myFile.close();
  // create 2 arrays with the length of the text file's lines, save commands and distance should be taken
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
      car.setMotorSpeed(50, -50);
    }
    else {
      car.stop();
      break;
    }
  }
  delay(1000);
  car.rotate(180);
  // read everythings in arrays
  for (int m = 0; m < sizeof(BTcommands); m++) {
    // since odometer values are increasing, i am resetting it by taking the modulus of total distance
    // and i am not intressed in x commands that stops the car..
    while ( (distanceTraveled() % tatalDistance) < BTdistance[m] && BTcommands[m] != 'x') {
      char tmpCMD = BTcommands[m];
      // if we reach the total distance that is saved, then the car stops
      if ((distanceTraveled() % tatalDistance) + 1 >= BTdistance[sizeof(BTcommands) - 1]) {
        car.stop();
        break;

      }

      /*
         Get current distance from front and back US sensors
      */
      int d1 = US1.getDistance();
      int d2 = US2.getDistance();

      /*
         Executes commands default speed is 50
      */
      switch (tmpCMD) {
        case 'w' :
          if (d1 > 2 && d1 < 30) {
            digitalWrite(LED1, HIGH);
            car.stop();
          } else {
            digitalWrite(LED1, LOW);
            car.setMotorSpeed(50, 50);
          }
          break;
        case 'a' :
          car.setMotorSpeed(50, -50);
          break;
        case 's' :
          if (d2 > 2 && d2 < 30) {
            digitalWrite(LED1, HIGH);
            car.stop();
          } else {
            digitalWrite(LED1, LOW);
            car.setMotorSpeed(-50, -50);
          }
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
  // after we are done with backtracking, remove the saved file..
  SD.remove("datalog.txt");
}
