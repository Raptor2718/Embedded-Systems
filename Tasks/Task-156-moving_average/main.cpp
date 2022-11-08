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
AnalogIn ldr(AN_LDR_PIN);
Buzzer buzz;

LCD_16X2_DISPLAY lcd;

double average = 0;

typedef enum {on, off} state;

state green_state;


int main()
{
    unsigned short samples[100];
    unsigned short new_samples[100];

    for (unsigned int m=0; m<100; m++) {
        printf("%X ", samples[m]);
    }

    for (unsigned int m=0; m<100; m++) {
        samples[m] = 0;
        new_samples[m] = 0;
    }

    // Automatic headlamp 
    while (true) {

        for (unsigned int m=0; m<100; m++) {
            unsigned short ldrVal   = ldr.read_u16();
            samples[m] = ldrVal;
            wait_us(10000);          // 10ms
        }

        // TASK a. Calculate the average value in samples
        for (int i = 0; i < 100; i++)
        {
            average+= samples[i];
        }
        average = average/100;

        // TASK b. Display to 1dp
        printf("average brightness = %5.1A\n", average); //why do I get weird long values when I try to show this in hex. %X

        // TASK c. Switch green LED on when dark;
        switch (green_state)
        {
            case on:
                if (average <= 0xA000)
                {
                    green_state = off;
                    greenLED = 0;
                }
            break;

            case off: 
                if (average >= 0xD000)
                {
                    green_state = on;
                    greenLED = 1;
                }
            break;

        }


    }  
}

