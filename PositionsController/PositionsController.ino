#include <AccelStepper.h>
// Define Input Connections
#define CH1 12
#define CH2 10

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);
AccelStepper stepperR(AccelStepper::DRIVER, 4, 5);

// Integers to represent values from sticks and pots
int ch1Value;
int ch2Value;

int posL = 0;
int posR = 0;

int MinPulse = 300;
int ToPos = 400;
int RePos = 200;
float speed = 2000.0;
float accel = 250.0;

// Read the number of a specified channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup(){
  // Set up serial monitor
  Serial.begin(115200);
  
  // Set all pins as inputs
  pinMode(CH1, INPUT);

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
  ch1Value = readChannel(CH1, -1000, 1000, 0);
  ch2Value = readChannel(CH2, -1, 1, 0);


  posL = stepperL.currentPosition();
  posR = stepperR.currentPosition();
  Serial.print("Pos: ");
  Serial.println(posL);

  // Print to Serial Monitor
  Serial.print("Ch1: ");
  Serial.println(ch1Value);

  stepperL.moveTo(ch1Value);
  stepperR.moveTo(ch1Value);

  if(ch2Value == 1)
  {
    stepperL.run();
    stepperR.run();
    Serial.println("Running State");
  }
  else if (ch2Value == -1)
  {
    stepperL.stop();
    stepperR.stop();
    Serial.println("Stop State");
  }
  else
  {
    Serial.println("Pos State");
  }
}