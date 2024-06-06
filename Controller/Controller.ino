#include <AccelStepper.h>
#include <MultiStepper.h>

// Define Input Connections
#define BUTTON_PIN 11
volatile unsigned long pulseInTimeBegin = micros();
volatile unsigned long pulseInTimeEnd = micros();
volatile bool newPulseDurationAvailable = false;

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);
AccelStepper stepperR(AccelStepper::DRIVER, 4, 5);
MultiStepper steppers;

// Integers to represent values from sticks and pots
int ch1Value;

int posL = 0;
int posR = 0;

int MinPulse = 200;
int ToPos = 400;
int RePos = 200;
float speed = 2000.0;
float accel = 300.0;

void buttonPinInterrupt()
{
  if (digitalRead(BUTTON_PIN) == HIGH) {
    // start measuring
    pulseInTimeBegin = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd = micros();
    newPulseDurationAvailable = true;
  }
}

void setup(){
  // Set up serial monitor
  Serial.begin(115200);
  
  // Set all pins as inputs
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonPinInterrupt,
                  CHANGE);

  stepperL.setMaxSpeed(speed);
  stepperL.setAcceleration(accel);
  stepperL.setMinPulseWidth(MinPulse);

  stepperR.setMaxSpeed(speed);
  stepperR.setAcceleration(accel);
  stepperR.setMinPulseWidth(MinPulse);
  stepperR.setPinsInverted(1);
  
  // steppers.addStepper(stepperL);
  // steppers.addStepper(stepperR);
}


void loop() {
  // Get values for each channel
  if (newPulseDurationAvailable) {
    newPulseDurationAvailable = false;
    unsigned long pulseDuration = pulseInTimeEnd - pulseInTimeBegin;
    ch1Value = map(pulseDuration,1000,2000,-600,600);
  }
  
  posL = stepperL.currentPosition();
  posR = stepperR.currentPosition();
  Serial.print("Pos: ");
  Serial.println(posL);

  // Print to Serial Monitor
  Serial.print("Ch1: ");
  Serial.println(ch1Value);
  
  // delay(10);
  if (ch1Value == 0)
  {
    stepperL.setSpeed(0);
    stepperR.setSpeed(0);
    stepperL.stop();
    stepperR.stop();

    Serial.println("Stop State");
  }
  else if (ch1Value < 0)
  {
    stepperL.setSpeed(ch1Value);
    stepperR.setSpeed(ch1Value);
    Serial.println("Reverse State");
  }
  else if (ch1Value > 0)
  {
    stepperL.setSpeed(ch1Value);
    stepperR.setSpeed(ch1Value);
    Serial.println("Forward State");
  }


  stepperL.runSpeed();
  stepperR.runSpeed();

}