#include <stdio.h>
#include "nu32dip.h"            // Include standard UART library
#include "ws2812b.h"            // Include LED library
#include <math.h>           // Import math library
#include <stdlib.h>

int main(void) {
  float heartbeat = 1000; // ms
  float color_cycle = 1500; // ms
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  ws2812b_setup();
  
  int numLEDs = 8;
  float hue = 0;
    
  wsColor colors[numLEDs];
  for (int i = 0; i < numLEDs; i++) {
      colors[i] = HSBtoRGB(hue + 359/8*i, 1, 1);
  }

  ws2812b_setColor(colors, numLEDs);
 
  float t = _CP0_GET_COUNT();
  float t_color = _CP0_GET_COUNT();
  
  while (1) {
      
    ws2812b_setColor(colors, numLEDs);
    
    if  (_CP0_GET_COUNT() > t_color + 24000*(color_cycle/360)) {
        t_color = _CP0_GET_COUNT();
        hue = hue+1;
        if (hue >= 359) {
            hue = 0;
        }
        for (int i = 0; i < numLEDs; i++) {
          float temp_hue = hue + 359/8*i;
          if (temp_hue >= 360) {
              temp_hue = temp_hue - 360;
          }
          colors[i] = HSBtoRGB(temp_hue, 1, 1);
        }
    }
    
    if (_CP0_GET_COUNT() > t + 24000*heartbeat) {
        NU32DIP_GREEN = !NU32DIP_GREEN; // switches heartbeat lamp
        NU32DIP_YELLOW = !NU32DIP_GREEN;
        t = _CP0_GET_COUNT();
    }  
      
  }
}
