#include "mbed.h"
#include "uop_msb.h"

uop_msb::LCD_16X2_DISPLAY lcd;
DigitalOut lcd_backlight(LCD_BKL_PIN);

int main()
{
    lcd_backlight=1;
    
    const int smiley_num = 0;  // range 0-7 

    const int trex_num = 9; //Thought it might get saved to the 9th address. 

    const int cactus_num = 0b00100000; 
    //As my previous approach didnt work, assumed that the empty boxes in page 13 would hold smthn. 
        // https://static.rapidonline.com/pdf/57-2224.pdf


    // Display is 5x8 pixels, 3 most significant bits of each byte should b 0s

    const char trex[8] =
    {0b00001111,
     0b00001011,
     0b00001111,
     0b00010110,
     0b00011111,
     0b00001101,
     0b00001000,
     0b00001100};

    const char smiley[8] =
    {0b00000000,
     0b00001010,
     0b00001010,
     0b00000000,
     0b00010001,
     0b00001010,
     0b00000100,
     0b00000000};

     const char cactus[8] = 
     {
        0b00000100,
        0b00000101,
        0b00010101,
        0b00011111,
        0b00011111,
        0b00001110,
        0b00001110,
        0b00001110
     };
    
    lcd.set_CGRAM_Address(smiley_num);   

    for (int i=0;i<8;i++){
        lcd.write(uop_msb::LCD_16X2_DISPLAY::DATA, smiley[i]);
    }

    lcd.set_CGRAM_Address(trex_num);    

    for (int i=0;i<8;i++){
        lcd.write(uop_msb::LCD_16X2_DISPLAY::DATA, trex[i]);
    }
    
    lcd.set_CGRAM_Address(cactus_num);    

    for (int i=0;i<8;i++){
        lcd.write(uop_msb::LCD_16X2_DISPLAY::DATA, cactus[i]);
    }

    lcd.cls();

    lcd.character(0,0,smiley_num); 
    lcd.character(0,2, trex_num);
    lcd.character(0,4, cactus_num);


}

