#include <stdio.h>
#include "nu32dip.h"            // Include standard UART library
#include <math.h>           // Import math library
#include <stdlib.h>
#include "servo.h"

int main(void) {
  float heartbeat = 1000; // ms
  float t = _CP0_GET_COUNT();
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  servoInit(); //initialize the servo PWM  
  while (1) {    

    servoSet(45);
    
    while (_CP0_GET_COUNT() < t + 24000*4000) {
        if (_CP0_GET_COUNT() > t + 24000*heartbeat) {
            NU32DIP_GREEN = !NU32DIP_GREEN; // switches heartbeat lamp
            NU32DIP_YELLOW = !NU32DIP_GREEN;
        }  
    }
    t = _CP0_GET_COUNT();
    
    servoSet(135);      

    while (_CP0_GET_COUNT() < t + 24000*4000) {
        if (_CP0_GET_COUNT() > t + 24000*heartbeat) {
            NU32DIP_GREEN = !NU32DIP_GREEN; // switches heartbeat lamp
            NU32DIP_YELLOW = !NU32DIP_GREEN;
    }  
    }
    t = _CP0_GET_COUNT();
      
  }
}
