#include <AccelStepper.h>

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);

void setup() {
  // put your setup code here, to run once:
    // Serial.begin(9600);
    // stepperL.setMaxSpeed(3000.0);
    // stepperL.setSpeed(2500.0);
    // // stepperL.setAcceleration(10.0);
    // stepperL.moveTo(5000);
    pinMode(2,OUTPUT); // ustaw Pin9 jako PUL
}

void loop() {
  // put your main code here, to run repeatedly:
  // stepperL.runSpeed();
  // Serial.println(stepperL.currentPosition());
  digitalWrite(2,HIGH);

  delayMicroseconds(500);

  digitalWrite(2,LOW);

  delayMicroseconds(500);
}
