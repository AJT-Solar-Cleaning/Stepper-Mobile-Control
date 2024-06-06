#include <AccelStepper.h>

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);
AccelStepper stepperR(AccelStepper::DRIVER, 4, 5);

int start = 0;
int kept = 1;
int posL = 0;
int posR = 0;

int MinPulse = 200;
int ToPos = 1000;
int RePos = 200;
float speed = 500.0;
float accel = 250.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  stepperL.setMaxSpeed(speed);
  stepperL.setAcceleration(accel);
  stepperL.setMinPulseWidth(MinPulse);
  stepperR.setPinsInverted(1);
  stepperL.moveTo(ToPos);

  stepperR.setMaxSpeed(speed);
  stepperR.setAcceleration(accel);
  stepperR.setMinPulseWidth(MinPulse);
  stepperR.moveTo(ToPos);
}

void loop() {
  if (kept == 1 && start != 2)
  {
      if (stepperL.distanceToGo() == 0) 
      {
        stepperL.moveTo(-stepperL.currentPosition());
        start += 1;
      }

      if (stepperR.distanceToGo() == 0) 
      {
        stepperR.moveTo(-stepperR.currentPosition());
        start += 1;
      }

      stepperL.run();
      stepperR.run();

      posL = stepperL.currentPosition();
      posR = stepperR.currentPosition();

      Serial.print("StepperL Position: ");
      Serial.println(posL);
      Serial.print("StepperR Position: ");
      Serial.println(posR);
      Serial.println("----------------------------");
  }
  else 
  {
      stepperL.moveTo(RePos);
      stepperR.moveTo(RePos);

      stepperL.run();
      stepperR.run();

      posL = stepperL.currentPosition();
      posR = stepperR.currentPosition();

      Serial.print("StepperL Position: ");
      Serial.println(posL);
      Serial.print("StepperR Position: ");
      Serial.println(posR);
      Serial.println("----------------------------");

  }
}
