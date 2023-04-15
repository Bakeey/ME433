#include <stdio.h>
#include "spi.h"            // Include SPI library
#include <math.h>           // Import math library

int main(void) {
  float delay = 2.5; // ms
  initSPI();
  
  int length = 400;         // length of signal
  int sine[length];       // preallocation
  int triangle[length];
  for(int ii = 0; ii < length; ++ii){
      sine[ii] = (int)1023/2 * sin(4*PI*ii/length) + 1023/2;
      if (ii <= length/2) {
          triangle[ii] = ii * 1023 / 200;
      }
      else {
          triangle[ii] = (400 - ii) * 1023 / 200;
      }
  }
  while (1) {
    for(int ii = 0; ii < length; ++ii){
      float t = _CP0_GET_COUNT();
      send_to_dac(sine[ii],0);
      send_to_dac(triangle[ii],1);

      while(_CP0_GET_COUNT() < t + 24000*delay){}
    }
  }
}
