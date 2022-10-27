#include "mbed.h"
#include "uop_msb.h"


LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);

// main() runs in its own thread in the OS
int main()
{
    lcd.cls();

    lcd.locate(0,1);
    lcd.printf("random number: ");

    while (true) {

        lcd.locate(0,15);

        wait_us(2000000);
    }
}

