#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "hal/led.h"
#include "hal/joystick.h"

#define GREEN_LED &leds[0]
#define RED_LED &leds[1]
#define FLASH_DURATION_MS 250
#define MAX_RESPONSE_TIME_MS 5000

static long long getTimeInMs(void){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
    return milliSeconds;
}

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
    printf("Get ready...\n");
    Led_setBrightness(GREEN_LED, 0);
    Led_setBrightness(RED_LED, 0);

    for (int i = 0; i < 4; i++) {
        Led_setBrightness(GREEN_LED, 1);
        sleepForMs(FLASH_DURATION_MS);
        Led_setBrightness(GREEN_LED, 0);
        
        Led_setBrightness(RED_LED, 1);
        sleepForMs(FLASH_DURATION_MS);
        Led_setBrightness(RED_LED, 0);
    }
}

static void waitForJoystickRelease() {
    struct JoystickData data;
    struct JoystickData data2;
    data = Joystick_getReading();
    if(data.y > 0.1 || data.y < -0.1) {
        printf("Please let go of joystick\n");
        while(1) {
            data = Joystick_getReading();
            sleepForMs(1000); //1s sleep
            data2 = Joystick_getReading();

            if(data.y - data2.y  == 0 && data2.y < 0.05 && data2.y > -0.05) {
                return;
            }     
        }
        
    }
}

static void incorrectResponse() {
    printf("Incorrect.\n");
    Led_setBrightness(GREEN_LED, 0);
    Led_setTrigger(RED_LED, "timer");
    sleepForMs(50);
    Led_setDelayOn(RED_LED, 50);
    Led_setDelayOff(RED_LED, 150);

    sleepForMs(1000);
    Led_setBrightness(RED_LED, 0);
}

static void correctResponse() {
    printf("Correct!\n");
    Led_setBrightness(RED_LED, 0);
    Led_setTrigger(GREEN_LED, "timer");
    sleepForMs(50);
    Led_setDelayOn(GREEN_LED, 50);
    Led_setDelayOff(GREEN_LED, 150);

    sleepForMs(1000);
    Led_setBrightness(GREEN_LED, 0);
}

int main() {
    srand(time(NULL));
    Joystick_initialize();

    printf("Hello embedded world, from JunPin Foo!\n");

    printf("When the LEDs light up, press the joystick in that direction!\n");

    printf("(Press left or right to exit)\n");

    while(1){
        restart_game:
        initialFlashLed();

        waitForJoystickRelease();

        int randonNumber = rand()%((3000+1)-500) + 500;

        // printf("Random: %d\n", randonNumber);

        //GAME STARTED
        struct JoystickData data = Joystick_getReading();
        struct JoystickData data2 = Joystick_getReading();
        long long startCountdown = getTimeInMs();
        while((getTimeInMs() - startCountdown) < randonNumber){
            data2 = Joystick_getReading();
            if(fabs(data.y - data2.y)  > 0.02) { //moved
                printf("too soon\n");
                goto restart_game;
            } 
        }
        if(randonNumber % 2 == 0) {
            printf("Press UP now!\n");
            long long start = getTimeInMs();
            Led_setBrightness(GREEN_LED, 1);
            while(1){
                data = Joystick_getReading();
                if(data.y > 0.7){ //CORRECT
                    long long end = getTimeInMs();
                    correctResponse();
                    long long responseTime = end - start;
                    printf("resp time: %llu\n", responseTime);
                    break;
                } 

                if((getTimeInMs() - start) >= MAX_RESPONSE_TIME_MS){ //NO RESPONSE
                    printf("No input within 5000ms; quitting!\n");
                    return 0;
                }

                if(data.y < -0.7) {//WRONG
                    incorrectResponse();
                    break;
                }

                if(data.x > 0.7 || data.x < -0.7){ //Quit
                    printf("User selected to quit.\n");
                    return 0;
                }            
            }
        }
        else {
            // int start = getTimeInMs();
            printf("Press DOWN now!\n");
            long long start = getTimeInMs();
            Led_setBrightness(RED_LED, 1);
            while(1){
                data = Joystick_getReading();
                if(data.y < -0.7){ //CORRECT
                    long long end = getTimeInMs();
                    correctResponse();
                    long long responseTime = end - start;
                    printf("resp time: %llu\n", responseTime);
                    break;
                } 

                if((getTimeInMs() - start) >= MAX_RESPONSE_TIME_MS){ //NO RESPONSE
                    printf("No input within 5000ms; quitting!\n");
                    return 0;
                }

                if(data.y > 0.7) {//WRONG
                    incorrectResponse();
                    break;
                }

                if(data.x > 0.7 || data.x < -0.7){ //Quit
                    printf("User selected to quit.\n");
                    return 0;
                }            
            }

        }

    }

}