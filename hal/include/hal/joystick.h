/* joystick.h
    This header file defines the interface for joystick control using an ADC chip over I2C.
    It provides functions to initialize and clean up the joystick, as well as retrieve 
    the current joystick position with normalized x and y values.
*/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LED_FILE_NAME "/sys/class/leds"

struct JoystickData {
    double x; //neg - smaller number (18), NotMoved - (858), pos - bigger number (1644)
    double y; //neg - smaller number (8), NotMoved - (839), pos - bigger number (1635)
    bool isPressed;
};

void Joystick_initialize(void);
void Joystick_cleanUp(void);
struct JoystickData Joystick_getReading();

#endif