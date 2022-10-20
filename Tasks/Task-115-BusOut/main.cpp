#include "mbed.h"
#define WAIT_TIME_MS 1000 


DigitalOut A(PC_7,1);
DigitalOut B(PC_8,1);
DigitalOut C(PC_9,1);

//BusOut lights(PC_2, PC_3, PC_6, PB_0, PB_7, PB_14);

BusOut lights(PC_2, PC_3, PC_6, PC_7, PC_8, PC_9);


int main()
{
    pin_mode(PC_7, OpenDrainNoPull);
    pin_mode(PC_8, OpenDrainNoPull);
    pin_mode(PC_9, OpenDrainNoPull);

    //All OFF
    lights = 0;
    A = 1;
    B = 1;
    C = 1;

    while (true)
    {
        for (int i = 0; i < 64; i++)
        {
            
            lights = i;
            // the below is because the transistors invert it.
            A = !A;
            B = !B;
            C = !C;

            wait_us(1000000);
        }
        
    }
}