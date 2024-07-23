#include <AccelStepper.h>

// Define Input Connections
#define BUTTON_PIN_MTR 10
#define BUTTON_PIN_SPD 11
#define BUTTON_PIN_LIN 12
#define BUTTON_PIN_TUR 13

#define BUTTON_PIN_RLY 22
#define DIR1 6
#define PWM1 7
#define DIR2 8
#define PWM2 9

volatile unsigned long pulseInTimeBegin_MTR = micros();
volatile unsigned long pulseInTimeEnd_MTR = micros();
volatile bool newPulseDurationAvailable_MTR = false;

volatile unsigned long pulseInTimeBegin_SPD = micros();
volatile unsigned long pulseInTimeEnd_SPD = micros();
volatile bool newPulseDurationAvailable_SPD = false;

volatile unsigned long pulseInTimeBegin_LIN = micros();
volatile unsigned long pulseInTimeEnd_LIN = micros();
volatile bool newPulseDurationAvailable_LIN = false;

volatile unsigned long pulseInTimeBegin_TUR = micros();
volatile unsigned long pulseInTimeEnd_TUR = micros();
volatile bool newPulseDurationAvailable_TUR = false;

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);
AccelStepper stepperR(AccelStepper::DRIVER, 4, 5);

// Integers to represent values from sticks and pots
int MTR;
int SPD = 0;
int LIN;
int TUR;

int LIN_L = 0;
int LIN_R = 0;
int PosLIN_L = 0;
int PosLIN_R = 0;

int MaxLIN = 255;
int MaxSPD = 2000;

int posL = 0;
int posR = 0;

int MinPulse = 100;

float accel = 100.0;

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

void buttonPinInterrupt_LIN()
{
  if (digitalRead(BUTTON_PIN_LIN) == HIGH) {
    // start measuring
    pulseInTimeBegin_LIN = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd_LIN = micros();
    newPulseDurationAvailable_LIN = true;
  }
}

void buttonPinInterrupt_TUR()
{
  if (digitalRead(BUTTON_PIN_TUR) == HIGH) {
    // start measuring
    pulseInTimeBegin_TUR = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd_TUR = micros();
    newPulseDurationAvailable_TUR = true;
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
    int oSPD = SPD;
    SPD = map(pulseDuration_SPD,1000,2000,-MaxSPD,MaxSPD);
    int Diff = SPD - oSPD;
    // if (abs(Diff) > 500)
    // {
    //   Serial.print("Diff: ");
    //   Serial.print(Diff);
    //   SPD = 0;
    // }
    if (abs(SPD) < 200)
    {
      SPD = 0;
    }
    Serial.print("         SPD: ");
    Serial.println(SPD);
  }

  if (newPulseDurationAvailable_LIN) 
  {
    newPulseDurationAvailable_LIN = false;
    unsigned long pulseDuration_LIN = pulseInTimeEnd_LIN - pulseInTimeBegin_LIN;
    LIN = map(pulseDuration_LIN,990,1800,0,200);
  }

  if (newPulseDurationAvailable_TUR) 
  {
    newPulseDurationAvailable_TUR = false;
    unsigned long pulseDuration_TUR = pulseInTimeEnd_TUR - pulseInTimeBegin_TUR;
    int oTUR = TUR;
    TUR = map(pulseDuration_TUR,1000,2000,-MaxSPD,MaxSPD);
    int Diff = TUR - oTUR;
    if (abs(Diff) > 9000)
    {
      Serial.print("Diff: ");
      Serial.print(Diff);
      TUR = 0;
    }
    if (abs(TUR) < 140)
    {
      TUR = 0;
    }
    // Serial.print("         TUR: ");
    // Serial.println(TUR);
  }
}

