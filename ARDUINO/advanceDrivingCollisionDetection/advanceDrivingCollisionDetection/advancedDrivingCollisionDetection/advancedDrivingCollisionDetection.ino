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
int totalDistance = 0;

// to split up the string and set 1. and 2. char of it as speed
int extract() {
  int carSpeed = rData.substring(1, 3).toInt();
  return carSpeed;
}

int distanceTraveled() {
  int carDistance = (odoRight.getDistance() + odoLeft.getDistance()) /2;
  return carDistance;
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


      

    distanceTraveled = (odoRight.getDistance() + odoLeft.getDistance()) /2;
    dataFile.println(command + distanceTraveled);

    Serial.println(command + distanceTraveled);



    dataFile.close();

  }

  // if the file isn't open, pop up an error:

  else {

    Serial.println("error opening datalog.txt");



  }



}





//void startRetracing() {
//
//  File myFile = SD.open("datalog.txt");
//
//  if (myFile) {
//
//    // read from the file until there's nothing else in it:
//
//    while (myFile.available()) {
//
//      Serial.write(myFile.seek(0));
//
//      Serial.write(myFile.read());
//
//    }
//
//    // close the file:
//
//    myFile.close();
//
//  } else {
//
//    // if the file didn't open, print an error:
//
//    Serial.println("error opening test.txt");
//
//  }
//
//}




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
  pinMode(LED, OUTPUT);
  initialiseSD(SDpin);

}

void loop() {

  int d1 = US1.getDistance();               //get current distance from frontal US sensor
  int d2 = US2.getDistance();

  boolean frontStop = false;
  boolean backStop = false;

  char direction = 'n';

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

  char y = rData.charAt(0);

  switch (y) {
    case 't' :                         //turn collision detection on/off
      colToggle(collisionControl);

    case 'w' :                         //move forward

      lSpeed = extract();
      rSpeed = extract();
      moveFor(lSpeed, rSpeed);
      
      dir = 'w';
      
 //     Serial.print();
      writeSD("s", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;
      break;

    case 'a' :                       //turn in-place to the left (more of a drift in place)

      lSpeed = -extract();
      rSpeed = extract();
      turn(lSpeed, rSpeed);
      writeSD("d", distanceTraveled);// ? take care of rotations 
  
      break;

    case 's' :                        //move backward

      lSpeed = -extract();
      rSpeed = -extract();
      moveBack(lSpeed, rSpeed);

      dir = 's';
      writeSD("w", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;
      break;

    case 'd' :                      //turn in-place to the right (more of a drift in place)

      lSpeed = extract();
      rSpeed = -extract();
      turn(lSpeed, rSpeed);
      
      dir = 'd';
      writeSD("a", distanceTraveled);
      break;

    case 'q' :                     //diagonal forward left turn

      lSpeed = extract() / 2;
      rSpeed = extract();
      moveFor(lSpeed, rSpeed);

      dir = 'q';
      writeSD("c", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;
      
      break;

    case 'e' :                    //diagonal forward right turn

      lSpeed = extract();
      rSpeed = extract() / 2;
      moveFor(lSpeed, rSpeed);

      dir = 'e';
      writeSD("z", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;
      
      break;

    case 'z' :                    //diagonal backward left turn

      lSpeed = -(extract() / 2);
      rSpeed = -extract();
      moveBack(lSpeed, rSpeed);

      dir = 'z';
      writeSD("e", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;

      break;

    case 'c' :                   //diagonal backward right turn

      lSpeed = -extract();
      rSpeed = -(extract() / 2);
      moveBack(lSpeed, rSpeed);

      dir = 'c';
      writeSD("q", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;

      break;

    case 'x' :                  //Stop
      eStop(extract(), extract());
      
      dir = 'x';
      writeSD("x", distanceTraveled-totalDistance);
      totalDistance = distanceTraveled;

      break;

     case 'h' :                  //Stop
      
      printTXT();

      break;
      
    default :
      direction = 'n';
      break;
  }

  rData = "";
}
