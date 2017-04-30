#include <Smartcar.h>
#include <SPI.h> 
#include <SD.h> 

const int TRIGER_ODOL_PIN = 2;
const int TRIGER_ODOR_PIN = 3;
const int TRIGGER_PIN = 6;
const int ECHO_PIN = 5;
const int SDpin = 53; // this is the CS pin on the SD card breakout, change it if you choose a different pin.

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
  //Serial.println(avarage);
}

void initialiseSD(int pin) {
  //use this function whenever you want to verify that the SD card is working properly
  Serial.print("Initializing SD card..."); 
    // see if the card is present and can be initialized: 
    if (!SD.begin(pin)) { Serial.println("Card failed, or not present"); 
    // don't do anything more: 
    return; 
    } 
    Serial.println("card initialized."); 
}

void writeSD(String command) {
  //use this command when you want to write to the textfile.
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(command);
    Serial.println(command);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    } 
}


void setup() {
  Serial.begin(9600);
  US.attach(TRIGGER_PIN, ECHO_PIN);
  car.begin();
  odoLeft.attach(TRIGER_ODOL_PIN);
  odoRight.attach(TRIGER_ODOR_PIN);
  odoLeft.begin();
  odoRight.begin();
  initialiseSD(SDpin);
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
        writeSD("s");
        break;

      case 's' :                        //move backward
        lSpeed = -extract();
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);
        writeSD("w");
        break;

      case 'a' :                       //turn in-place to the left (more of a drift in place)
        lSpeed = -extract();
        rSpeed = extract();
        turn(lSpeed, rSpeed);
        writeSD("d");
        break;

      case 'd' :                      //turn in-place to the right (more of a drift in place)
        lSpeed = extract();
        rSpeed = -extract();
        turn(lSpeed, rSpeed);
        writeSD("a");
        break;

      case 'q' :                     //diagonal forward left turn
        lSpeed = extract() / 2;
        rSpeed = extract();
        moveFor(lSpeed, rSpeed);
        writeSD("c");
        break;

      case 'e' :                    //diagonal forward right turn
        lSpeed = extract();
        rSpeed = extract() / 2;
        moveFor(lSpeed, rSpeed);
        writeSD("z");
        break;

      case 'z' :                  //diagonal backward left turn
        lSpeed = -(extract() / 2);
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);
        writeSD("e");
        break;

      case 'c' :                 //diagonal backward right turn
        lSpeed = -extract();
        rSpeed = -(extract() / 2);
        moveBack(lSpeed, rSpeed);
        writeSD("q");
        break;

      case 'x' :                //Stop
        eStop(extract(), extract());
        writeSD("x");
        break;

      default :
        Serial.print("The smartCar doesn't move!");
        break;
    }
    rData = "";
  }
}








