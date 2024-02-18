#include "uop_msb.h"
#include <chrono>
using namespace uop_msb;

BusIn buttons(BTN1_PIN, BTN2_PIN, BTN3_PIN, BTN4_PIN);
// DigitalIn buttonA(BTN1_PIN);    //ButtonA is synonamous with buttons[0]
// DigitalIn buttonB(BTN2_PIN);
// DigitalIn buttonC(BTN3_PIN);
// DigitalIn buttonD(BTN4_PIN);

DigitalOut redLED(LED1);       //Red Traffic 1
DigitalOut yellowLED(LED2);    //Yellow Traffic 1
DigitalOut greenLED(LED3);     //Green Traffic 1

//Dual Digit 7-segment Display
LatchedLED disp(LatchedLED::SEVEN_SEG);

// Timer for the flashing light
Timer tmr;
Timer a_tmr;
Timer b_tmr;

enum states {waiting_press, debounce} a_state, b_state;
int main()
{
    volatile int count = 0;

    // This little C++ trick allows us to use BOTH BusIn and DigitalIn
    //references
    DigitalIn& buttonA = buttons[0];    //ButtonA is synonamous with buttons[0]
    DigitalIn& buttonB = buttons[1];
    DigitalIn& buttonC = buttons[2];
    DigitalIn& buttonD = buttons[3];

    // Configure switches
    buttonC.mode(PullDown);
    buttonD.mode(PullDown);

    // Turn ON the 7-segment display
    disp.enable(true);

    //Start timer
    tmr.start();
    a_tmr.start();
    b_tmr.start();

    // Internal state
    int btnA_prev = buttonA;
    int btnB_prev = buttonB;
    int btnA_curr;
    int btnB_curr;
    
    microseconds atm = a_tmr.elapsed_time();
    microseconds btm = b_tmr.elapsed_time();
    microseconds tm = tmr.elapsed_time();   //Part of the C++ chrono class
    long long flash_period;
    while (true) {

        // ************************************
        // Read all inputs and store the result
        // ************************************
        btnA_curr = buttonA;
        btnB_curr = buttonB;
        tm = tmr.elapsed_time();
        atm = a_tmr.elapsed_time();
        btm = b_tmr.elapsed_time();

        // ****************************
        // UPDATE IF THERE WAS A CHANGE
        // ****************************

        // Did button A change?
        switch (a_state) {
            case waiting_press: 
                if (btnA_curr != btnA_prev) {

                    //Was it a press?
                    if (btnA_curr == 1) {
                        //Button A was pressed!
                        if (count < 99) {
                            count+=1;
                        }
                    }

                    // The previous value is now set to the current
                    btnA_prev = btnA_curr;

                    //**************
                    //Update display
                    //**************
                    disp = count;    
                    a_state = debounce; 
                    a_tmr.reset();
                }
                break;
            case debounce: 
                a_state = (atm >= 50ms) ? waiting_press: debounce;
                break;
        }
                

        // Did button B change?
        switch (b_state) {
            case waiting_press: 
                if (btnB_curr != btnB_prev) {

                    //Was it a press?
                    if (btnB_curr == 1) {
                        //Button B was pressed!
                        if (count > 0) {
                            count-=1;
                        }
                    }
                    
                    // The previous value is now set to the current
                    btnB_prev = btnB_curr;

                    //**************
                    //Update display
                    //**************
                    disp = count;  
                    //update state 
                    b_state = debounce;
                    b_tmr.reset();  
                }          
                break;
            case debounce: 
                b_state = (btm >= 50ms) ? waiting_press: debounce;
                break;
        }
        

        // LED
        flash_period = static_cast<long long>(100000 + 900000*(count/99.0));
        if (tm >= microseconds(flash_period)) {
            greenLED = !greenLED;
            tmr.reset();
        }
        
        // Slow it down a bit (and debounce the switches)
        wait_us(50000);  
    }
}





