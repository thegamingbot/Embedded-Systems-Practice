#include <stdint.h>
#include "tm4c123gh6pm.h"
#define red_on 0x02U
#define blue_on 0x04U
#define green_on 0x08U

// Defining function pointer
typedef void (*glow)();

// red_on
void red()
{
    GPIO_PORTF_DEN_R = red_on;
}
// blue_on
void blue()
{
    GPIO_PORTF_DEN_R = blue_on;
}
// green_on
void green()
{
    GPIO_PORTF_DEN_R = green_on;
}
// MAGENTA
void red_blue()
{
    GPIO_PORTF_DEN_R = (red_on | blue_on);
}
// YELLOW
void red_green()
{
    GPIO_PORTF_DEN_R = (red_on | green_on);
}
// CYAN
void blue_green()
{
    GPIO_PORTF_DEN_R = (blue_on | green_on);
}
// WHITE
void white()
{
    GPIO_PORTF_DEN_R = (red_on | blue_on | green_on);
}

// Dealy function 
void delay(int time)
{
    int i, j;
    for (i = 0; i < time; i++)
    {
        for(j = 0; j < 3180; j++);
    }
}

int main(void)
{ 
  // PWM
  int intensity = 15999;
  // Enable Peripheral Clocks  
  // Enable clock to PWM1
  SYSCTL_RCGCPWM_R |= 2;
  // Enable clock to PORTF           
  SYSCTL_RCGCGPIO_R |= 0x20;   
  // No pre-divide for PWM clock
  SYSCTL_RCC_R &= ~0x00100000; 
  // Enable port PF2 for PWM1 M1PWM7  
  // PF2 uses alternate function
  GPIO_PORTF_AFSEL_R = 0x0E;     
  GPIO_PORTF_PCTL_R |= 0x000005550; 
  // Pin digital
  GPIO_PORTF_DEN_R = 0x02;
  // Stop counter       
  PWM1_2_CTL_R = 0;
  // M1PWM6 output set when reload            
  PWM1_2_GENB_R = 0x0000008C;
  // Stop counter  
  PWM1_3_CTL_R = 0; 
  // M1PWM6 output set when reload             
  PWM1_3_GENA_R = 0x0000008C; 
  // M1PWM6 output set when reload 
  PWM1_3_GENB_R = 0x0000008C;  
  // Clear when match PWMCMPA  
  // Set load value for 1kHz (16MHz/16000)
  PWM1_2_LOAD_R = 16000;  
  // Set duty cycle to min       
  PWM1_2_CMPA_R = 15999;  
  // Start timer       
  PWM1_2_CTL_R = 1;     
  // Set load value for 1kHz (16MHz/16000)         
  PWM1_3_LOAD_R = 16000;   
  // Set duty cycle to min      
  PWM1_3_CMPA_R = 15999;  
  // Start timer       
  PWM1_3_CTL_R = 1;     
  // Start PWM1 ch7          
  PWM1_ENABLE_R = 0xE0;        
  
    glow color[] = {
        // Red = 0
        red,
        // Blue = 1
        blue,
        // Green = 2
        green,
        // Magenta = 3
        red_blue,
        // Yellow = 4
        red_green,
        // Cyan = 5
        blue_green,
        // White = 6
        white
    };
  int index;
  while(1)
  {
      color[index]();
      index = (index + 1) % 7;
      // Increasing the brightness of LED
     while (intensity > 0)
        {
            // Set the intensity to the PWM registers
            intensity = intensity - 200;
            PWM1_2_CMPA_R = intensity;
            PWM1_3_CMPA_R = intensity;
            delay(10);
        }
        // Decreasing the brightness of LED
        while (intensity < 15999)
        {
            intensity = intensity + 200;
            // Set the intensity to the PWM registers
            PWM1_2_CMPA_R = intensity;
            PWM1_3_CMPA_R = intensity;
            delay(10);
        }
  }
}