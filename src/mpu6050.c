#include <util/delay.h>

#include <mpu6050.h>
#include <i2c_master.h>

#define I2C_WRITE_REG(reg, value) \
    do { \
        i2c_write((reg)); \
        i2c_write((value)); \
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

    i2c_start(0xD0);

    /* Use 1kHz sample rate */
    I2C_WRITE_REG(MPU6050_SMPLRT_DIV, 0x07);

    /* X axis gyroscope reference frequency */
    I2C_WRITE_REG(MPU6050_PWR_MGMT_1, 0x01);

    /* Fs = 8kHz */
    I2C_WRITE_REG(MPU6050_CONFIG, 0x00);

    /* Full scale range +/- 2000 degree/C */
    I2C_WRITE_REG(MPU6050_GYRO_CONFIG, 0x18);

    /* Enable interrupts */
    I2C_WRITE_REG(MPU6050_INT_ENABLE, 0x01);

    i2c_stop();
}

void mpu6050_start_loc()
{
    /* I2C start with device write address */
    i2c_start(0xD0);

    /* Write start location address from where to read */ 
    i2c_write(MPU6050_ACCEL_XOUT_H);

    /* I2C start with device read address */
    i2c_start(0xD1);
}

static inline int16_t mpu6050_read_int16(uint8_t ack)
{
    int16_t r = i2c_read_ack();

    if (ack)
        r = (r << 8) | i2c_read_ack();
    else
        r = (r << 8) | i2c_read_nack();

    return r;
}

void mpu6050_read_raw_data(struct mpu6050_raw_data *data)
{
    mpu6050_start_loc();
    data->acc_x  = mpu6050_read_int16(1);
    data->acc_y  = mpu6050_read_int16(1);
    data->acc_z  = mpu6050_read_int16(1); 
    data->temp   = mpu6050_read_int16(1);
    data->gyro_x = mpu6050_read_int16(1);
    data->gyro_y = mpu6050_read_int16(1);
    data->gyro_z = mpu6050_read_int16(0);
    i2c_stop();
}

// vim: ts=4 sw=4 et:
