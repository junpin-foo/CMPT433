#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>

int init_i2c_bus(const char* bus, int address);
void write_i2c_reg16(int i2c_file_desc, uint8_t reg_addr, uint16_t value);
uint16_t read_i2c_reg16(int i2c_file_desc, uint8_t reg_addr);

#endif