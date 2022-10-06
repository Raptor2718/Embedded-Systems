// You need this to use the Module Support Board
#include "uop_msb.h"
using namespace uop_msb;

#define WAIT_TIME_MS 500000 
DigitalOut greenLED(TRAF_GRN1_PIN);
Buzzer buzz;
Buttons buttons;

void play(int n, int secs)
{
    greenLED = 1;

    switch (n)
    {
        case 1: buzz.playTone("C");
        break;
        case 2: buzz.playTone("D");
        break;
        case 3: buzz.playTone("E");
        break;
        case 4: buzz.playTone("F");
        break;
        case 5: buzz.playTone("G");
        break;
        case 6: buzz.playTone("A", Buzzer::HIGHER_OCTAVE);
        break;
        case 7: buzz.playTone("B", Buzzer::HIGHER_OCTAVE);
        break;
    }
    wait_us(WAIT_TIME_MS * (secs - 0.1));

    buzz.rest();
    wait_us(WAIT_TIME_MS * 0.1);
};

// TIP: (I suggest you read this!)
//
// Press the black reset button to restart the code (and stop the sound)
// Otherwise, the noise can be "distracting" :)
int main()
{
    //Wait for the BLUE button to be pressed (otherwise this becomes super annoying!)
    while (buttons.BlueButton == 0);
    
    //Repeat everything "forever" (until the power is removed or the chip is reset)
    while (true)
    {

        play(1, 2);
        play(1, 1);
        play(2, 2);
        play(1, 2);
        play(4, 2);
        play(3, 2);
        wait_us(WAIT_TIME_MS * 2);
        play(1, 2);
        play(1, 1);
        play(2, 2);
        play(1, 2);
        play(5, 2);
        play(4, 2);
        wait_us(WAIT_TIME_MS * 2);
        play(1, 2);
        play(1, 2);
        play(6, 2);
        play(5, 2);
        play(3, 2);
        play(2, 2);
        wait_us(WAIT_TIME_MS * 2);
        play(6, 4);
        play(4, 2);
        play(5, 4);
        play(4, 4);


        
        
        //Pause
        buzz.rest();
        wait_us(WAIT_TIME_MS * 4);

    }
}