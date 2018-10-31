#include <avr/io.h>
#include <stdint.h>

#include <context.h>

void spi_init_master(void) 
{
    /******** Configure SPI Master Pins *********
     * SS(PB4)   --> Output
     * MOSI(PB5) --> Output 
     * MISO(PB6) --> Input
     * SCK(PB7)  --> Output
     ********************************************/
    DDRB = DDRB |  (1<<PB4);
    DDRB = DDRB |  (1<<PB5);
    DDRB = DDRB & ~(1<<PB6);               
    DDRB = DDRB |  (1<<PB7);
    
    /* enable SPI + enable Master + choose SPI clock = FOSC/4 */
    SPCR = (1 << SPE) | (1 << MSTR); 
}

void spi_send_byte(const uint8_t data) 
{
    SPDR = data; /* send data by SPI */

    /* wait until SPI interrupt flag=1 (data is sent correctly) */
    while (!(SPSR & (1 << SPIF))); 
}

uint8_t spi_recieve_byte(void)
{
   while (!(SPSR & (1 << SPIF))); /* wait until SPI interrupt flag=1(data is receive correctly) */
   return SPDR; /* return the received byte from SPI data register */
}

void spi_send_string(const uint8_t *str)
{
    uint8_t i = 0;
    while (str[i] != '\0') {
        spi_send_byte(str[i]);
        i++;
    }
}

void spi_receive_string(char *str)
{
    unsigned char i = 0;
    str[i] = spi_recieve_byte();

    while (str[i] != '#') {
        i++;
        str[i] = spi_recieve_byte();
    }
    str[i] = '\0';
}

void spi_send_context(struct ctx *ctx)
{
    spi_send_byte(ctx->acc_x_1);
    spi_send_byte(ctx->acc_x_2);
    spi_send_byte(ctx->acc_y_1);
    spi_send_byte(ctx->acc_y_2);
    spi_send_byte(ctx->acc_z_1);
    spi_send_byte(ctx->acc_z_2);
    spi_send_byte(ctx->temp_1);
    spi_send_byte(ctx->temp_2);
    spi_send_byte(ctx->gyro_x_1);
    spi_send_byte(ctx->gyro_x_2);
    spi_send_byte(ctx->gyro_y_1);
    spi_send_byte(ctx->gyro_y_2);
    spi_send_byte(ctx->gyro_z_1);
    spi_send_byte(ctx->gyro_z_2);
}

// vim: ts=4 sw=4 et:
