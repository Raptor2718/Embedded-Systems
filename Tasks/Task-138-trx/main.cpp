#include "uop_msb.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ratio>
#include <string>

using namespace uop_msb;
using namespace chrono;

#include "mbed.h"

LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);

//Count variable
unsigned counter=0;
int times;
string cacti = "                ";
int jump = 0;
int obstacle; 

int up = 0;

InterruptIn btnA(BTN1_PIN);
Ticker tick;

DigitalOut greenLED(PC_6);


void funcA()
{
    jump = 1;
}

void funcTmr()
{
    jump = 2;
}

int main()
{
    lcd.locate(1, 1);
    lcd.printf("T");
    //Set up interrupts
    btnA.rise(&funcA);
    tick.attach(&funcTmr, 500ms);
    
    //Main loop - mostly sleeps :)
    while (true) {
        sleep();

        if (jump == 1)
        {
            // lcd.locate(1, 1);
            // lcd.printf(" ");
            lcd.locate(0, 1);
            lcd.printf("T");
            up = 1;

        //wait for 2 seconds in air
            while(times == (times+4));  

            up = 0;

            // lcd.locate(1, 1);
            // lcd.printf("T");
            lcd.locate(0, 1);
            lcd.printf(" ");

        } else if (jump == 2) {

            greenLED = !greenLED;
            //increments counter
            times++;

            obstacle = rand() % 6;

    //scroll cacti to the left
            for (int i = 0; i < 15; i++)
            {
                cacti[i] = cacti[i+1];
                lcd.locate(1, i);

                if (i == 1)
                {
                    if (up == 0)
                    {
                        lcd.printf("T");

                        if (cacti[1] == 'c')
                        {
                            lcd.cls();
                            lcd.locate(0, 3);
                            lcd.printf("game over!");
                            lcd.locate(1, 8);
                            lcd.printf(":c");

                            while(true);
                        } 

                        continue;
                    }
                    
                }
                lcd.printf("%c", cacti[i]);
            }

            if (obstacle == 0)
            {
                cacti[15] = 'c';
            } else {
                cacti[15] = ' ';
            }

            lcd.locate(1, 15);
            lcd.printf("%c", cacti[15]);

        }

        jump = 0;

    }
}





