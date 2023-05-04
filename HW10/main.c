#include <stdio.h>
#include "nu32dip.h"            // Include standard UART library
#include "ws2812b.h"            // Include LED library
#include <math.h>           // Import math library

int main(void) {
  float heartbeat = 1000; // ms
  float color_cycle = 2500; // ms
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  ws2812b_setup();
  
  int numLEDs = 8;
  float hue = 0;
  
  wsColor white = HSBtoRGB(hue,1,1); 

  wsColor c[numLEDs]; 
  float t = _CP0_GET_COUNT();
  float t_color = _CP0_GET_COUNT();
  
  for (int i=0; i<numLEDs; i++) {
      c[i] = white;
  }
    
  while (1) {
      
    ws2812b_setColor(&white, numLEDs);
    
    if (_CP0_GET_COUNT() > t_color + 24000*(color_cycle/360)) {
        t_color = _CP0_GET_COUNT();
        hue = hue+1;
        if (hue >= 359) {
            hue = 0;
        }
        white = HSBtoRGB(hue,1,1);
    }
    
    if (_CP0_GET_COUNT() > t + 24000*heartbeat) {
        NU32DIP_GREEN = !NU32DIP_GREEN; // switches heartbeat lamp
        NU32DIP_YELLOW = !NU32DIP_GREEN;
        t = _CP0_GET_COUNT();
    }  
      
  }
}
