void turnOnRed () {
    // Red LED data register on portF
    *((unsigned int *) 0x40025008) = 0x02U;
    // Blue LED data register on portF
    *((unsigned int *) 0x40025010) = 0x00U;
    // Green LED data register on portF
    *((unsigned int *) 0x40025020) = 0x00U;
}

void turnOnBlue () {
    // Red LED data register on portF
    *((unsigned int *) 0x40025008) = 0x00U;
    // Blue LED data register on portF
    *((unsigned int *) 0x40025010) = 0x04U;
    // Green LED data register on portF
    *((unsigned int *) 0x40025020) = 0x00U;
}

void turnOnGreen () {
    // Red LED data register on portF
    *((unsigned int *) 0x40025008) = 0x00U;
    // Blue LED data register on portF
    *((unsigned int *) 0x40025010) = 0x00U;
    // Green LED data register on portF
    *((unsigned int *) 0x40025020) = 0x08U;
}

void delay(int time){
    for (int j = 0; j < time; j++)
        for (int i = 0; i < 1000000; i++);
}

int main(){
    // Clock gating register
    *((unsigned int *) 0x400FE608) = 0x20U;
    // Direction for input/output
    *((unsigned int *) 0x40025400) = 0x0EU;
    // Digital function register
    *((unsigned int *) 0x4002551C) = 0x0EU;

    while (1){
        turnOnRed ();
        delay(3);
        turnOnBlue ();
        delay(3);
        turnOnGreen ();
        delay(3);
    }
}

/*
Frequency: 10 MHz (Goto Simulator -> Simulated frequency)
Time period = 1 / F = 1 / 10M = 1us
Each for loop has 5 instructions:
    - 2 * LDR (4 cycles)
    - CMP (1 cycle)
    - BLT (3 cycles, if branch taken)
    - ADD (1 cycle)
Time taken for 1 loop = 9us (approx 10us)
Number of loops required for 1 second = 1 / 10us = 1000000
Number of loops required for 2 second = 2 / 10us = 2000000
Number of loops required for 3 second = 3 / 10us = 3000000
*/