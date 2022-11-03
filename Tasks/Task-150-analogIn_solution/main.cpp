#include "PinNames.h"
#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;

DigitalOut red(PC_2);
AnalogIn mic(MIC_AN_PIN);


TimerCompat mictmr;
int counter = 0;
unsigned long micTotal;

int main()
{
    mictmr.start();
    while (true)
    {
        unsigned int micVal   = mic.read_u16();

        if (counter < 5)
        {
            if (mictmr.read_ms() >= 0.25)
            {
                micTotal += micVal; 
                mictmr.reset();
                mictmr.start();
                counter++;
            }
        } else {
            if (micTotal/5 >= 0xFFFF * 3/4)
            {
                red = 1;
                wait_us(500000);
            } else {
                red = 0;
            }

            mictmr.reset();
            mictmr.start();
            counter = 0;
            micTotal = 0;
        };
    }
}

