#include "mbed.h"

PortOut lights(PortC, 0b0000000001001100); // Bus out turns the leds at slightly different times. Port out does it at the same time.

int main()
{
    //All OFF
    lights = 0;

    while (true)
    {
        lights = 0b0000000000001100;
        wait_us(1000000);
        lights = 0b0000000001001000;
        wait_us(1000000);
        lights = 0b0000000001000100;
        wait_us(1000000);  

        //same thing as below. but 0b is only in newer cpp compilers.
        
        lights = 12;
        wait_us(500000);
        lights = 72;
        wait_us(500000);
        lights = 68;
        wait_us(500000);  


    }
}