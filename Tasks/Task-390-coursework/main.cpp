#include "CircularBuffer.h"
#include "cmsis_os2.h"
#include "mbed.h"
#include "common_types.hpp"
#include "display.hpp"
#include "sensor.hpp"
#include "uop_msb.h"
#include <cstdio>
#include <string>
#include <vector>
#include <format>
#include <iostream>
#include "EthernetInterface.h"
#include "TCPSocket.h"
//SPL06_001_SPI sensor(); // mosi, miso, sclk, cs

DigitalOut blueLED(LED2);
DigitalOut redLED(LED3);

Mail<mail_t, 16>mailbox; 
Mail<mail_t, 16>dispbox;
CircularBuffer<mail_t, 200>mailbuf;
Mutex buf_mutex;

Thread buft(osPriorityNormal);

SDCard sd_card(PB_5, PB_4, PB_3, PF_3, PF_4);

void update_buffer();
void data_log();

int main()
{
    
    redLED = 0;
    Sensor sensor(mailbox, 500ms, PB_5, PB_4, PB_3, PB_2, AN_LDR_PIN);
    Display disp(dispbox);

    buft.start(callback(update_buffer));
    
    while(true) {
        ThisThread::sleep_for(60000ms);
        data_log();
    }
    
}

void update_buffer() {
    while(true) {
        mail_t* payload;
        redLED = !redLED;
        payload = mailbox.try_get_for(10s); // blocking

        if (payload) {
            mail_t mail(payload->ldr, payload->temp, payload->pressure);
            mailbox.free(payload);

            // update buffer
            buf_mutex.lock();
            if (!mailbuf.full()) {
                mailbuf.push(mail);
            }
            buf_mutex.unlock();

            mail_t *dispmail = dispbox.try_alloc();
            if (dispmail == NULL) {
                return;
            }
            dispmail->ldr = mail.ldr;
            dispmail->pressure = mail.pressure;
            dispmail->temp = mail.temp;
            osStatus stat = dispbox.put(dispmail);

            if (stat != osOK) {
                dispbox.free(dispmail);
                return;
            }
        }
    }
}

void data_log() {
    vector<mail_t> dump;
    mail_t item;
    static bool init_write = true;
    
    buf_mutex.lock();
    while (!mailbuf.empty()) {
        mailbuf.pop(item);
        dump.push_back(item);
    }
    buf_mutex.unlock();

    const char *filename = "sensor.log";
    if (init_write) {
        init_write = false;  
        const char *initlog = "light level,temperature (degC),preassure (hPa)\n";
        sd.write_file((char*)filename, (char*)initlog, false, true);
    }
    while (!dump.empty()) {
        item = dump.back();
        dump.pop_back();
        char logstr[64]; 
        sprintf(logstr, "%u,%.2f,%.2f\n", item.ldr, item.temp, item.pressure);
        sd.write_file((char*)filename, (char*)logstr, true, true);
    }

}

