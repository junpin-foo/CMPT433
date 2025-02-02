#include "hal/led.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

LED leds[] = {
    {"ACT"}, // Green LED
    {"PWR"}  // Red LED
};

void set_trigger(LED *led, const char *trigger) {
    char trigger_file[256];
    snprintf(trigger_file, sizeof(trigger_file), "%s/%s/trigger", LED_FILE_NAME, led->name);

    FILE *pLedTriggerFile = fopen(trigger_file, "w");
    if (pLedTriggerFile == NULL) {
        perror("Error opening LED trigger file");
        exit(EXIT_FAILURE);
    }

    int charWritten = fprintf(pLedTriggerFile, trigger);
    if (charWritten <= 0) {
        perror("Error writing data to LED file");
        fclose(pLedTriggerFile);
        exit(EXIT_FAILURE);
    }

    fclose(pLedTriggerFile);
    long seconds = 1;
    long nanoseconds = 500000000;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void set_brightness(LED *led, int brightness) {
    char brightness_file[256];
    snprintf(brightness_file, sizeof(brightness_file), "%s/%s/brightness", LED_FILE_NAME, led->name);

    FILE *pLedBrightnessFile = fopen(brightness_file, "w");
    if (pLedBrightnessFile == NULL) {
        perror("Error opening LED brightness file");
        exit(EXIT_FAILURE);
    }

    int charWritten = fprintf(pLedBrightnessFile, "%d", brightness);
    if (charWritten <= 0) {
        perror("Error writing data to LED file");
        fclose(pLedBrightnessFile);
        exit(EXIT_FAILURE);
    }

    fclose(pLedBrightnessFile);
    long seconds = 1;
    long nanoseconds = 500000000;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}
