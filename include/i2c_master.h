#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <stdint.h>

void    i2c_init();                     /* I2C initialize function */
uint8_t i2c_start(uint8_t);             /* I2C start function */
uint8_t i2c_repeated_start(uint8_t);    /* I2C repeated start function */
void    i2c_stop();                     /* I2C stop function */
void    i2c_start_wait(uint8_t);        /* I2C start wait function */
uint8_t i2c_write(uint8_t);             /* I2C write function */
uint8_t i2c_read_ack();                 /* I2C read ack function */
uint8_t i2c_read_nack();                /* I2C read nack function */

#endif /* ! _I2C_MASTER_H */
