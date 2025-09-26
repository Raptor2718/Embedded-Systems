#include "mbed.h"
#include "TrafficLight.h"
#include "stdio.h"

#define BTN1_PIN PG_0

TrafficLight lights;
DigitalIn sw(USER_BUTTON);
InterruptIn btnA(BTN1_PIN);

TrafficLight::LIGHT_STATE s;
UnbufferedSerial ser(USBTX,USBRX);


void stoplight();

int main()
{
    printf("What's the flash rate?\r\n");
    double num;
    char buf [32];
    while(!ser.read(buf, 1));
    printf("get data %c\r\n",buf[0]);


    scanf("%lf", &num);
    lights.setFlashSpeed(num);

    btnA.rise(&stoplight);
    while (true) {

        //Wait for switch press
        while (sw==0);

        //Update lights
        s = lights.nextState();

        //Debounce switch
        wait_us(300000);

        //Wait for switch release
        while (sw==1);

        //Switch debounce
        wait_us(300000);
        
    }
}

void stoplight() 
{
    lights.stop();
}
