#include "TM4C123GH6PM.h"

void delay(int t){
    for(int i=0; i < t; i++);
}

int main(){
    // Clock gating register
    SYSCTL_RCGCGPIO_R = 0x04;
    // Port C direction register
    GPIO_PORTC_DIR_R = 0xF0;
    // Port C digital enable
    GPIO_PORTC_DEN_R = 0xF0;
    int i = 0;
    while (1){
        // Getting hexadecimal from integer
        // Ex: 3 in binary is 0011, 3 << 4 in binary is 0011 0000 => 0x30
        GPIO_PORTC_DATA_R = i << 4;
        // Simulate a 1 second delay
        delay(1000000);
        // Increment the value
        i = (i + 1) % 16;
    }
}