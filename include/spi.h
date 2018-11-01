#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

void    spi_init_master(void); 
void    spi_send_byte(const uint8_t);
void    spi_send_data(void *, uint8_t);

#endif /* ! _SPI_H */
