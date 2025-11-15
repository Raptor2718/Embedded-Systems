#include "uop_msb.h"
#include "PushSwitch.hpp"

#include <chrono>
#include <cstdint>
#include <cstdio>
using namespace uop_msb;
 
Semaphore sem1;
Semaphore sem2(10);
Mutex countLock;
uint16_t counter = 0;
Thread t1;
Thread t2;

int vars1 = 0;
int vars2 = 10;
Mutex v1;
Mutex v2;

void climb()
{
    PushSwitch sw(BTN1_PIN);
    DigitalOut led(LED1);

    while (true) {
        sw.waitForPress(); //Blocking

        led = 1;
        // sem2.acquire(); //Decrement
        v2.lock();
        if (vars2 != 0) {
            vars2--;
            v2.unlock();

            countLock.lock();
            counter++;
            printf("%u u: %u\n", (unsigned int)time(NULL), counter);
            countLock.unlock();
            //sem1.release(); //Increment
            v1.lock();
            vars1++;
            v1.unlock();
            led = 0;
        } else {
            v2.unlock();
        }
        
        //Debounce
        ThisThread::sleep_for(50ms);
        sw.waitForRelease();
        ThisThread::sleep_for(50ms);
    }
}

void descend()
{
    PushSwitch sw(BTN2_PIN);
    DigitalOut led(LED2);

    while (true) {
        sw.waitForPress(); //Blocking

        led = 1;

        //sem1.acquire(); //Decrement
        v1.lock();
        if (vars1 != 0) {
            vars1--;
            v1.unlock();
            countLock.lock();
            counter--;
            printf("%u d: %u\n", (unsigned int)time(NULL), counter);
            countLock.unlock();
            //sem2.release(); //Increment
            v2.lock();
            vars2++;
            v2.unlock();
            led = 0;
        } else {
            v1.unlock();
        }
        

        //Debounce
        ThisThread::sleep_for(50ms);
        sw.waitForRelease();
        ThisThread::sleep_for(50ms);        
    }
}

int main(void)
{
    set_time(0);
    DigitalOut led(LED3);
    t1.start(climb);
    t2.start(descend);

    while (true) {
        ThisThread::sleep_for(1000ms);
        led = !led;
    }
    
}   