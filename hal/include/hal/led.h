/* led.h
 * 
 * This file declares a structure for defining LEDs and functions for initializing, 
 * cleaning up, and controlling LED attributes like trigger, brightness, 
 * and delays using sysfs files. The `LED_FILE_NAME` macro defines the 
 * path to the sysfs LED directory.
 */

#ifndef _LED_H_
#define _LED_H_

#include <stdio.h>
#include <stdlib.h>

#define LED_FILE_NAME "/sys/class/leds"

typedef struct {
    const char *name; //(e.g., "ACT", "PWR")
} LED;

extern LED leds[];

void Led_initialize(void);
void Led_cleanUp(void);
void Led_setTrigger(LED *led, const char *trigger);
void Led_setBrightness(LED *led, int brightness);
void Led_setDelayOn(LED *led, int on);
void Led_setDelayOff(LED *led, int off);

#endif