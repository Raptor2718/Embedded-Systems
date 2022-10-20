#include "mbed.h"

DigitalIn ButtonA(PG_0); //Button A
DigitalOut redLED(PC_2); //Red Traffic 1

// main() runs in its own thread in the OS
int main()
{
    int btnA;
    // Turn OFF the red LED
    redLED = 0;

    while (true) {
    
        // Wait for the button to be pressed
        do {
            //btnA = ButtonA; //Read button A
        } while (!ButtonA);


        //Toggle the red LED
        redLED = !redLED;

        //Wait for noise to settle
        wait_us(10000);

        // Wait for the button to be released
        do {
            //btnA = ButtonA; //Read button A
        } while (ButtonA);

        //Wait for noise to settle
        redLED = !redLED;
        
        wait_us(10000);
    }
}

