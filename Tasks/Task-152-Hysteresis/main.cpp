#include "uop_msb.h"
#include <chrono>
#include <cstdint>
using namespace uop_msb;
using namespace chrono;

//Output
DigitalOut redLED(TRAF_RED1_PIN);
DigitalOut yellowLED(TRAF_YEL1_PIN);
DigitalOut greenLED(TRAF_GRN1_PIN);

//Analogue Inputs
AnalogIn pot(AN_POT_PIN);
Buzzer buzz;

typedef enum {on, off} state;

state red_state = off;

int main()
{

    while (true) {
        //Read Analog to Digital Converter values (16 bit)
        unsigned short potVal   = pot.read_u16();
        printf("Potentiometer: %X\n", potVal);

        switch (red_state)
        {
            case on:
                if (potVal <= 0x7000)
                {
                    redLED = 0;
                    red_state = off;
                    buzz.rest();
                }
            break;

            case off:
                if (potVal >= 0x9000)
                {
                    redLED = 1;
                    red_state = on;
                    buzz.playTone("C");
                }
            break;
        }

        //Wait 0.5 seconds
        wait_us(500000);

    }  
}

