#include "mbed.h"

// Create a DigitalOut “object” called greenLED. Pass constant LED1 as a “parameter”
DigitalOut greenLED(LED1);

//These are "commented out" and so are not part of the program. You can uncomment them by removing the // characters
//Your task is to make a sequence alternating between Green+Red and just Blue 
DigitalOut blueLED(LED2);
DigitalOut redLED(LED3);

//The main function - all executable C / C++ applications have a main function. This is our entry point in the software
int main() 
{
    // ALL the repeating code is contained in a  “while loop”
    while(true) 
    { 
        //The code between the { curly braces } is the code that is repeated forever

        // Turn onboard LED ON  
        blueLED = 0;
        greenLED = 1; 
        redLED =  1;

<<<<<<< HEAD
        // Wait 0.2 second (0.2 million microseconds)
        wait_us(200000); 
=======
        // Wait 0.2 second (1 million microseconds)
        wait_us(100000); 
>>>>>>> 073e54812c27590ac6ecf7e9e017cc417de0d5d6

        // Turn LED OFF
        greenLED = 0;
        redLED = 0;
        blueLED = 1;

        // Wait 0.2 second
<<<<<<< HEAD
        wait_us(200000); 
=======
        wait_us(100000); 
>>>>>>> 073e54812c27590ac6ecf7e9e017cc417de0d5d6
    }
}