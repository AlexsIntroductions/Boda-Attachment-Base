#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "servo.h"
#ifndef FINGER_H
#define FINGER_H

typedef struct
{
    Servo *top;
    Servo *bot;
} Finger;

void initFinger(Finger *f, Servo *s1, Servo *s2);

void extendWhole(Finger *f);
void extendTop(Finger *f);
void extendBottom(Finger *f);

void retractWhole(Finger *f);
void retractTop(Finger *f);
void retractBottom(Finger *f);

#endif