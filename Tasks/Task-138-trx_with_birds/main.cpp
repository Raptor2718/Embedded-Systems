#include "mbed.h"
#include "uop_msb.h"
#include <chrono>
#include <ratio>
#include <string>

using namespace uop_msb;
using namespace chrono;

DigitalIn BB(PG_0);   
DigitalIn DB(PG_2, PullDown);

LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);

typedef enum {wait_press, debounce1, wait_release, debounce2} switch_state;

switch_state BBstate = wait_press;
switch_state DBstate = wait_press;

TimerCompat BBtmr;
TimerCompat DBtmr;
TimerCompat Scrolltmr;
TimerCompat Scoretmr;

//variables for the game.
int up = 0;
int down = 0;
int up_time = 1000;
int down_time = 1500;
int scroll_time = 500;
int scroll_count = 1;
string birds = "                ";
string cacti = "                ";
char icon = 'T';
int gap = 0;
int gap1 = 0;
int cacti_num;
int birds_num;
int Scoretime = 1000;
int score = 0;

int main(void)
{

    //custom character setup
    const int trex_number = 0;  // range 0-7 

    const char trex[8] =
    {0b00001111,
     0b00001011,
     0b00001111,
     0b00010110,
     0b00011111,
     0b00001101,
     0b00001000,
     0b00001100};

    lcd.set_CGRAM_Address(trex_number*8);    

    for (int i=0;i<8;i++){
        lcd.write(uop_msb::LCD_16X2_DISPLAY::DATA, trex[i]);
    }

    //set up
    disp.enable(true);
    lcd.cls();
    Scrolltmr.start();
    Scoretmr.start();

    while(true)
    {

        //up_button 

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

        //down button

        switch (DBstate){
            case (wait_press):
                if (down == 1)
                {
                    if (DBtmr.read_ms() >= down_time)
                    {
                        down = 0;
                        DBstate = debounce1;
                        DBtmr.reset();
                        DBtmr.start();

                    }
                } else if (DB == 1)
                {
                    down = 1;
                    DBtmr.reset();
                    DBtmr.start();

                } 

            break;

            case debounce1:
                if (DBtmr.read_ms() >= 50)
                {
                    DBstate = wait_release;
                    DBtmr.stop();
                }

            break;

            case wait_release:
                if (DB == 0)
                {
                    DBstate = debounce2;
                    DBtmr.reset();
                    DBtmr.start();
                }

            break;

            case debounce2:
                if (DBtmr.read_ms() >= 50)
                {
                    DBstate = wait_press;
                    DBtmr.stop();
                }

            break;

        }

        //scrolling 

        if (Scrolltmr.read_ms() >= scroll_time)
        {
            //scroll string and print 
            for (int i = 0; i < 16; i++)
            {
                cacti[i] = cacti[i+1];
                birds[i] = birds[i+1];

                switch (i)
                {
                    case 1:
                        if ((up == 0) && (cacti[1] == 'c'))
                        {
                            lcd.cls();
                            lcd.locate(0,2);
                            lcd.printf("GAME OVER! :c");
                            lcd.locate(1, 3);
                            lcd.printf("score: %d", score);

                            while(true);
                        }

                        if ((down == 0) && (birds[1] == 'b'))
                        {
                            lcd.cls();
                            lcd.locate(0,2);
                            lcd.printf("GAME OVER! :c");
                            lcd.locate(1, 3);
                            lcd.printf("score: %d", score);

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

                        if (score >= 50)
                        {
                            if (gap1 >= 5)
                            {
                                gap1 = 0;
                                birds_num = rand() % 3;
                                if (birds_num == 0)
                                {
                                    birds[15] = 'b';
                                } else {
                                    birds[15] = ' ';
                                }
                            } else {
                                birds[15] = ' ';
                            }
                        } else {
                            birds[15] = ' ';
                        }
                }

                lcd.locate(1,i);
                lcd.printf("%c", cacti[i]);
                lcd.locate(0,i);
                lcd.printf("%c", birds[i]);

            }

            Scrolltmr.reset();
            Scrolltmr.start();
            gap++;
            gap1++;

        }


        if (Scoretmr.read_ms() >= Scoretime)
        {
            score++;
            Scoretmr.reset();
            disp = score;
        }

        if (up == 0)
        {
            lcd.locate(1, 1);
            lcd.printf("%c", icon);
            lcd.locate(0, 1);
            lcd.printf(" ");
        } else {
    
            lcd.locate(0, 1);
            lcd.printf("%c", icon);
            lcd.locate(1, 1);
            lcd.printf(" ");
        }

        if (down == 0)
        {
            //if ((Scoretmr.read_ms() % 200) == 0)
            icon = trex_number;
        } else {
            icon = '>';
        }

        if (score >= scroll_count*50)
        {
            scroll_time -= 45;
            scroll_count++;
        }


    }
        
}