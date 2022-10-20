#include "mbed.h"
#define WAIT_TIME_MS 1000 


// DigitalOut redLED(PC_2,1);
// DigitalOut yellowLED(PC_3,1);
// DigitalOut greenLED(PC_6,1);

BusOut lights(PC_2, PC_3, PC_6, PC_7, PC_8, PC_9);
//BusOut lights(PC_2, PC_3, PC_6, PB_0, PB_7, PB_14);

int main()
{
    pin_mode(PC_7, OpenDrainNoPull);
    //All OFF
    lights = 0;

    while (true)
    {
        for (int i = 0; i < 64; i++)
        {
            lights = i;
            wait_us(1000000);
        }
        
    }
}