#include <util/delay.h>

#include <mpu6050.h>
#include <i2c_master.h>
#include <context.h>

#define I2C_WRITE(addr, reg, value) \
    do { \
        i2c_start_wait((addr)); \
        i2c_write((reg)); \
        i2c_write((value)); \
        i2c_stop(); \
    } while (0)


/* Function: mpu6050_init
 * ----------------------
 *
 * Gyro initialization function
 *
 * returns: void
 */
void mpu6050_init()
{
    _delay_ms(150);         /* Power up time >100ms */

    /* Use 1kHz sample rate */
    I2C_WRITE(0xD0, MPU6050_SMPLRT_DIV, 0x07);

    /* X axis gyroscope reference frequency */
    I2C_WRITE(0xD0, MPU6050_PWR_MGMT_1, 0x01);

    /* Fs = 8kHz */
    I2C_WRITE(0xD0, MPU6050_CONFIG, 0x00);

    /* Full scale range +/- 2000 degree/C */
    I2C_WRITE(0xD0, MPU6050_GYRO_CONFIG, 0x18);

    /* Enable interrupts */
    I2C_WRITE(0xD0, MPU6050_INT_ENABLE, 0x01);
}

void mpu6050_start_loc()
{
    i2c_start_wait(0xD0);     /* I2C start with device write address */
    i2c_write(MPU6050_ACCEL_XOUT_H);  /* Write start location address from where to read */ 
    i2c_repeated_start(0xD1); /* I2C start with device read address */
}

// vim: ts=4 sw=4 et:
