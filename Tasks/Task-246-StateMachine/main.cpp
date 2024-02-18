#include "Btn.h"
#include "uop_msb.h"
#include <chrono>
#include <ratio>
using namespace uop_msb;
using namespace chrono;

BusIn buttons(BTN1_PIN, BTN2_PIN, BTN3_PIN, BTN4_PIN);

DigitalOut redLED(TRAF_RED1_PIN);       //Red Traffic 1
DigitalOut yellowLED(TRAF_YEL1_PIN);    //Yellow Traffic 1
DigitalOut greenLED(TRAF_GRN1_PIN);     //Green Traffic 1

//Dual Digit 7-segment Display
LatchedLED disp(LatchedLED::SEVEN_SEG);

//Timers
Timer tmrLED;

int main()
{
    volatile int count = 0;

    // This little C++ trick allows us to use BOTH BusIn and DigitalIn
    DigitalIn& buttonA = buttons[0];    //ButtonA is synonamous with buttons[0]
    DigitalIn& buttonB = buttons[1];
    DigitalIn& buttonC = buttons[2];
    DigitalIn& buttonD = buttons[3];

    //Configure switches
    buttonC.mode(PullDown);
    buttonD.mode(PullDown);

    //Turn ON the 7-segment display
    disp.enable(true);

    microseconds timeLED = 0ms;

    Btn btnA(buttonA, 0);
    Btn btnB(buttonB, 0);
    
    //Start
    tmrLED.start();
    disp = count;

    while (true) {

        // ************************************
        // Read all inputs and store the result
        // ************************************
        timeLED = tmrLED.elapsed_time();

        // ***************************
        // UPDATE "STATE" for button A
        // ***************************
        if (btnA.pressed()) {
            if (count < 99) {
                disp = ++count;
            }
        }
        
        // ***************************
        // UPDATE "STATE" for button B
        // ***************************
        if (btnB.pressed()) {
            if (count > 0) {
                disp = --count;
            }
        }

        // ********************************
        // UPDATE "STATE" for LED and Timer
        // ********************************
        
        // The timer value is the "state" for the flashing LED

        // METHOD 1 (Simple if)
        // if (timeLED >= 250ms) {
        //     greenLED = !greenLED;
        //     tmrLED.reset();
        // }


        // METHOD 2 (switch-case style)
        // Convert the time (type microseconds) to ms (type long long)
        long long time_ms = duration_cast<milliseconds>(timeLED).count();       //Ugh..
        switch (time_ms) {
            case 0 ... 249:     // Cool huh?
            break;

            default:
            greenLED = !greenLED;
            tmrLED.reset();
        }
  
    }
}