void setup(){
  // Set up serial monitor
  Serial.begin(115200);
  
  // Set all pins as inputs
  pinMode(BUTTON_PIN_MTR, INPUT);
  pinMode(BUTTON_PIN_SPD, INPUT);
  pinMode(BUTTON_PIN_LIN, INPUT);
  pinMode(BUTTON_PIN_TUR, INPUT);

  pinMode(BUTTON_PIN_RLY, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR1, OUTPUT);  
  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_MTR), buttonPinInterrupt_MTR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_SPD), buttonPinInterrupt_SPD, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_LIN), buttonPinInterrupt_LIN, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_TUR), buttonPinInterrupt_TUR, CHANGE);

  stepperL.setMaxSpeed(MaxSPD);
  stepperL.setAcceleration(accel);
  stepperL.setMinPulseWidth(MinPulse);

  stepperR.setMaxSpeed(MaxSPD);
  stepperR.setAcceleration(accel);
  stepperR.setMinPulseWidth(MinPulse);
  stepperL.setPinsInverted(1);
  
  // steppers.addStepper(stepperL);
  // steppers.addStepper(stepperR);
}


void loop() {
  // Get values for each channel
  Pulse();
  
  LIN_L = analogRead(A6);
  LIN_R = analogRead(A7);

  PosLIN_L = map(LIN_L, 795, 1005, 200, 0);
  PosLIN_R = map(LIN_R, 795, 1000, 0, 150);

  if (PosLIN_L < 0) PosLIN_L = 0;
  if (PosLIN_R < 0) PosLIN_R = 0;

  // Serial.print("Pos: ");
  // Serial.println(posL);

  // Print to Serial Monitor
  // Serial.print("LIN_L: ");
  // Serial.println(PosLIN_L);
  // Serial.print("LIN_R: ");
  // Serial.println(PosLIN_R);

  // delay(50);
  // Serial.print("MTR: ");
  // Serial.println(MTR);

  // delay(10);


  if (SPD == 0 || abs(SPD) > MaxSPD)
  {
    stepperL.setSpeed(0);
    stepperR.setSpeed(0);
    stepperL.stop();
    stepperR.stop();
    // if (TUR == 0 || abs(TUR) > MaxSPD)
    // {
    //   stepperL.setSpeed(0);
    //   stepperR.setSpeed(0);
    //   stepperL.stop();
    //   stepperR.stop();
    // }
    // else
    // {
    //   int newTUR = abs(TUR);
    //   if (TUR < 0)
    //   {
    //     stepperL.setSpeed(newTUR); //-500
    //     stepperR.setSpeed(-newTUR); //500
    //   }
    //   else if (TUR > 0)
    //   {
    //     stepperL.setSpeed(-newTUR); //500
    //     stepperR.setSpeed(newTUR); //-500
    //   }
    // }
  }
  else
  {
    stepperL.setSpeed(SPD);
    stepperR.setSpeed(SPD);
  }

  if (MTR > 40)
  {
    digitalWrite(BUTTON_PIN_RLY, LOW);
  }
  else if (MTR < -40)
  {
    digitalWrite(BUTTON_PIN_RLY, HIGH);
  }

  // if ((LIN <= 30 && LIN >= -30) || abs(LIN) > MaxLIN)
  // {
  //   analogWrite(PWM1, 0);
  // }
  // else 
  // {
  //   if (LIN > 30)
  //   {
  //     digitalWrite(DIR1, LOW);
  //   }
  //   else if (LIN < -30)
  //   {
  //     digitalWrite(DIR1, HIGH);
  //   }

  //   analogWrite(PWM1, abs(LIN));
  // }

  if (abs(PosLIN_L - LIN) < 10)
  {
    analogWrite(PWM1, 0);
  }
  else
  {
    if (LIN > PosLIN_L)
    {
      digitalWrite(DIR1, LOW);
    }
    else if (LIN < PosLIN_L)
    {
      digitalWrite(DIR1, HIGH);
    }
    analogWrite(PWM1, 110);
  }

  stepperL.runSpeed();
  stepperR.runSpeed();   
}