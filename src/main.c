#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include <i2c_master.h>
#include <spi.h>
#include <mpu6050.h>

int main(void)
{
    i2c_init();
    mpu6050_init();
    spi_init_master();

    for (;;) {
        struct mpu6050_raw_data data;
        mpu6050_read_raw_data(&data);
        spi_send_data(&data, sizeof(data));
    }

    return 0;
}

// vim: ts=4 sw=4 et:
