#include "PinNames.h"
#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;
using namespace chrono;

LatchedLED ledDisp(LatchedLED::STRIP);

AnalogIn pot(AN_POT_PIN);
AnalogIn ldr(AN_LDR_PIN);
AnalogIn mic(MIC_AN_PIN);

DigitalOut green(PC_6);
DigitalOut yellow(PC_3);
DigitalOut red(PC_2);

TimerCompat mictmr;
TimerCompat lighttmr;

int lightCount;
int micCount;
int potCount;

int set_count(unsigned short input, unsigned short max_val)
{

    if (input > 7/8 * max_val)
    {
        return 8;
    } else if (input > 6/8 * max_val)
    {
        return 7;
    } else if (input > 5/8 * max_val)
    {
        return 6;
    } else if (input > 4/8 * max_val)
    {
        return 5;
    } else if (input > 3/8 * max_val)
    {
        return 4;
    } else if (input > 2/8 * max_val)
    {
        return 3;
    } else if (input > 1/8 * max_val)
    {
        return 2;
    } else {
        return 1;
    }

}

int main()
{
    mictmr.start();
    lighttmr.start();
    ledDisp.enable(true);

    while (true) {
        //Read Analog to Digital Converter values (16 bit)
        unsigned short potVal   = pot.read_u16();
        unsigned short lightVal = ldr.read_u16();
        unsigned short micVal   = abs(mic.read_u16() - 0x8000); 

        ledDisp.setGroup(LatchedLED::LEDGROUP::GREEN);
        if (lighttmr.read_ms() <= 500){
            ledDisp = 2^lightCount - 1;
        } else {
            lightCount = set_count(lightVal, 0xFFFF);
            ledDisp = 2^lightCount - 1;
            lighttmr.reset();
            lighttmr.start();

        }

        ledDisp.setGroup(LatchedLED::LEDGROUP::RED);
        potCount = set_count(potVal, 0xFFFF);
        ledDisp = 2^potCount - 1;


        ledDisp.setGroup(LatchedLED::LEDGROUP::BLUE);
        if (mictmr.read_ms() <= 500){
            ledDisp = 2^micCount - 1;
        } else {
            micCount = set_count(micVal, 0x8000);
            ledDisp = 2^micCount - 1;
            mictmr.reset();
            mictmr.start();

        }

    }
}



