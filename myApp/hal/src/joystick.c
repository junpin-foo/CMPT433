#include "hal/joystick.h"
#include "hal/i2c.h"
#include <assert.h>

#define I2CDRV_LINUX_BUS "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x48
#define REG_CONFIGURATION 0x01
#define REG_DATA 0x00
#define TLA2024_CHANNEL_CONF_0 0x83C2

static int i2c_file_desc = -1;
static bool isInitialized = false;

void Joystick_initialize() {
    i2c_file_desc = init_i2c_bus(I2CDRV_LINUX_BUS, I2C_DEVICE_ADDRESS);
    write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_0);
    isInitialized = true;
}

struct JoystickData Joystick_getReading() {
    assert(isInitialized);

    uint16_t raw_read = read_i2c_reg16(i2c_file_desc, REG_DATA);

    // Swap and shift bytes to interpret the raw reading
    // printf("Joystick Data Y: %x\n", raw_read);
    double y_position = ((raw_read & 0xFF00) >> 8 | (raw_read & 0x00FF) << 8) >> 4;

    struct JoystickData data = {
        .x = 0.0, // Placeholder, only Y-axis is sampled here
        .y = y_position,
        .isPressed = false // Placeholder, update based on additional inputs if required
    };

    return data;
}