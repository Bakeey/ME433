#ifndef SPI__H__
#define SPI__H__

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#define PI 3.14159265

void initSPI();
unsigned char spi_io(unsigned char o);
void send_to_dac(int message, char channel);

#endif // SPI__H__