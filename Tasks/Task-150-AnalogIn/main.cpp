#include "uop_msb.h"
#include <chrono>
#include <cstdint>
using namespace uop_msb;
using namespace chrono;

//Fun output stuff
LCD_16X2_DISPLAY disp;
Buzzer buzz;
LatchedLED ledDisp(LatchedLED::STRIP);

//Analogue Inputs
AnalogIn pot(AN_POT_PIN);
AnalogIn ldr(AN_LDR_PIN);
AnalogIn mic(MIC_AN_PIN);

DigitalOut green(PC_6);
DigitalOut yellow(PC_3);
DigitalOut red(PC_2);

TimerCompat mictmr;
TimerCompat tmr;

int main()
{
    mictmr.start();
    tmr.start();
    //Test LED Bar Display
    ledDisp.enable(true);

    ledDisp.setGroup(LatchedLED::LEDGROUP::RED);
    for (unsigned int n=0; n<8; n++) {
        ledDisp = 1 << n;
        wait_us(250000);
    }
    ledDisp = 0;

    ledDisp.setGroup(LatchedLED::LEDGROUP::GREEN);
    for (unsigned int n=0; n<8; n++) {
        ledDisp = 1 << n;
        wait_us(250000);
    }
    ledDisp = 0;
    
    ledDisp.setGroup(LatchedLED::LEDGROUP::BLUE);
    for (unsigned int n=0; n<8; n++) {
        ledDisp = 1 << n;
        wait_us(250000);
    }     
    ledDisp = 0;


    while (true) {
        //Read Analog to Digital Converter values (16 bit)
        unsigned short potVal   = pot.read_u16();
        unsigned short lightVal = ldr.read_u16();
        unsigned short micVal   = mic.read_u16(); 

        if (lightVal <= 0x800)
        {
            green = 1;
        } else {
            green = 0;
        }

        if (potVal >= 0x8000){
            yellow = 1;
        } else {
            yellow = 0;
        }

        if (micVal >= 0xA000)
        {
            mictmr.reset();
            red = 1;
            mictmr.start();
        } else if (mictmr.read_ms() <= 500){
            red = 1;
        } else {
            red = 0;
        }

        

        //Write to terminal
        if (tmr.read_ms() % 1000 == 0)
        {
            printf("--------------------------------\n");
            printf("Potentiometer: %X\n", potVal);
            printf("Light Dependant Resistor: %X\n", lightVal);
            printf("Microphone: %X\n", micVal); 
        } 
          


    }
}

