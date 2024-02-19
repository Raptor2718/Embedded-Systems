#include "uop_msb.h"
#include <chrono>
#include <intrpt4btn.h>

using namespace uop_msb;

DigitalOut redLED(TRAF_RED1_PIN);       //Red Traffic 1
DigitalOut yellowLED(TRAF_YEL1_PIN);    //Yellow Traffic 1
DigitalOut greenLED(TRAF_GRN1_PIN);     //Green Traffic 1

#define WAIT_TIME_MS 500 
DigitalOut led1(LED1);

//Serial comms
static UnbufferedSerial serial_port(USBTX, USBRX,9600);
char dot = '.';
char _A = 'A';
char _B = 'B';
//Interrupt Service Routines
//Timer ISR
void flashy() {
    led1 = !led1;
}

void A_flashy() {
    redLED = !redLED;
    serial_port.write(&_B,1);
}
void B_flashy() {
    yellowLED = !yellowLED;
    serial_port.write(&_A,1);
}

//objects that run the passed interrupt at given adge of given pin
Intrpt4btn btnA(BTN1_PIN, 0, &A_flashy);
Intrpt4btn btnB(BTN2_PIN, 1, &B_flashy);

int main()
{
    //Timer interrupt
    Ticker flash;
    flash.attach(flashy, 3100ms);

    while (true)
    {
        sleep();

        //Echo a dot each time one or more interrupts occur
        serial_port.write(&dot,1);
    }
}
