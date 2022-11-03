#include "PinNames.h"
#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;
using namespace chrono;

LatchedLED ledDisp(LatchedLED::STRIP);

LCD_16X2_DISPLAY lcddisp;


AnalogIn pot(AN_POT_PIN);
AnalogIn ldr(AN_LDR_PIN);
AnalogIn mic(MIC_AN_PIN);

DigitalOut green(PC_6);
DigitalOut yellow(PC_3);
DigitalOut red(PC_2);


int set_count(unsigned short input, unsigned short max_val)
{

    if (input > (7.0/8 * max_val))
    {
        return 7;
    } else if (input > (6.0/8 * max_val))
    {
        return 6;
    } else if (input > (5.0/8 * max_val))
    {
        return 5;
    } else if (input > (4.0/8 * max_val))
    {
        return 4;
    } else if (input > (3.0/8 * max_val))
    {
        return 3;
    } else if (input > (2.0/8 * max_val))
    {
        return 2;
    } else if (input > (1.0/8 * max_val))
    {
        return 1;
    } else {
        return 0;
    }

}

int main()
{
    
    ledDisp.enable(true);
    lcddisp.cls();

    while (true) {
        //Read Analog to Digital Converter values (16 bit)
        unsigned short potVal   = pot.read_u16();
        unsigned short lightVal = ldr.read_u16();
        unsigned short micVal   = abs(mic.read_u16() - 0x8000); 

        ledDisp.setGroup(LatchedLED::LEDGROUP::RED);
        ledDisp = 1 << set_count(potVal, 0xFFFF);

        ledDisp.setGroup(LatchedLED::LEDGROUP::GREEN);
        ledDisp = 1 << set_count(lightVal, 0xFFFF);
        
        ledDisp.setGroup(LatchedLED::LEDGROUP::BLUE);
        ledDisp = 1 << set_count(micVal, 0x2000);

    }
}



