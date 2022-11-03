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

    if (input > ((7/8) * max_val))
    {
        return 7;
    } else if (input > ((6/8) * max_val))
    {
        return 6;
    } else if (input > ((5/8) * max_val))
    {
        return 5;
    } else if (input > ((4/8) * max_val))
    {
        return 4;
    } else if (input > ((3/8) * max_val))
    {
        return 3;
    } else if (input > ((2/8) * max_val))
    {
        return 2;
    } else if (input > ((1/8) * max_val))
    {
        return 1;
    } else {
        return 0;
    }

}

int main()
{
    mictmr.start();
    //lighttmr.start();
    ledDisp.enable(true);

    while (true) {
        //Read Analog to Digital Converter values (16 bit)
        unsigned short potVal   = pot.read_u16();
        unsigned short lightVal = ldr.read_u16();
        unsigned short micVal   = abs(mic.read_u16() - 0x8000); 

        ledDisp.setGroup(LatchedLED::LEDGROUP::RED);
        potCount = set_count(potVal, 0xFFFF);
        ledDisp = 1 << potCount;


        ledDisp.setGroup(LatchedLED::LEDGROUP::BLUE);
        if (mictmr.read_ms() <= 500){
            ledDisp = 1 << micCount;
        } else {
            micCount = set_count(micVal, 0x5000);
            printf("pot: %x, count: %d\n", potVal, potCount);
            ledDisp = 1 << micCount;
            mictmr.reset();
            mictmr.start();

        }

    }
}



