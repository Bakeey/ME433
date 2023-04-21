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
    writePin(0b01000000, 0x0A , 0b10000000);  
    
    if (_CP0_GET_COUNT() > t + 24000*heartbeat) {
        NU32DIP_GREEN = !NU32DIP_GREEN; // switches heartbeat lamp
        t = _CP0_GET_COUNT();
    }    
      
  }
}
