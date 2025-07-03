#include <REG51.H>  // Standard 8051 register definitions

void delay(void) {
    int i, j;
    for(i = 0; i < 100; i++)
        for(j = 0; j < 1000; j++);  // Simple delay loop
}

void main(void) {
    while(1) {
        P1 = 0x01;  // Turn ON P1.0 (LED)
        delay();
        P1 = 0x00;  // Turn OFF P1.0
        delay();
    }
}