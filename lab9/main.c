#include "TM4C123GH6PM.h"

/* Function prototype of SPI and Delay */
void initializeSPI1();
void writeSPI1(unsigned char data);
void delay(int time); 

/* Main routine of code */
int main(){
    unsigned char val1 = 'A';
	unsigned char val2 = 'B';

    initializeSPI1();
    while(1){              
        writeSPI1(val1);
        delay(1000);
        writeSPI1(val2);
        delay(1000);	
    }
}

/* A function to write to SPI1 */
void writeSPI1(unsigned char data){
    GPIO_PORTF_DATA_R &= ~(1<<2);             /* Make PF2 Selection line (SS) low */
    while((SSI1_SR_R & 2) == 0);         /* Wait untill Tx FIFO is not full */
    SSI1_DR_R = data;                    /* Transmit byte over SSI1Tx line */
    while(SSI1_SR_R & 0x10);             /* Wait until transmit complete */
    GPIO_PORTF_DATA_R |= 0x04;                /* Keep selection line (PF2) high in idle condition */
}

/* A function to initialize SPI1 */
void initializeSPI1(){
    /* Enable clock to SPI1, GPIOD and GPIOF */
   	SYSCTL_RCGCSSI_R |= (1<<1);         /* Set clock enabling bit for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1<<3);        /* Enable clock to GPIOD for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1<<5);        /* Enable clock to GPIOF for slave select */

    /*Initialize PD3 and PD0 for SPI1 alternate function*/
    GPIO_PORTD_AMSEL_R &= ~0x09;        /* Disable analog functionality RD0 and RD3 */
    GPIO_PORTD_DEN_R |= 0x09;           /* Set RD0 and RD3 as digital pin */
    GPIO_PORTD_AFSEL_R |= 0x09;         /* Enable alternate function of RD0 and RD3*/
    GPIO_PORTD_PCTL_R &= ~0x0000F00F;   /* Assign RD0 and RD3 pins to SPI1 */
    GPIO_PORTD_PCTL_R |= 0x00002002;    /* Assign RD0 and RD3 pins to SPI1  */
    
    /* Initialize PF2 as a digital output as a slave select pin */
    GPIO_PORTF_DEN_R |= (1<<2);         /* Set PF2 pin digital */
    GPIO_PORTF_DIR_R |= (1<<2);         /* Set PF2 pin output */
    GPIO_PORTF_DATA_R |= (1<<2);        /* Keep SS idle high */

    /* Select SPI1 as a Master, POL = 0, PHA = 0, clock = 4 MHz, 8 bit data */
    SSI1_CR1_R = 0;                     /* Disable SPI1 and configure it as a Master */
    SSI1_CC_R = 0;                      /* Enable System clock Option */
    SSI1_CPSR_R = 4;                    /* Select prescaler value of 4 .i.e 16MHz/4 = 4MHz */
    SSI1_CR0_R = 0x00007;               /* 4MHz SPI1 clock, SPI mode, 8 bit data */
    SSI1_CR1_R |= 2;                    /* Enable SPI1 */
}

/* This function generates delay in ms */
void delay(int time){
    for(int i = 0; i < time; i++)
        for(int j = 0; j < 3180; j++);
}