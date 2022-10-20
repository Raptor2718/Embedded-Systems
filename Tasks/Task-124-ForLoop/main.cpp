#include "mbed.h"

//DigitalOut redLED(PC_2,0);
// DigitalOut yellowLED(PC_3,0);
// DigitalOut greenLED(PC_6,0);
BusOut leds(PC_2, PC_3, PC_6);
//PortOut leds(PortC, 0b0000000001001100); you cant actually use portOut with this code. remember it takes the binary of all the pins.
// main() runs in its own thread in the OS

int main()
{
    while (true) {

        //For-Loop
        for (int i = 7; i >= 0; i++)
        {
            wait_us(1000000); 
            if (i%3 == 0)
            {
                continue;   //skips odd numbers
            }
            leds = i;

        }

        //2s pause
        wait_us(2000000);
    }
}

