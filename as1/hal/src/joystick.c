#include "hal/joystick.h"
#include "hal/i2c.h"
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define I2CDRV_LINUX_BUS "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x48 // ADC chip
#define REG_CONFIGURATION 0x01
#define REG_DATA 0x00
#define TLA2024_CHANNEL_CONF_0 0x83C2 // Configuration for Y-axis
#define TLA2024_CHANNEL_CONF_1 0x83D2 // Configuration for X-axis

static int i2c_file_desc = -1;
static bool isInitialized = false;

// Joystick calibration values
#define X_MIN 18
#define X_CENTER 858
#define X_MAX 1644

#define Y_MIN 8
#define Y_CENTER 839
#define Y_MAX 1635

double scale_value(double raw, double min, double max) {
    return 2.0 * ((raw - min) / (max - min)) - 1.0;
}


void Joystick_initialize() {
    Ic2_initialize();
    i2c_file_desc = init_i2c_bus(I2CDRV_LINUX_BUS, I2C_DEVICE_ADDRESS);
    isInitialized = true;
}

struct JoystickData Joystick_getReading() {
    if (!isInitialized) {
        fprintf(stderr, "Error: Joystick not initialized!\n");
        exit(EXIT_FAILURE);
    }

    write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_1);
    uint16_t raw_x = read_i2c_reg16(i2c_file_desc, REG_DATA);
    double x_position = ((raw_x & 0xFF00) >> 8 | (raw_x & 0x00FF) << 8) >> 4;
    double x_scaled = scale_value(x_position, X_MIN, X_MAX);

    struct timespec reqDelay = {0, 100000};
    nanosleep(&reqDelay, (struct timespec *) NULL);

    write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_0);
    uint16_t raw_y = read_i2c_reg16(i2c_file_desc, REG_DATA);
    double y_position = ((raw_y & 0xFF00) >> 8 | (raw_y & 0x00FF) << 8) >> 4;
    double y_scaled = scale_value(y_position, Y_MIN, Y_MAX);

    struct JoystickData data = {
        .x = x_scaled,
        .y = y_scaled,
        .isPressed = false // Placeholder
    };

    return data;
}