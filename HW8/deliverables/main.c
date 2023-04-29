#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "font.h"
#include <stdio.h>

void blink(int, int); // blink the LEDs function
void drawChar(char letter, char x, char y);
void drawString(char* message, char x, char y);

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    init_mpu6050();
    ssd1306_setup();
	
	// char array for the raw data
    unsigned char d[14];
	// floats to store the data
    float ax, ay, az, gx, gy, gz, t;
	
	// read whoami
    unsigned char who;
    who = whoami();
	// print whoami
    char m[100];
    float time = (float)_CP0_GET_COUNT();
    float time_ms;
    float frame = 1;
    char fps[100];
    sprintf(m,"0x%X\r\n", who);
    NU32DIP_WriteUART1(m);
	// if whoami is not 0x68, stuck in loop with LEDs on
    if (who != 0x68) {
        while(1){
            blink(1,5);
        }
    }
    
    
    // unsigned char m_in[100];
	// wait to print until you get a newline
    // NU32DIP_ReadUART1(m_in,100);

    while (1) {
		// use core timer for exactly 100Hz loop
        _CP0_SET_COUNT(0);
        blink(1, 5);
        
        // read IMU
        burst_read_mpu6050(d);
		// convert data
        az = conv_zXL(d);

        // print out the data
        sprintf(m,"%f\r\n",az);
        sprintf(fps,"FPS: %f\r\n",frame);
        NU32DIP_WriteUART1(m);
        
        ssd1306_clear();
        drawString(m, 10, 10);
        drawString(fps, 10, 20);
        ssd1306_update();
        time = (float)_CP0_GET_COUNT(); // FIX THIS
        time_ms = 48000000.0 / 2.0 / time;
        frame = 1000.0/time_ms;
           

        while (_CP0_GET_COUNT() < 48000000 / 2 / 100) {
        }
    }
}

// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 1; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = 1; // off
        NU32DIP_YELLOW = 0; // on
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
    }
}

void drawChar(char letter, char x, char y) {
    for (int j = 0; j<5; j++) {
        char col = ASCII[letter- 0x20][j];
        for (int i=0; i<8; i++) {
            ssd1306_drawPixel(x+j,y+i, (col>>i)&0b1);
        }
    }
}

void drawString(char*m, char x, char y) {
    int k=0;
    while (m[k] !='\r') {
        drawChar(m[k],x+5*k,y);
        k++;
    }
}