/* reaction_timer.c
    This program implements a reaction timer game using a joystick and LEDs.
    The player must quickly press the joystick in the correct direction when prompted.
    The program records the player's best reaction time and provides feedback via LEDs.
 */

#include "reaction_timer.h"

static long long bestRecordTime = 0;

static void exitGameCleanUp(void);

static JoystickDirection getJoystickDirection(void) {
    struct JoystickData data = Joystick_getReading();
    if (data.x > 0.5) return JOYSTICK_RIGHT;
    if (data.x < -0.5) return JOYSTICK_LEFT;
    if (data.y > 0.5) return JOYSTICK_UP;
    if (data.y < -0.5) return JOYSTICK_DOWN;
    return JOYSTICK_CENTER;
}

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

static void waitForJoystickRelease(void) {
    bool messagePrinted = false;
    while (1) {
        if (getJoystickDirection() != JOYSTICK_CENTER) {
            if (!messagePrinted) {
                printf("Please release the joystick...\n");
                messagePrinted = true;
            }
        } else {
            sleepForMs(100);
            return;
        }
        sleepForMs(300);
    }
}

static void incorrectResponse(void) {
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

static void startGameWithDirection(bool topSide) {
    if(topSide) {
        printf("Press UP now!\n");
        long long start = getTimeInMs();
        Led_setBrightness(GREEN_LED, 1);
        while(1){
            JoystickDirection data = getJoystickDirection();
            if(data == JOYSTICK_UP){ //CORRECT
                long long end = getTimeInMs();
                correctResponse(end - start);
                break;
            } 

            if((getTimeInMs() - start) >= MAX_RESPONSE_TIME_MS){ //NO RESPONSE
                printf("No input within 5000ms; quitting!\n");
                exitGameCleanUp();
            }

            if(data == JOYSTICK_DOWN) {//WRONG
                incorrectResponse();
                break;
            }

            if(data == JOYSTICK_LEFT || data == JOYSTICK_RIGHT){ //Quit
                printf("User selected to quit.\n");
                exitGameCleanUp();
            }            
        }
    }
    else {
        // int start = getTimeInMs();
        printf("Press DOWN now!\n");
        long long start = getTimeInMs();
        Led_setBrightness(RED_LED, 1);
        while(1){
            JoystickDirection data = getJoystickDirection();
            if(data == JOYSTICK_DOWN){ //CORRECT
                long long end = getTimeInMs();
                correctResponse(end - start);
                break;
            } 

            if((getTimeInMs() - start) >= MAX_RESPONSE_TIME_MS){ //NO RESPONSE
                printf("No input within 5000ms; quitting!\n");
                exitGameCleanUp();
            }

            if(data == JOYSTICK_UP) {//WRONG
                incorrectResponse();
                break;
            }

            if(data == JOYSTICK_LEFT || data == JOYSTICK_RIGHT){ //Quit
                printf("User selected to quit.\n");
                exitGameCleanUp();
            }            
        }

    }
}

static void exitGameCleanUp(void){
    Led_setBrightness(GREEN_LED, 0);
    Led_setBrightness(RED_LED, 0);
    Joystick_cleanUp();
    Led_cleanUp();
    exit(0);
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

        //GAME STARTED
        sleepForMs(randomNumber);
        JoystickDirection data = getJoystickDirection();
        if(data == JOYSTICK_LEFT || data == JOYSTICK_RIGHT){ //Quit
            printf("User selected to quit.\n");
            exitGameCleanUp();
        }  
        if(data != JOYSTICK_CENTER) { // moved
            printf("too soon\n");
            continue; // Restart the game loop
        }

        if(randomNumber % 2 == 0) {
            startGameWithDirection(true); //Up 
        } 
        else {
            startGameWithDirection(false); //Down
        }

    }

}