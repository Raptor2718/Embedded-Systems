#include "mbed.h"
#include "string.h"
#include <iostream>
using namespace std;

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut redLED(LED3);

BusOut lights(LED1, LED2, LED3);


int main()
{
    while (true) {
        char cmdString[20];
        cout << "trype a cvs with colors" << endl;
        cin >> cmdString;
        const char delim[]=",";
        char *nextString;

        //Search for next instance of delim    
        nextString = strtok(cmdString, delim);

        while (nextString != NULL) 
        {
            printf("Found %s\n", nextString);
            printf("cmdString is now %s\n", cmdString);

            
            if (strcmp(nextString, "red") == 0)
            {
                redLED = 1;
            } else if (strcmp(nextString, "green") == 0)
            {
                greenLED = 1;
            } else if (strcmp(nextString, "blue") == 0)
            {
                blueLED = 1;
            } else if (strcmp(nextString, "wait") == 0)
            {
                lights = 0;
            }

            wait_us(1000000);

            nextString = strtok(NULL, delim);
        
        }

        lights = 0;


    }

}

