#include <AccelStepper.h>

AccelStepper stepperL(AccelStepper::DRIVER, 2, 3);
AccelStepper stepperR(AccelStepper::DRIVER, 4, 5);

int start = 0;
int kept = 1;
int posL = 0;
int posR = 0;
float speed = 500.0;
float accel = 250.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepperL.setMaxSpeed(speed);
  stepperL.setAcceleration(accel);
  stepperL.setMinPulseWidth(200);
  stepperL.setPinsInverted(1);
  stepperL.moveTo(4000);

  stepperR.setMaxSpeed(speed);
  stepperR.setAcceleration(accel);
  stepperR.setMinPulseWidth(200);
  stepperR.moveTo(4000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // start = Serial.parseInt();
  // Serial.print("start : ");
  // Serial.println(start);

  // switch (start)
  // {
  //   case 1:
  //     kept = 0;
  //     break;

  //   case 2:
  //     kept = 1;
  //     break;
  // }
  
  // Serial.print("kept : ");
  // Serial.println(kept);

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

      // Serial.print(0); // To freeze the lower limit
      // Serial.print(" ");
      // Serial.print(1000); // To freeze the upper limit
      // Serial.print(" ");
      // Serial.println("Min:0,Max:200000");
      // Serial.print("StepperL Position: ");
      Serial.println(posL);
      // Serial.print("StepperR Position: ");
      Serial.println(posR);
      // Serial.println("----------------------------");
  }
  else 
  {
      stepperL.moveTo(400);
      stepperR.moveTo(400);

      stepperL.run();
      stepperR.run();

      posL = stepperL.currentPosition();
      posR = stepperR.currentPosition();

      // Serial.print(0); // To freeze the lower limit
      // Serial.print(" ");
      // Serial.print(5000); // To freeze the upper limit
      // Serial.print(" ");
      // Serial.println("Min:0,Max:200000");
      // Serial.print("StepperL Position: ");
      Serial.println(posL);
      // Serial.print("StepperR Position: ");
      Serial.println(posR);
      // Serial.println("----------------------------");

  }
}
