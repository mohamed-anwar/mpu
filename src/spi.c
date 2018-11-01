#include <avr/io.h>
#include <stdint.h>

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

void spi_send_data(void *data, uint8_t len)
{
    uint8_t *_data = (uint8_t *) data;
    while (len--) {
        spi_send_byte(*(_data++));
    }
}

// vim: ts=4 sw=4 et:
