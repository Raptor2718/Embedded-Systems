#include "mbed.h"
#include "uop_msb.h"
#include <string>

using namespace uop_msb;

LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);



// main() runs in its own thread in the OS

string st = "                ";
int random_num;

int main()
{
    lcd.cls();

    lcd.locate(0,0);
    lcd.printf("random number: ");



    while (true) {

        
        lcd.locate(0,15);
        random_num = rand() % 6;
        lcd.printf("%i", random_num);

        for (int i = 0; i < 15; i++)
        {
            st[i] = st[i+1];
            lcd.locate(1, i);
            lcd.printf("%c", st[i]);
        }

        if (random_num == 0)
        {
            st[15] = '*';
            lcd.locate(1, 15);
            lcd.printf("%c", st[15]);

        } else {

            st[15] = ' ';
            lcd.locate(1, 15);
            lcd.printf("%c", st[15]);
        }

        wait_us(1000000);
    }
}

