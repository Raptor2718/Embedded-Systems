#include "uop_msb.h"
#include <algorithm>
#include <chrono>
using namespace uop_msb;

//threads 
Thread tsamp, tavg;

//Hardware
Ticker tick;
AnalogIn ldr(AN_LDR_PIN);

// function prototypes
void sample();
void print_moving_avg();
void tickerISR();

// global vars
int samples[2000];
int sample_pos = 0;
long long running_sum;

Mutex sumlock;

int main(void)
{
/*
1. Create an ISR - called by a Ticker every 1ms. This ISR simply has the job of signalling a waiting thread to perform an ADC conversion
2. Create a thread that waits for a signal from the ISR. This thread should ideally have the highest priority
3. Each time it is unblocked by the signal, it should read the ADC (Use `AnalogIn`) for the LDR and add it to a running sum.
4. Every 1s, it should print out the average of the past 1000 samples to the terminal. 
*/
    tsamp.start(sample);
    tavg.start(print_moving_avg);
    tick.attach(&tickerISR, 1ms);

    while (true) {
        ThisThread::sleep_for(1000ms);
    }
    
}   

void tickerISR() {
    tsamp.flags_set(1);
}

void sample() {
    while(true) {
        ThisThread::flags_wait_any(1);

        samples[sample_pos] = ldr.read_u16();
        sample_pos++;

        if (sample_pos == 999) {
            tavg.flags_set(1);
            ThisThread::flags_wait_any(2);
        } else if (sample_pos == 1999) {
            tavg.flags_set(1);
            ThisThread::flags_wait_any(2);
            sample_pos = 0;
        }
    }
}

void print_moving_avg() {
    while (true) {
        ThisThread::flags_wait_any(1);
        int old_pos = sample_pos;
        tsamp.flags_set(2);

        running_sum = 0;
        for (int i = old_pos; i >= old_pos-1000; i--) {
            running_sum += samples[i];
        }

        float average = running_sum/1000.0;
        printf("light level = %.2f\n", average);
    }
}