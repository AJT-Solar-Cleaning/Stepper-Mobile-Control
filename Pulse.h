#ifndef PULSE_H
#define PULSE_H
#include <Arduino.h>

void buttonPinInterrupt(int BUTTON_PIN);
void buttonPinInterrupt2(int BUTTON_PIN);
int ReadPulse(int Max);
int ReadPulse2(int Max);

#endif