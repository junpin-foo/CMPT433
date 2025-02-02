#ifndef _LED_H_
#define _LED_H_

#include <stdio.h>
#include <stdlib.h>

#define LED_FILE_NAME "/sys/class/leds"

typedef struct {
    const char *name; //(e.g., "ACT", "PWR")
} LED;

extern LED leds[];

void Led_initialize();
void Led_setTrigger(LED *led, const char *trigger);
void Led_setBrightness(LED *led, int brightness);
void Led_setDelayOn(LED *led, int on);
void Led_setDelayOff(LED *led, int off);

#endif