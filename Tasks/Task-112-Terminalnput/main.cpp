#include "uop_msb.h"
using namespace uop_msb;

Buzzer buzz;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);


int main()
{
    printf("\nTASK-112\n");

    //getchar
    char c1, c2;
    printf("\n\nPress a key\n");
    c1 = getchar();
    printf("You entered character %c which has the ASCII code %d\n", c1, c1);

    printf("\n\nPress another key\n");
    c2 = getchar();
    printf("You entered character %c which has the ASCII code %d\n", c2, c2);

    //scanf
    printf("\n\nTo set the delay (in ms), type in an int number and press return\n");
    int delay_ms=65;
    int parsed = scanf("%d", delay_ms);
    
    printf("You entered %d correct integer values. The value was %dms\n", parsed, delay_ms);

    // Stop
    while (parsed > 0) {
        wait_us(delay_ms * 1000);
        led1 = 1;
        led2 = 1;
        led3 = 1;
        wait_us(delay_ms * 1000);
        led1 = 0;
        led2 = 0;
        led3 = 0;        
    } 
//as long as you enter a valid int fscan will make parsed 1 and so the below code will never run. smart

    printf("Invalid - please restart and try again\n");
    while(1);    
}