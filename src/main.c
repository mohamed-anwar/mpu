#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include <i2c_master.h>
#include <spi.h>
#include <mpu6050.h>
#include <context.h>

void read_raw_values(struct ctx *ctx)
{
    mpu6050_start_loc();
    ctx->acc_x_1  = i2c_read_ack();
    ctx->acc_x_2  = i2c_read_ack();
    ctx->acc_y_1  = i2c_read_ack();
    ctx->acc_y_2  = i2c_read_ack();
    ctx->acc_z_1  = i2c_read_ack(); 
    ctx->acc_z_2  = i2c_read_ack();
    ctx->temp_1   = i2c_read_ack();
    ctx->temp_2   = i2c_read_ack();
    ctx->gyro_x_1 = i2c_read_ack();
    ctx->gyro_x_2 = i2c_read_ack();
    ctx->gyro_y_1 = i2c_read_ack();
    ctx->gyro_y_2 = i2c_read_ack();
    ctx->gyro_z_1 = i2c_read_ack();
    ctx->gyro_z_2 = i2c_read_nack();    
    i2c_stop();
}

int main(void)
{
    i2c_init();
    mpu6050_init();
    spi_init_master();

    for (;;) {
        struct ctx ctx;
        read_raw_values(&ctx);
        spi_send_context(&ctx);
    }

    return 0;
}

// vim: ts=4 sw=4 et:
