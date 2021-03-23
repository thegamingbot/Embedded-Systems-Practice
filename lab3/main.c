#include "TM4C123GH6PM.h"

# define DISABLE_LOCK           0x4C4F434BU
# define ENABLE_COMMIT          0xFFU
# define ENABLE_RED             0x02U
# define ENABLE_BLUE            0x04U
# define ENABLE_GREEN           0x08U
# define ENABLE_SWITCH_1        0x01U
# define ENABLE_SWITCH_2        0x10U
# define DISABLE                0x00U

unsigned long primaryColors[] = {
    // Red - index = 0
    ENABLE_RED,
    // Blue - index = 1
    ENABLE_BLUE,
    // Green - index = 2
    ENABLE_GREEN,
};

unsigned long secondaryColors[] = {
    // Magenta - index = 0
    ENABLE_RED | ENABLE_BLUE,
    // Yellow - index = 1
    ENABLE_RED | ENABLE_GREEN,
    // Cyan - index = 2
    ENABLE_BLUE | ENABLE_GREEN,
    // White - index = 3
    ENABLE_RED | ENABLE_BLUE | ENABLE_GREEN
};

void delay (unsigned long counter){
	for(unsigned long i = 0; i < counter; i++);	
}

void turnOnLED (unsigned long color){
    GPIO_PORTF_DATA_R |= color;
}

void turnOffLED () {
    GPIO_PORTF_DATA_R = DISABLE;
}

int main(){
    int primaryIndex = 0, secondaryIndex = 0, primaryFlag = 0, secondaryFlag = 0, whiteFlag = 0;
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOF;
    GPIO_PORTF_LOCK_R = DISABLE_LOCK;
    GPIO_PORTF_CR_R = ENABLE_COMMIT;
    GPIO_PORTF_PUR_R = ENABLE_SWITCH_1 | ENABLE_SWITCH_2;
    GPIO_PORTF_DEN_R = (ENABLE_RED | ENABLE_BLUE | ENABLE_GREEN | ENABLE_SWITCH_1 | ENABLE_SWITCH_2);
    GPIO_PORTF_DIR_R = (ENABLE_RED | ENABLE_BLUE | ENABLE_GREEN);

    while (1){
        if (GPIO_PORTF_DATA_R == 0x01U){
            turnOnLED (primaryColors[0]);
        }
        else if (GPIO_PORTF_DATA_R == 0x10U){
            turnOnLED (primaryColors[1]);
        }
        else if (GPIO_PORTF_DATA_R == 0x00U){
            turnOnLED (primaryColors[2]);
        }
        else {
            turnOffLED();
        }
    }
}


/*
Primary colors() are displayed by the switch in pin 4
Secondary colors are displayed by the switch in pin 0
White is displayed by the both the switches

So the data in 0x400253FC when:
 - Red glows: 0x02 | 0x01 = 0x03
 - Magenta glows: 0x06 | 0x10 = 0x16
 - White glows: 0x0e | 0x00 = 0x0e
 - Blue glows: 0x04 | 0x01 = 0x05
 - Yellow glows: 0x0a | 0x10 = 0x1a
 - White glows: 0x0e | 0x00 = 0x0e
 - Green glows: 0x08 | 0x01 = 0x09
 - Cyan glows: 0x0c | 0x10 = 0x1c
 - White glows: 0x0e | 0x00 = 0x0e
*/
