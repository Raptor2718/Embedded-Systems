#include "uop_msb.h"
using namespace uop_msb;

#define WAIT_TIME_MS 500 

DigitalOut red(TRAF_RED1_PIN,1);         //Note the initial state
DigitalOut amber(TRAF_YEL1_PIN,0);
DigitalOut green(TRAF_GRN1_PIN,0);

//Object for controlling the LCD
LCD_16X2_DISPLAY lcd;

int main()
{
    red = 1;
    lcd.puts("RED");
    wait_us(1000000);

    amber = 1;
    lcd.cls();
    lcd.puts("Amber");
    wait_us(1000000);

    green = 1;
    lcd.cls();
    lcd.puts("Green");    
    wait_us(1000000);

    lcd.cls();
    lcd.puts("TASK-104");

    while (true)
    {
        red = !red;
        lcd.puts("Red "); 

        amber = !amber;
        lcd.puts("Amber "); 

        green = !green;
        lcd.puts("Green"); 
        

        
        wait_us(WAIT_TIME_MS * 1000);



        for (int i = 0; i < 8; i++)
        {
            amber = !amber;
            wait_us(500000);
        }


    }
}
