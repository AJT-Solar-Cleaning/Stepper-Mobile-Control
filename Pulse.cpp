#include "Pulse.h"

volatile unsigned long pulseInTimeBegin = micros();
volatile unsigned long pulseInTimeEnd = micros();
volatile bool newPulseDurationAvailable = false;

volatile unsigned long pulseInTimeBegin2 = micros();
volatile unsigned long pulseInTimeEnd2 = micros();
volatile bool newPulseDurationAvailable2 = false;

void buttonPinInterrupt(int BUTTON_PIN)
{
  if (digitalRead(BUTTON_PIN) == HIGH) 
  {
    // start measuring
    pulseInTimeBegin = micros();
  }
  else 
  {
    // stop measuring
    pulseInTimeEnd = micros();
    newPulseDurationAvailable = true;
  }
}

void buttonPinInterrupt2(int BUTTON_PIN)
{
  if (digitalRead(BUTTON_PIN) == HIGH) 
  {
    // start measuring
    pulseInTimeBegin2 = micros();
  }
  else 
  {
    // stop measuring
    pulseInTimeEnd2 = micros();
    newPulseDurationAvailable2 = true;
  }
}

int ReadPulse(int Max)
{
  if (newPulseDurationAvailable) 
  {
    newPulseDurationAvailable = false;
    unsigned long pulseDuration = pulseInTimeEnd - pulseInTimeBegin;
    int mem = map(pulseDuration,1000,2000,-Max,Max);
    return mem;
  }
}

int ReadPulse2(int Max)
{
  if (newPulseDurationAvailable2) 
  {
    newPulseDurationAvailable2 = false;
    unsigned long pulseDuration = pulseInTimeEnd2 - pulseInTimeBegin2;
    int mem = map(pulseDuration,1000,2000,-Max,Max);
    return mem;
  }
}