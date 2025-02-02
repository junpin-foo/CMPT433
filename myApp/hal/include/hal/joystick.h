#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LED_FILE_NAME "/sys/class/leds"

struct JoystickData {
    double x;
    double y; //Up - smaller number (19), NotMoved - (874), Down - bigger number (1644)
    bool isPressed;
};

void Joystick_initialize();
struct JoystickData Joystick_getReading();

#endif