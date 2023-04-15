#include "spi.h"

// initialize SPI1
void initSPI() {
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    // Pin B14 has to be SCK1
    // Turn of analog pins
    //... yeah this is fine...
    // Make an output pin for CS
    TRISAbits.TRISA0 = 0;               // RPA0 output for CS
    SS1Rbits.SS1R = 0b0000;
    // Set SDO1
    TRISAbits.TRISA1 = 0;               // RPA1 output for SDO1
    RPA1Rbits.RPA1R = 0b0011;           
    // Set SDI1
    SDI1Rbits.SDI1R = 0b0100;           // RPB8 input for SDI1
    
    LATAbits.LATA0 = 1;

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
    
    __builtin_enable_interrupts();
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}


void send_to_dac(int message, char channel){
    unsigned short t = (channel << 15) | (0b111 << 12) | (message << 2);
   
    unsigned char first_8_bits = (t >> 8) & 0xFF;
    unsigned char last_8_bits = t & 0xFF;

    LATAbits.LATA0 = 0;
    spi_io(first_8_bits);
    spi_io(last_8_bits);
    LATAbits.LATA0 = 1;
}
