#include "mbed.h"
//#define wait_us(us) _wait_us_inline(us)
#define wait_ms(ms) wait_us(ms*1000)
DigitalOut redLED(PC_2);
DigitalOut yellowLED(PC_3);
DigitalOut greenLED(PC_6);

void light(int led, int phase)
{
    switch(led)
    {
        case 0: redLED = phase;
        break;
        case 1: yellowLED = phase;
        break;
        default: greenLED = phase;
    }

    wait_ms(2000);

}

// main() runs in its own thread in the OS
int main()
{
    while (true) {
        light(0, 1);
        light(0, 0);

        redLED = 1; //because the function light waits 2s
        light(1, 1);
        redLED = 0;
        light(1, 0);
        

        light(2, 1);
        light(2, 0);

        for (int i = 0; i < 4; i++)
        {
            yellowLED = 1;
            wait_ms(250);
            yellowLED = 0;
            wait_ms(250);
        };
    }
}

