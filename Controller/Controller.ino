#include <AccelStepper.h>

// Define Input Connections
#define BUTTON_PIN_MTR 10
#define BUTTON_PIN_SPD 11
#define BUTTON_PIN_RLY 22

volatile unsigned long pulseInTimeBegin_SPD = micros();
volatile unsigned long pulseInTimeEnd_SPD = micros();
volatile bool newPulseDurationAvailable_SPD = false;

volatile unsigned long pulseInTimeBegin_MTR = micros();
volatile unsigned long pulseInTimeEnd_MTR = micros();
volatile bool newPulseDurationAvailable_MTR = false;

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);
AccelStepper stepperR(AccelStepper::DRIVER, 4, 5);

// Integers to represent values from sticks and pots
int SPD;
int MTR;

int MaxSPD = 600;

int posL = 0;
int posR = 0;

int MinPulse = 200;

float speed = 2000.0;
float accel = 300.0;

void buttonPinInterrupt_MTR()
{
  if (digitalRead(BUTTON_PIN_MTR) == HIGH) {
    // start measuring
    pulseInTimeBegin_MTR = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd_MTR = micros();
    newPulseDurationAvailable_MTR = true;
  }
}

void buttonPinInterrupt_SPD()
{
  if (digitalRead(BUTTON_PIN_SPD) == HIGH) {
    // start measuring
    pulseInTimeBegin_SPD = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd_SPD = micros();
    newPulseDurationAvailable_SPD = true;
  }
}

void Pulse()
{
  if (newPulseDurationAvailable_MTR) 
  {
    newPulseDurationAvailable_MTR = false;
    unsigned long pulseDuration_MTR = pulseInTimeEnd_MTR - pulseInTimeBegin_MTR;
    MTR = map(pulseDuration_MTR,1000,2000,-50,50);
  }

  if (newPulseDurationAvailable_SPD) 
  {
    newPulseDurationAvailable_SPD = false;
    unsigned long pulseDuration_SPD = pulseInTimeEnd_SPD - pulseInTimeBegin_SPD;
    SPD = map(pulseDuration_SPD,1000,2000,-MaxSPD,MaxSPD);
  }
}

void setup(){
  // Set up serial monitor
  Serial.begin(115200);
  
  // Set all pins as inputs
  pinMode(BUTTON_PIN_MTR, INPUT);
  pinMode(BUTTON_PIN_SPD, INPUT);

  pinMode(BUTTON_PIN_RLY, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_MTR), buttonPinInterrupt_MTR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_SPD), buttonPinInterrupt_SPD, CHANGE);

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
  Pulse();
  
  posL = stepperL.currentPosition();
  posR = stepperR.currentPosition();
  Serial.print("Pos: ");
  Serial.println(posL);

  // Print to Serial Monitor
  Serial.print("SPD: ");
  Serial.println(SPD);
  Serial.print("MTR: ");
  Serial.println(MTR);

  // delay(10);
  if (SPD == 0)
  {
    stepperL.setSpeed(0);
    stepperR.setSpeed(0);
    stepperL.stop();
    stepperR.stop();

    Serial.println("Stop State");
  }
  else if (SPD < 0)
  {
    stepperL.setSpeed(SPD);
    stepperR.setSpeed(SPD);
    Serial.println("Reverse State");
  }
  else if (SPD > 0)
  {
    stepperL.setSpeed(SPD);
    stepperR.setSpeed(SPD);
    Serial.println("Forward State");
  }

  if (MTR > 40)
  {
    digitalWrite(BUTTON_PIN_RLY, LOW);
  }
  else if (MTR < -40)
  {
    digitalWrite(BUTTON_PIN_RLY, HIGH);    
  }
  stepperL.runSpeed();
  stepperR.runSpeed();

}