#include "mbed.h"
#include "uop_msb.h"
#include <iostream>
#include "PressAndRelease.hpp"

using namespace uop_msb;
using namespace std;

//Globals
DigitalOut led1(LED1);
DigitalOut led2(LED2);

//Event queue for main
EventQueue mainQueue;
EventQueue printQ;

Thread printThread;
Thread rt_thread;

//Flash a given LED - parameter passed by reference
void flashLed(DigitalOut& led, bool val) {
    led = val;
}
 
void onLed1() {
    // This is NOT on the main thread
    //flashLed(led1);                         
    //Dispatch printf on main thread
    mainQueue.call(flashLed, led1, 1);
    printQ.call(printf, "Button A pressed\n");    
}

void onLed2() {
    //flashLed(led2);     
    mainQueue.call(flashLed, led2, 1);
    printQ.call(printf, "Button B pressed\n");
}

void offLed1() {
    // This is NOT on the main thread
    //flashLed(led1);                         
    //Dispatch printf on main thread
    mainQueue.call(flashLed, led1, 0);
    printQ.call(printf, "Button A released\n");    
}

void offLed2() {
    //flashLed(led2);     
    mainQueue.call(flashLed, led2, 0);
    printQ.call(printf, "Button B released\n");
}

int main() {  
    PressAndRelease btnA(BTN1_PIN, &onLed1, &offLed1);
    PressAndRelease btnB(BTN2_PIN, &onLed2, &offLed2);
    //Start main queue - dispatch
    rt_thread.start(callback(&mainQueue, &EventQueue::dispatch_forever));
    printThread.start(callback(&printQ, &EventQueue::dispatch_forever));

    while(true) {
        ThisThread::sleep_for(1s);
    }
}









