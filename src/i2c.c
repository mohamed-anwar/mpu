#include <avr/io.h>
#include <math.h>

#include <i2c_master.h>

/* Define SCL clock frequency */
#define SCL_CLK 100000L
/* Define bit rate */
#define BITRATE(TWSR)   ((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

/* function: i2c_init
 * --------------
 * I2C initialize function
 *
 * returns: void
 */
void i2c_init(void)                                             
{
    /* Set bitrate register value by formula */
    TWBR = BITRATE(TWSR = 0x00);
}   

/* Function: i2c_start
 * -------------------
 * I2C start function
 *
 * returns: TODO
 */
uint8_t i2c_start(uint8_t address)                     
{
    uint8_t status;

    /* Enable TWI, generate start condition and clear interrupt flag */
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (start condition) */
    while (!(TWCR & (1 << TWINT)));

    /* Read TWI status register with masking lower three bits */
    status = TWSR & 0xF8;

    /* Check weather start condition transmitted successfully or not?
     * If not then return 0 to indicate start condition fail
     */
    if (status != 0x08)
        return 0;

    /* If yes then write SLA+W in TWI data register */
    TWDR = address;

    /* Enable TWI and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (Write operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Read TWI status register with masking lower three bits */    
    status = TWSR & 0xF8;

    /* Check weather SLA+W transmitted & ack received or not?
     * If yes then return 1 to indicate ack received i.e. ready to accept data byte
     */
    if (status == 0x18)
        return 1;

    /* Check weather SLA+W transmitted & nack received or not? */
    if (status == 0x20) {
        /* If yes then return 2 to indicate nack received i.e. device is busy */
        return 2;
    } else {
        /* Else return 3 to indicate SLA+W failed */
        return 3;
    }
}

/* I2C repeated start function */
uint8_t i2c_repeated_start(uint8_t address)
{
    uint8_t status;

    /* Enable TWI, generate start condition and clear interrupt flag */
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (start condition) */
    while (!(TWCR & (1 << TWINT)));

    /* Read TWI status register with masking lower three bits */
    status = TWSR & 0xF8;

    /* Check weather repeated start condition transmitted successfully or not?
     * If no then return 0 to indicate repeated start condition fail
     */
    if (status != 0x10)
        return 0;

    /* If yes then write SLA+R in TWI data register */
    TWDR = address;

    /* Enable TWI and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (Write operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Read TWI status register with masking lower three bits */
    status = TWSR & 0xF8;

    /* Check weather SLA+R transmitted & ack received or not?
     * If yes then return 1 to indicate ack received
     */ 
    if (status == 0x40)
        return 1;

    /* Check weather SLA+R transmitted & nack received or not? */
    if (status == 0x20) {
        /* If yes then return 2 to indicate nack received i.e. device is busy */
        return 2;
    } else {
        /* Else return 3 to indicate SLA+W failed */
        return 3;
    }
}

/* I2C stop function */
void i2c_stop()
{
    /* Enable TWI, generate stop condition and clear interrupt flag */
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);

    /* Wait until stop condition execution */ 
    while (TWCR & (1 << TWSTO));
}

/* I2C start wait function */
void i2c_start_wait(uint8_t address)
{
    uint8_t status;
    for (;;) {
        /* Enable TWI, generate start condition and clear interrupt flag */
        TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);

        /* Wait until TWI finish its current job (start condition) */
        while (!(TWCR & (1 << TWINT)));

        /* Read TWI status register with masking lower three bits */
        status = TWSR & 0xF8;                               

        /* Check weather start condition transmitted successfully or not?
         * If no then continue with start loop again
         */
        if (status != 0x08)
            continue;

        /* If yes then write SLA+W in TWI data register */
        TWDR = address;

        /* Enable TWI and clear interrupt flag */
        TWCR = (1 << TWEN) | (1 << TWINT);

        /* Wait until TWI finish its current job (Write operation) */
        while (!(TWCR & (1 << TWINT)));

        /* Read TWI status register with masking lower three bits */
        status = TWSR & 0xF8;

        /* Check weather SLA+W transmitted & ack received or not? */
        if (status != 0x18 ) {

            /* If not then generate stop condition */
            i2c_stop();

            /* continue with start loop again */
            continue;
        }

        /* If yes then break loop */
        break;
    }
}

/* I2C write function */
uint8_t i2c_write(uint8_t data)
{
    uint8_t status;

    /* Copy data in TWI data register */
    TWDR = data;

    /* Enable TWI and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (Write operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Read TWI status register with masking lower three bits */
    status = TWSR & 0xF8;

    /* Check weather data transmitted & ack received or not?
     * If yes then return 0 to indicate ack received
     */
    if (status == 0x28)
        return 0;

    /* Check weather data transmitted & nack received or not? */
    if (status == 0x30) {
        /* If yes then return 1 to indicate nack received */
        return 1;
    } else {
        /* Else return 2 to indicate data transmission failed */
        return 2;
    }
}

/* I2C read ack function */
uint8_t i2c_read_ack()
{
    /* Enable TWI, generation of ack and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);

    /* Wait until TWI finish its current job (read operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Return received data */
    return TWDR;
}   

/* I2C read nack function */
uint8_t i2c_read_nack()
{
    /* Enable TWI and clear interrupt flag */
    TWCR = (1 << TWEN) | (1 << TWINT);

    /* Wait until TWI finish its current job (read operation) */
    while (!(TWCR & (1 << TWINT)));

    /* Return received data */
    return TWDR;
}

// vim: ts=4 sw=4 et:
