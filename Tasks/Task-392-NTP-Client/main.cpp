/*#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
 */
#include "uop_msb.h"
using namespace uop_msb;
#include <iostream>
#include "NTPTimeSet.hpp"
using namespace std;

//const char *const myHTTP = HTTP_RESPONSE; //This if you wish to set above Compiler defines into Flash Silicon


LCD_16X2_DISPLAY disp;
DigitalOut lcdBacklight(LCD_BKL_PIN,1);
InterruptIn blueButton(USER_BUTTON);

int main()
{
    // Create network interface (Ethernet in our case)    
    
    time_t timestamp;
    NTPTimeSet timeSetter([](const char* msg){
        printf("[NTP] %s", msg);
    });

    int status = timeSetter.setTime();
    if(status != 0){
        printf("Failed to set time\n");
    }

    while (true)
    {
        //Spin waiting for button press
        while (blueButton == 0);
        
        //Immediately read the time (seconds since 00:00 on the 1/1/1070)
        timestamp = time(NULL);

        //Display the current date and time
        disp.cls(); disp.locate(0, 0);
        disp.printf("%s\n", ctime(&timestamp));
        
        //Debounce
        ThisThread::sleep_for(250ms);
        
        //Spin waiting for button release
        while (blueButton == 1);

        //Debounce
        ThisThread::sleep_for(250ms);
    }


}
 
