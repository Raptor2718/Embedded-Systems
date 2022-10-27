#include "uop_msb.h"
#include <chrono>
#include <cstdio>
#include <ratio>
#include <string>
using namespace uop_msb;
using namespace chrono;

#include "mbed.h"

LCD_16X2_DISPLAY lcd;
LatchedLED disp(LatchedLED::SEVEN_SEG);


//Count variable
int counter=0;
int times;
string cacti = "                ";

InterruptIn btnA(BTN1_PIN);
InterruptIn btnB(BTN2_PIN);
Ticker tick;

DigitalOut redLED(TRAF_RED1_PIN);       //Red Traffic 1
DigitalOut yellowLED(TRAF_YEL1_PIN);    //Yellow Traffic 1
DigitalOut greenLED(TRAF_GRN1_PIN);     //Green Traffic 1

//Dual Digit 7-segment Display

int obstacle = (rand() * 10) % 4;


void funcA()
{
    lcd.locate(1, 1);
    lcd.printf(" ");
    lcd.locate(0, 1);
    lcd.printf("T");

    while(times == (times+2));

    lcd.locate(1, 1);
    lcd.printf("T");
    lcd.locate(0, 1);
    lcd.printf(" ");
}

void funcB()
{
    yellowLED = !yellowLED;
}

void funcTmr()
{
    greenLED = !greenLED;
    times++;

    obstacle = (rand() * 12) % 4;

    if (obstacle == 0 )
    {
        cacti[15] = 'c';
    }

    for (int i = 0; i < 15; i++)
    {
        cacti[i] = cacti[i + 1];
    }
    cacti[15] = ' ';

    for (int i = 0; i < 16; i++)
    {
        lcd.locate(1, i);
        lcd.printf("%c",cacti[i]);
    }
}

int main()
{
    lcd.locate(1, 1);
    lcd.printf("T");
    //Set up interrupts
    btnA.rise(&funcA);
    btnB.fall(&funcB);
    tick.attach(&funcTmr, 500ms);
    
    //Main loop - mostly sleeps :)
    while (true) {
        sleep();

        printf("I have been woken %d times\n", ++counter);
    }
}





