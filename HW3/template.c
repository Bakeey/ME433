#include "nu32dip.h" // constants, functions for startup and UART

void wave(float);

int main(void) {
  float delay = 10; // ms
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  while (1) {
	if (!NU32DIP_USER){
        wave(delay);
	}
    
    while(!NU32DIP_USER){} // wait until user button is released
  }
}

// respond with sine wave
void wave(float delay){
    char signal[10];
    unsigned int t;
    
    for (int ii = 0; ii < 100; ++ii) {
        t = _CP0_GET_COUNT();
        sprintf(signal,"%f\r\n",sin(2*PI*ii/100));
        NU32DIP_WriteUART1(signal);

        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        while(_CP0_GET_COUNT() < t + 24000*delay){}
    }
}