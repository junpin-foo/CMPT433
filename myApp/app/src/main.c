// Main program to build the application
// Has main(); does initialization and cleanup and perhaps some basic logic.

#include <stdio.h>
#include <stdbool.h>
#include "hal/led.h"
#include "hal/joystick.h"

int main() {
    // Example usage
    // set_trigger(&leds[0], "heartbeat"); // Set ACT LED trigger to "heartbeat"
    // set_brightness(&leds[0], 1);        // Set ACT LED brightness to 1
    // set_brightness(&leds[0], 0);        // Set ACT LED brightness to 1
    // set_brightness(&leds[1], 1);        // Set ACT LED brightness to 1

    printf("Initializing Joystick...\n");
    Joystick_initialize();
    printf("Joystick initialized successfully.\n");

    while (1) {
        // Get the joystick reading
        struct JoystickData data = Joystick_getReading();

        // Print the joystick data
        printf("Joystick Data - X: %.2f, Y: %.2f, IsPressed: %s\n",
               data.x, data.y, data.isPressed ? "true" : "false");
    }

}