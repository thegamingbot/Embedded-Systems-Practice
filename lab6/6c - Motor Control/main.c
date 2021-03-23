#include "TM4C123GH6PM.h"

#define pressed 0x00U
#define sw1 *((unsigned int *)0x40025040)
#define sw2 *((unsigned int *)0x40025004)

// Delay function ('n' ms) 
void delayMs(int n){
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 3180; j++);
}

// Delay function ('n' us)
void delayUs(int n){
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 3; j++);
}

// Main driver function
int main(){
    // Clock gating register to enable the clock on GPIO port F and B
    SYSCTL_RCGCGPIO_R = 0x22U;

    // Set PORTF 3 pins as output (LED) pin
    GPIO_PORTF_DIR_R |= 0x08;
    // Set PORTF 3 pins as digital pins
    GPIO_PORTF_DEN_R |= 0x08;
    // Enable alternate function
    GPIO_PORTF_AFSEL_R |= 0x08;
    // Clear PORTF 3 alternate function
    GPIO_PORTF_PCTL_R &= ~0x0000F000;
    // Set PORTF 3 alternate funtion to PWM
    GPIO_PORTF_PCTL_R |= 0x00005000;
    // Set GPIOCR register for PF4 and PF0 to allow write access (SW1 is connected to PF4 and SW2 is connected to PF0 where Pull-up needs to be enabled)
    GPIO_PORTF_CR_R = 0xFFU;
    // Enable Pull-Up Resistor for PF4 and PF0
    GPIO_PORTF_PUR_R = 0x11U;
    // Unlocks GPIOCR register for write access
    GPIO_PORTF_LOCK_R = 0x4C4F434BU;

    // Set PORTB 3 as digital pins
    GPIO_PORTB_DEN_R |= 0x0C;
    // Set PORTB 3 as output
    GPIO_PORTB_DIR_R |= 0x0C;
    // Enable PORTB 3
    GPIO_PORTB_DATA_R |= 0x08;

    int val[] = {0, 30, 60, 100};
    int i = 1, buttonSw = 0, buttonSe = 0, clockwise = 1;

    // Enable clock to PWM1
    SYSCTL_RCGCPWM_R |= 0x02;

    // Use system clock for PWM
    SYSCTL_RCC_R &= ~0x00100000;
    // Positive pulse
    PWM1_INVERT_R |= 0x80;
    // Disable PWM1_3 during configuration
    PWM1_3_CTL_R = 0;
    PWM1_3_GENB_R = 0x0000080C;
    PWM1_3_LOAD_R = 3999;
    // Enable PWM1_3
    PWM1_3_CTL_R = 1;
    // Enable PWM1
    PWM1_ENABLE_R |= 0x80;

    // Main loop
    while (1){
        if (buttonSe == 0 && sw1 == pressed){
            buttonSe = 1;
            PWM1_3_CMPB_R = 75 * val[i];
            i = (i + 1) % (sizeof(val) / sizeof(val[0]));
        }
        else if (buttonSe == 1 && sw1 != pressed)
            buttonSe = 0;

        if (buttonSw == 0 && sw2 == pressed){
            buttonSw = 1;
            if (clockwise == 1){
                GPIO_PORTB_DATA_R &= ~0x04;
                GPIO_PORTB_DATA_R |= 0x08;
            }
            else{
                GPIO_PORTB_DATA_R &= ~0x08;
                GPIO_PORTB_DATA_R |= 0x04;
            }
            clockwise = 1 - clockwise;
        }
        else if (buttonSw == 1 && sw2 != pressed)
            buttonSw = 0;
    }
}
