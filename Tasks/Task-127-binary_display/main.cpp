#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;

LCD_16X2_DISPLAY lcd;

//BusOut lights(PC_2, PC_3, PC_6);
BusIn buttons(PG_0, PG_1, PG_2, PG_3);
int num;

int main()
{
    buttons[2].mode(PinMode::PullDown);
    buttons[3].mode(PinMode::PullDown);

    lcd.cls();
    lcd.locate(0, 0);

    //lights = 0;

    while (true) {

        while (buttons == 0);
        wait_us(10000);

        num = buttons;

        lcd.printf("%i ", num);
        //lights = buttons;

        while (buttons != 0);
        wait_us(10000);

    }
}

