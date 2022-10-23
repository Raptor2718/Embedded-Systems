#include "mbed.h"

BusIn buttons(PG_0, PG_1, PG_2, PG_3);
DigitalIn ButtonC(PG_2, PinMode::PullDown);
DigitalIn ButtonD(PG_3, PinMode::PullDown);

DigitalOut red(PC_2, 0);

int main()
{
    // pin_mode(PG_2,PullDown);
    // pin_mode(PG_3,PullDown);

    while (true) {
        while(buttons < 15);
        wait_us(10000);
        red = !red; 

        while(buttons == 15);
        wait_us(10000);
        //red = !red; 
    
    }
}

