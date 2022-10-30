#include "mbed.h"
#include "uop_msb.h"
#include <chrono>
#include <ratio>
#include <string>

using namespace uop_msb;
using namespace chrono;

DigitalIn BB(USER_BUTTON);    

LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);

typedef enum {wait_press, debounce1, wait_release, debounce2} switch_state;

switch_state BBstate = wait_press;

TimerCompat BBtmr;
TimerCompat Scrolltmr;
TimerCompat Scoretmr;

//variables for the game.
int up = 0;
int up_time = 1000;
string cacti = "                ";
int gap = 0;
int cacti_num;
int Scoretime = 1000;
int score = 0;

int main(void)
{
    lcd.cls();
    Scrolltmr.start();
    Scoretmr.start();


    while(true)
    {

        //code for when button is pressed.
        switch(BBstate)
        {
            case wait_press:
                if (up == 1)
                {
                    if (BBtmr.read_ms() >= up_time)
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
                if (BBtmr.read_ms() >= 50)
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
                    BBtmr.start();
                }

            break;

            case debounce2:
                if (BBtmr.read_ms() >= 50)
                {
                    BBstate = wait_press;
                    BBtmr.stop();
                }

            break;

        }

        if (Scrolltmr.read_ms() >= 500)
        {
            //scroll string and print 
            for (int i = 0; i < 16; i++)
            {
                cacti[i] = cacti[i+1];

                switch (i)
                {
                    case 1:
                        if ((up == 0) && (cacti[1] == 'c'))
                        {
                            lcd.cls();
                            lcd.locate(0,3);
                            lcd.printf("GAME OVER!");
                            lcd.locate(1, 8);
                            lcd.printf(":c");

                            while(true);
                        }
                    break;

                    case 15:
                        if (gap >= 3)
                        {

                            gap = 0;
                            cacti_num = rand() % 3;
                            if (cacti_num == 0)
                            {
                                cacti[15] = 'c';
                            } else {
                                cacti[15] = ' ';
                            } 

                        } else {
                            cacti[15] = ' ';
                        }
                }

                lcd.locate(1,i);
                lcd.printf("%c", cacti[i]);

            }

            Scrolltmr.reset();
            Scrolltmr.start();
            gap++;

        }

        if (Scoretmr.read_ms() >= Scoretime)
        {
            score++;
            Scoretmr.reset();
            lcd.locate(0, 13);
            lcd.printf("%d", score);
        }

        if (up == 0)
        {

            lcd.locate(1, 1);
            lcd.printf("T");
            lcd.locate(0, 1);
            lcd.printf(" ");
        } else {
    
            lcd.locate(0, 1);
            lcd.printf("T");
            lcd.locate(1, 1);
            lcd.printf(" ");
        }

    }
        
}