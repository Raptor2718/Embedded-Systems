//#include "mbed.h"
#include "AnalogIn.h"
#include "PinNames.h"
#include "PwmOut.h"
#include "uop_msb.h"
#include <algorithm>
#include <cstdio>
using namespace uop_msb;

/*  ASSUMPTIONS:
- 20 degrees is 20% the ADC range 
- 40 degrees is 40% the ADC range
*/
AnalogIn temp_sensor(PA_0);
PwmOut led(LED1);

TimerCompat sampling_timer;

unsigned short temp;
float led_val;

int main()
{
    sampling_timer.start();
    led.period(0.001f);

    while (true) {

        if (sampling_timer.read_ms() > 100)
        {
            temp = temp_sensor.read_u16();
            
            // Note assumptions at the top of the file - would need to check for the sensor used.
            // 2^16 * 0.2 ~= 13107             
            led_val = float(temp - 13107)/13107;
            
            if (led_val > 1)        led_val = 1;
            else if (led_val < 0)   led_val = 0;

            led.write(led_val);
        }
    }
}

