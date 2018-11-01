#include <avr/io.h>
#include <util/twi.h>
#include <math.h>

#include <i2c_master.h>

#ifndef SCL_CLK
#define SCL_CLK 100000UL
#endif

static uint8_t __bitrate_tbl[] = {2, 8, 32, 128};
#define __TWPS  (TWSR & (_BV(TWPS0)|_BV(TWPS1)))
#define BITRATE (((F_CPU/SCL_CLK)-16)/(__bitrate_tbl[__TWPS]))

void i2c_init(void)                                             
{
    TWSR = 0x00;
    TWBR = BITRATE;
}   

uint8_t i2c_start(uint8_t address)
{
retry:
    /* Send START condition */
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

    /* Wait until TWINT flag is set */
    while (!(TWCR & _BV(TWINT)));

    /* Check status register */
    if ((TW_STATUS != TW_START) && (TW_STATUS != TW_REP_START))
        goto retry;

    /* Send SLA+R/W */
    TWDR = address;

    /* Enable TWI and clear interrupt flag */
    TWCR = _BV(TWINT) | _BV(TWEN);

    /* Wait until TWINT flag is set */
    while (!(TWCR & _BV(TWINT)));

    /* Check status register */
    if (TW_STATUS != TW_MT_SLA_ACK)
        goto retry;

    return 0;
}

uint8_t i2c_write(uint8_t data)
{
    /* Copy data in TWI data register */
    TWDR = data;

    /* Enable TWI and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (Write operation) */
    while (!(TWCR & (1 << TWINT)));

    if (TW_STATUS == TW_MT_DATA_ACK)
        return 0;
    else if (TW_STATUS == TW_MT_DATA_NACK)
        return 1;
    else
        return 2;
}

uint8_t i2c_read_ack()
{
    /* Enable TWI, generation of ack and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);

    /* Wait until TWI finish its current job (read operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Return received data */
    return TWDR;
}   

uint8_t i2c_read_nack()
{
    /* Enable TWI and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (read operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Return received data */
    return TWDR;
}

void i2c_stop()
{
    /* Enable TWI, generate stop condition and clear interrupt flag */
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);

    /* Wait until stop condition execution */ 
    while (!(TWCR & (1 << TWSTO)));
}

// vim: ts=4 sw=4 et:
