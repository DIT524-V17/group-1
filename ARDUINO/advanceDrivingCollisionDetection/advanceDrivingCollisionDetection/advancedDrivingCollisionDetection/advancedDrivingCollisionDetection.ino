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
bool collisionControl = true;             //toggle variable for collision control true by default
char dir;

/*
   A method that splits up the string and set 1. and 2. char of it as speed
*/
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

int distanceTraveled() {
  int carDistance = (odoRight.getDistance() + odoLeft.getDistance()) / 2;
  return carDistance;
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

/*
    Method to read an entire txt file and print it on the monitor
*/
void printTXT() {
  File myFile = SD.open("datalog.txt");

  if (myFile) {

    // read from the file until there's nothing else in it:

    while (myFile.available()) {

      Serial.write(myFile.read());
    }

    // close the file:

    myFile.close();

  } else {

    // if the file didn't open, print an error:

    Serial.println("error opening test.txt");

  }

}

void initialiseSD(int pin) {

  //use this function whenever you want to verify that the SD card is working properly

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:

  if (!SD.begin(pin)) {

    Serial.println("Card failed, or not present");

    // don't do anything more:

    return;
  }

  Serial.println("card initialized.");
}



void writeSD(String command, int distanceTraveled) {

  //use this command when you want to write to the textfile.

  // open the file. note that only one file can be open at a time,

  // so you have to close this one before opening another.

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
      // executes commands default speed is 50
      switch (tmpCMD) {
        case 'w' :
          car.setMotorSpeed(50, 50);
          break;
        case 'a' :
          car.setMotorSpeed(-50, 50);
          break;
        case 's' :
          car.setMotorSpeed(-50, -50);
          break;
        case 'd' :
          car.setMotorSpeed(50, -50);
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
  initialiseSD(SDpin);
  // remove any existing log file to avoid executing the old routes
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
        eStop(extract(), extract());


      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        moveFor(lSpeed, rSpeed);

      }

      break;

    case 's':

      if (d2 > 2 && d2 < 30) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        eStop(extract(), extract());

      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        moveBack(lSpeed, rSpeed);

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
      moveFor(lSpeed, rSpeed);

      dir = 'w';
      break;

    case 'a' :                       //turn in-place to the left (more of a drift in place)

      lSpeed = -extract();
      rSpeed = extract();
      turn(lSpeed, rSpeed);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);

      break;

    case 's' :                        //move backward

      lSpeed = -extract();
      rSpeed = -extract();
      moveBack(lSpeed, rSpeed);

      dir = 's';
      break;

    case 'd' :                      //turn in-place to the right (more of a drift in place)

      lSpeed = extract();
      rSpeed = -extract();
      turn(lSpeed, rSpeed);

      dir = 'd';
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      break;

    case 'q' :                     //diagonal forward left turn

      lSpeed = extract() / 2;
      rSpeed = extract();
      moveFor(lSpeed, rSpeed);

      dir = 'q';
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);

      break;

    case 'e' :                    //diagonal forward right turn

      lSpeed = extract();
      rSpeed = extract() / 2;
      moveFor(lSpeed, rSpeed);

      dir = 'e';
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);

      break;

    case 'z' :                    //diagonal backward left turn

      lSpeed = -(extract() / 2);
      rSpeed = -extract();
      moveBack(lSpeed, rSpeed);

      dir = 'z';
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);

      break;

    case 'c' :                   //diagonal backward right turn

      lSpeed = -extract();
      rSpeed = -(extract() / 2);
      moveBack(lSpeed, rSpeed);

      dir = 'c';
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);

      break;

    case 'x' :                  //Stop
      eStop(extract(), extract());

      dir = 'x';

      break;
    // backtracking case...
    case 'h' :
      BT();
      break;

    //print log file to monitor case
    case 'j' :
      printTXT()
      break;
      
    default :
      break;
  }

  rData = "";
}
