#include "mbed.h"
#include "uop_msb.h"
#include <chrono>
#include <ratio>
using namespace uop_msb;
using namespace chrono;

DigitalIn BB(USER_BUTTON);    

LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);

typedef enum {wait_press, debounce1, wait_release, debounce2} switch_state;

switch_state BBstate = wait_press;

TimerCompat BBtmr;
TimerCompat Scrolltmr;

//variables for the game.
int up = 0;
int up_time = 1000.0;

int main(void)
{
    lcd.cls();

    //code for when button is pressed.
    switch(BBstate)
    {
        case wait_press:
            if (up == 1)
            {
                if (BBtmr >= up_time)
                {
                    up = 0;
                    BBstate = debounce1;
                    BBtmr.reset();
                    BBtmr.start();
                }
            } else if (BB == 1)
            {
                up = 1;
                BBtmr.reset();
                BBtmr.start();
            }

        break;

        case debounce1:
            if (BBtmr >= 50)
            {
                BBstate = wait_release;
                BBtmr.stop();
            }

        break;

        case wait_release:
            if (BB == 0)
            {
                BBstate = debounce2;
                BBtmr.reset();
                BBtmr.stop();
            }

        break;

        case debounce2:
            if (BBtmr >= 50)
            {
                BBstate = wait_release;
                BBtmr.stop();
            }

        break;

    }

    lcd.locate(0, 0);
    lcd.printf("up = %d", up);
}