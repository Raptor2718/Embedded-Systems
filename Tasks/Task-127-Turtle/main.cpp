#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;

LCD_16X2_DISPLAY lcd;

// DigitalOut up(PG_0);
// DigitalOut left(PG_1);
// DigitalOut right(PG_3, PinMode::PullDown);
// DigitalOut down(PG_2, PinMode::PullDown);

BusIn buttons(PG_0, PG_1, PG_2, PG_3);


int r = 0;
int c = 0;

int main()
{
    buttons[2].mode(PinMode::PullDown);
    buttons[3].mode(PinMode::PullDown);

    lcd.cls();
    lcd.locate(r, c);
    lcd.printf("0");

    while (true) {

        while(buttons == 0);

        if (buttons == 1)
        {
            r = (r+1) % 2;
        } else if (buttons == 2)
        {
            c = (c-1) % 16;
            if ( c < 0)
            {
                c += 16;
            }
        } else if (buttons == 4)
        {
            r = (r-1) % 2;
        } else
        {
            c = (c+1) % 16;
        };


        lcd.cls();
        lcd.locate(r, c);
        lcd.printf("0");

        wait_us(100000);


    }
}

