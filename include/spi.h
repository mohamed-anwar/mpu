#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include <context.h>

void    spi_init_master(void); 
void    spi_init_slave(void);
void    spi_send_byte(const uint8_t);
uint8_t spi_recieve_byte(void);
void    spi_send_string(const uint8_t *);
void    spi_receive_string(char *);
void    spi_send_context(struct ctx *);

#endif /* ! _SPI_H */
