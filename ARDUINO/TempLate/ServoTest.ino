#include <Servo.h>

/*
   the orange kable to pin
   the red kable to power
   the brown kable to gnd
*/

Servo m1;
Servo m2;

int data = 0;

void setup() {

  Serial.begin(9600);
  m1.attach(9);
  m2.attach(10);

}

void loop() {
  while (data < 180) {
    m1.write(data);
    m2.write(data);
    data++;
  }
}
