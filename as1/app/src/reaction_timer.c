#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "hal/led.h"
#include "hal/joystick.h"

//TODO: 
// 1. commment at top of file
// 2. enum for direction
// 3. clean up

#define GREEN_LED &leds[0]
#define RED_LED &leds[1]
#define FLASH_DURATION_MS 250
#define MAX_RESPONSE_TIME_MS 5000

static long long bestRecordTime = 0;

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
    while (1) {
        data = Joystick_getReading();
        // If joystick is not centered, prompt user
        if (fabs(data.y) > 0.1) {
            printf("Please release the joystick...\n");
        }
        sleepForMs(100);
        struct JoystickData newData = Joystick_getReading();

        // If joystick is stable and near center, exit
        if (fabs(newData.y) < 0.05 && fabs(data.y - newData.y) < 0.01) {
            return;
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

static void correctResponse(long long currentTime) {
    printf("Correct!\n");
    if(bestRecordTime == 0 || bestRecordTime > currentTime){
        printf("New best time!\n");
        bestRecordTime = currentTime;
    }
    printf("Your reaction time was %llums; best so far in game is %llums.!\n", currentTime, bestRecordTime);
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
    Led_initialize();

    printf("Hello embedded world, from JunPin Foo!\n\n");

    printf("When the LEDs light up, press the joystick in that direction!\n");

    printf("(Press left or right to exit)\n");

    while(1){

        initialFlashLed();

        waitForJoystickRelease();
        int randomNumber = rand()%((3000+1)-500) + 500; //between 3000 and 500 ms
        // printf("Random: %d\n", randonNumber);

        //GAME STARTED
        printf("random now\n");
        sleepForMs(randomNumber);
        struct JoystickData data = Joystick_getReading();
        if(data.x > 0.7 || data.x < -0.7){ //Quit
            printf("User selected to quit.\n");
            return 0;
        }  
        if(data.y > 0.02 || data.y < -0.02) { // moved
            printf("too soon\n");
            continue; // Restart the game loop
        }
        printf("random stop\n");

        if(randomNumber % 2 == 0) {
            printf("Press UP now!\n");
            long long start = getTimeInMs();
            Led_setBrightness(GREEN_LED, 1);
            while(1){
                data = Joystick_getReading();
                if(data.y > 0.7){ //CORRECT
                    long long end = getTimeInMs();
                    correctResponse(end - start);
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
                    correctResponse(end - start);
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