#include "mbed.h"
#include "uop_msb.h"

uop_msb::LCD_16X2_DISPLAY lcd;
DigitalOut lcd_backlight(LCD_BKL_PIN);

int main()
{
    lcd_backlight=1;
    const int custom_char_number = 0;  // range 0-7 

    // Display is 5x8 pixels, 3 most significant bits of each byte should b 0s
    const char character[8] =
    {0b00000000,
     0b00001010,
     0b00001010,
     0b00000000,
     0b00010001,
     0b00001010,
     0b00000100,
     0b00000000};
    
    //Set the LCD to point to the address of Character Graphics RAM                     
    lcd.set_CGRAM_Address(custom_char_number*8);    //8 bytes per character.     ???so the *8 is not really nessasary right?  
    //??? so does it go to the 0th address every time the code is run and then allocate the first 8 bytes?   
    //Write 8 bits of data at a time into the LCD's CGRAM (the address increments automagically)
    for (int i=0;i<8;i++){
        lcd.write(uop_msb::LCD_16X2_DISPLAY::DATA, character[i]);
    }
    // Clear the lcd
    lcd.cls();
    // Place the custom character at row 0 col 0
    lcd.character(0,0,custom_char_number); 
    
    // There are also a number of symbols already built into the LCD
    // Check page 13 of the datasheet
    // https://static.rapidonline.com/pdf/57-2224.pdf
    // The data sheet lists them by upper and lower nibble binary values rather than just a decimal
    // To access them without converting the binary to decimal (or hex) you can just state the values as binary
    //  0bUUUULLLL

    // These can both be done using lcd.printf too
    lcd.locate(1,0);
    lcd.printf("%c4=2 temp:35%c %c",0b11101000,0b11011111,custom_char_number);

}

