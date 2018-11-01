#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <stdint.h>

void    i2c_init();
uint8_t i2c_start(uint8_t);
uint8_t i2c_write(uint8_t);
uint8_t i2c_read_ack();
uint8_t i2c_read_nack();
void    i2c_stop();

#endif /* ! _I2C_MASTER_H */
