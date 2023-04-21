#include <stdio.h>
#include "nu32dip.h"            // Include SPI library
#include "i2c_master_noint.h"            // Include SPI library
#include <math.h>           // Import math library

void blink(int, int); // blink the LEDs function

int main(void) {
  float heartbeat = 1000; // ms
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  i2c_init();
  
  float t = _CP0_GET_COUNT();
  
  while (1) {
    if (readPin(ADDRESS, 0x09)==0) {
        writePin(ADDRESS, 0x0A , 0b10000000); 
    }
    else {
        writePin(ADDRESS, 0x0A , 0b00000000); 
    }
     
    if (_CP0_GET_COUNT() > t + 24000*heartbeat) {
        NU32DIP_GREEN = !NU32DIP_GREEN; // switches heartbeat lamp
        NU32DIP_YELLOW = NU32DIP_GREEN;
        t = _CP0_GET_COUNT();
    }    
      
  }
}
