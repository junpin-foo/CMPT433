#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "hal/led.h"
#include "hal/joystick.h"

#define GREEN_LED &leds[0]
#define RED_LED &leds[1]
#define FLASH_DURATION_MS 250
#define MAX_RESPONSE_TIME_MS 5000

// static long long getTimeInMs(void){
//     struct timespec spec;
//     clock_gettime(CLOCK_REALTIME, &spec);
//     long long seconds = spec.tv_sec;
//     long long nanoSeconds = spec.tv_nsec;
//     long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
//     return milliSeconds;
// }

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}



static void initialFlashLed(void) {
    set_brightness(GREEN_LED, 0);
    set_brightness(RED_LED, 0);

    for (int i = 0; i < 4; i++) {
        set_brightness(GREEN_LED, 1);
        sleepForMs(FLASH_DURATION_MS);
        set_brightness(GREEN_LED, 0);
        
        set_brightness(RED_LED, 1);
        sleepForMs(FLASH_DURATION_MS);
        set_brightness(RED_LED, 0);
    }
}

static void wait_for_joystick_release() {
    struct JoystickData data;
    do {
        data = Joystick_getReading();
    } while (data.y > 0.2 || data.y < -0.2); 
}


int main() {
    printf("Hello embedded world, from JunPin Foo!\n");

    printf("When the LEDs light up, press the joystick in that direction!\n");

    printf("(Press left or right to exit)\n");

    initialFlashLed();

    wait_for_joystick_release();

}