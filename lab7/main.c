#include "tm4c123gh6pm.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RED 0x02U //define colours
#define BLUE 0x04U
#define GREEN 0x08U

void delay();
char readChar(void);
void printChar(char c);
char *readString(char delimiter);
void printString(char *string);

int main()
{
  volatile int result;
  SYSCTL_RCGCADC_R |= 1; // Analog clock gating
  SYSCTL_RCGCUART_R |= (1<<0); // Enable the UART0 Module
  SYSCTL_RCGCGPIO_R |= (1<<0)|(1<<4); // Enable clock to PORTA
  GPIO_PORTA_AFSEL_R = (1<<1)|(1<<0); // Enable PA0 and PA1 As Alternate Function PIN
  GPIO_PORTA_PCTL_R |= 0x00000001|0x00000010; // make PA0 PA1 UART output pin
  GPIO_PORTA_DEN_R |= (1<<0)|(1<<1); // Enable digital on PA0 PA1
  UART0_CTL_R &= ~(1<<0); // Disable the UART by clearing the UARTEN
  UART0_IBRD_R = 104; // integer portion of the BRD
  UART0_FBRD_R = 11; // fractional portion of the BRD
  UART0_LCRH_R = (0x3<<5)|(1<<4); // 8-bit, no parity, 1-stop bit
  UART0_CTL_R = (1<<0)|(1<<8)|(1<<9); // Enable the UART by setting the UARTEN bit
  
  GPIO_PORTE_AFSEL_R |= 8; // Enable alternate function
  GPIO_PORTE_DIR_R &= ~8; // Disable digital function
  GPIO_PORTE_AMSEL_R |= 8; // Enable analog function
  ADC0_ACTSS_R &= ~8; // Disable SS3 during configuration
  ADC0_EMUX_R &= ~0xF000; // Software trigger conversion
  ADC0_SSMUX3_R = 0; // get input from channel 0
  ADC0_SSCTL3_R |= 6; // take one sample at a time, set flag at 1st sample
  ADC0_ACTSS_R |= 8; // enable ADC0 sequencer 3
  
  SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock on GPIO portF
  GPIO_PORTF_DEN_R=(1<<1)|(1<<2)|(1<<3); // Enable digital on PF1 PF2 PF3
  GPIO_PORTF_DIR_R=(1<<1)|(1<<2)|(1<<3); // Enable digital output on PF1 PF2 PF3
  GPIO_PORTF_DATA_R = ~((1<<1)|(1<<2)|(1<<3)); // Disable digital output on PF1 PF2 PF3
  
  char *string = "pink";
  while (1)
  {

      if (string[0] != 'r' || string[0] != 'g' || string[0] != 'b' || string[0] != 'v')
          string = readString('\r'); // read string from serial port

      if (string[0] == 'r' || string[0] == 'g' || string[0] == 'b' || string[0] == 'y' || string[0] == 'm' || string[0] == 'c' || string[0] == 'w')
      {
          if (string[0] == 'r') // based on the first character which is sent by python program, light up LED
              GPIO_PORTF_DATA_R = RED;
          else if (string[0] == 'g')
              GPIO_PORTF_DATA_R = GREEN;
          else if (string[0] == 'b')
              GPIO_PORTF_DATA_R = BLUE;
          else if (string[0] == 'y')
              GPIO_PORTF_DATA_R = RED | GREEN;
          else if (string[0] == 'm')
              GPIO_PORTF_DATA_R = RED | BLUE;
          else if (string[0] == 'c')
              GPIO_PORTF_DATA_R = BLUE | GREEN;
          else
              GPIO_PORTF_DATA_R = RED | GREEN | BLUE;
      }
      else if (string[0] == 'v') // if user wants to generate line graph
      {

          GPIO_PORTF_DATA_R = 0; // turn of LEDs
          while (1)              // take 50 readings of analog voltage
          {
              ADC0_PSSI_R |= 8;          /* start a conversion sequence 3 */
              if ((ADC0_RIS_R & 8) == 0) /* wait for conversion complete */
              {
                  result = ADC0_SSFIFO3_R; /* read conversion result */
                  float volt = result * 0.0008;
                  char snum[5];
                  sprintf(snum, "%0.3f", volt);
                  printString(snum); // send analog voltage readings to python

                  printString("\n\r");
                  delay();
              }
              ADC0_ISC_R = 8; /* clear completion flag*/
          }
      }
  }
  
  return 0;
}

void delay() 
{
 int volatile counter=0;
 while(counter<1000000)
  ++counter;
 }

char readChar(void) // read computer char by char to RX FIFO buffer
{
 char c;
 while((UART0_FR_R & (1<<4)) != 0);
 c = UART0_DR_R;
 return c;
}

void printChar(char c)
{
 while((UART0_FR_R & (1<<5)) != 0);
  UART0_DR_R = c;
}

char *readString(char delimiter)
{
    int stringSize = 0;
    char *string = (char *)calloc(10, sizeof(char));
    char c = readChar();
    printChar(c);
    while (c != delimiter)
    {
        *(string + stringSize) = c;
        stringSize++;
        c = readChar();
        printChar(c); // display the character the user typed
    }
    return string;
}

void printString(char * string)
{
  while(*string){
  printChar(*(string++));
  }
}