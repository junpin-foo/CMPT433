#ifndef _LED_H_
#define _LED_H_

#include <stdio.h>
#include <stdlib.h>

#define LED_FILE_NAME "/sys/class/leds"

typedef struct {
    const char *name; //(e.g., "ACT", "PWR")
} LED;

extern LED leds[];

void set_trigger(LED *led, const char *trigger);
void set_brightness(LED *led, int brightness);

#endif