#include "mbed.h"
using namespace uop_msb;


//BusOut lights(PC_2, PC_3, PC_6);
BusIn buttons(PG_0, PG_1, PG_2, PG_3);
LCD_16X2_DISPLAY lcd;

// main() runs in its own thread in the OS
int main()
{
    buttons[2].mode(PinMode::PullDown);
    buttons[3].mode(PinMode::PullDown);

    //lights = 0;

    while (true) {

        while (buttons == 0);
        wait_us(10000);

        //lights = buttons;

        while (buttons != 0);
        wait_us(10000);

    }
}

