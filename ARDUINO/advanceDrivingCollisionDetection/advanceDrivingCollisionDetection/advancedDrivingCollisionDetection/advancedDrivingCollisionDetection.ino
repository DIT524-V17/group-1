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
const int SDpin = 53; // this is the CS pin on the SD card breakout, change it if you choose a different pin.
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
      Serial2.println("Card failed, or not present");
    // don't do anything more: 
    return; 
    } 
    Serial2.println("card initialized.");
    
}

void writeSD(String command, int speed) {
  //use this command when you want to write to the textfile.
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    speed = extract();
    dataFile.println(command + speed);
    Serial.println(command + speed);
    
    
    
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    } 
}

void startRetracing() {
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

  //add handshake function here 
  // waitForApp() for (x == 1) { check if bluetooth is paired properly, 
  // eg : Serial2.print("Hello");
  //  delay(200);      
  //if (Serial2.available() > 0 ) { break; }
  initialiseSD(SDpin);
  
}
void loop() {
  //get current distance from frontal US sensor
  int d1 = US1.getDistance();
  int d2 = US2.getDistance();
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
      char m = Serial.read(); //add clause for using both serials for debugging

      rData += String(m);
    }
    /*
          A switch case that controls the car movement
    */
    char y = rData.charAt(0);
    switch (y) {
      case 'w' :                                                 //Move forward
        lSpeed = extract();
        rSpeed = extract();
        dir = 'w';
        writeSD("s", extract());
        break;
      case 'a' :                                                //Turn in-place to the left (more of a drift in place)
        lSpeed = -extract();
        rSpeed = extract();
        turn(lSpeed, rSpeed);
        writeSD("d", extract());
        break;
      case 's' :                                               //Move backward
        lSpeed = -extract();
        rSpeed = -extract();
        dir = 's';
        writeSD("w", extract());
        break;
      case 'd' :                                              //Turn in-place to the right (more of a drift in place)
        lSpeed = extract();
        rSpeed = -extract();
        turn(lSpeed, rSpeed);
        writeSD("a", extract());
        break;
      case 'q' :                                             //Diagonal forward left turn
        lSpeed = extract() / 2;
        rSpeed = extract();
        moveFor(lSpeed, rSpeed);
        writeSD("c", extract());
        break;
      case 'e' :                                            //Diagonal forward right turn
        lSpeed = extract();
        rSpeed = extract() / 2;
        moveFor(lSpeed, rSpeed);
        writeSD("z", extract());
        break;
      case 'z' :                                           //Diagonal backward left turn
        lSpeed = -(extract() / 2);
        rSpeed = -extract();
        moveBack(lSpeed, rSpeed);
        writeSD("e", extract());
        break;
      case 'c' :                                          //Diagonal backward right turn
        lSpeed = -extract();
        rSpeed = -(extract() / 2);
        moveBack(lSpeed, rSpeed);
        writeSD("q", extract());
        break;
      case 'x' :                                        //Stop the car
        eStop(extract(), extract());
        dir = 'x';
        writeSD("x", extract());
        break;
      case 'h' :
        printTXT();
        break;
      case 'j' :
        break;

      default :
        Serial.print("The smartCar doesn't move!");
        break;
    }
    rData = "";
  }
}
