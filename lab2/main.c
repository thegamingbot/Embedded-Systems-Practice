# include "TM4C123GH6PM.h"

# define GPIO_PORTF_LED         (*((volatile unsigned long *)0x40025038))
# define GPIO_PORTF_SWITCH      (*((volatile unsigned long *)0x40025040))

# define ENABLE_COMMIT          0x01U
# define ENABLE_RED             0x02U
# define ENABLE_BLUE            0x04U
# define ENABLE_GREEN           0x08U
# define ENABLE_SWITCH          0x10U
# define DISABLE                0x00U

unsigned long colors[] = {
    // Red - index = 0
    ENABLE_RED,
    // Blue - index = 1
    ENABLE_BLUE,
    // Green - index = 2
    ENABLE_GREEN,
    // Magenta - index = 3
    ENABLE_RED | ENABLE_BLUE,
    // Yellow - index = 4
    ENABLE_RED | ENABLE_GREEN,
    // Cyan - index = 5
    ENABLE_BLUE | ENABLE_GREEN
};

void delay (unsigned long counter){
	for(unsigned long i = 0; i < counter; i++);	
}

void turnOnLED (int index){
    GPIO_PORTF_LED = colors[index];
}

void turnOffLED (){
    GPIO_PORTF_LED = DISABLE;
}

int main (){
    int index = 0, flag = 0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF;
    GPIO_PORTF_CR_R = ENABLE_COMMIT;
    GPIO_PORTF_PUR_R |= ENABLE_SWITCH;
    GPIO_PORTF_DEN_R |= (ENABLE_RED | ENABLE_BLUE | ENABLE_GREEN | ENABLE_SWITCH);
    GPIO_PORTF_DIR_R |= (ENABLE_RED | ENABLE_BLUE | ENABLE_GREEN);
    GPIO_PORTF_SWITCH = 0x01U;

    while (1){
        while (GPIO_PORTF_SWITCH != 0x00U){
            turnOnLED (index);
            flag = 1;
            delay (1000000);
            GPIO_PORTF_SWITCH = 0x00U;
        }
        if (flag == 1){
            turnOffLED ();
            index = (index + 1) % 6;
            flag = 0;
            delay (1000000);
            GPIO_PORTF_SWITCH = 0x01U;
        }
    }
}