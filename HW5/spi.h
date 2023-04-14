#ifndef SPI__H__
#define SPI__H__

#define PI 3.14159265
#define CS LATAbits.LATA0;

void initSPI();
unsigned char spi_io(unsigned char o);

#endif // SPI__H__