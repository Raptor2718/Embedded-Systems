#include "Kernel.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "common_types.hpp"
#include "http.hpp"
#include "uop_msb.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>

DigitalOut greenLED(LED1);
// a class for checking dispmail - and displaying 
// alarm, light bar, serial print 
class Display {
    private:
        mail_t mail;
        // the mail shared by the threads dt and txThread
        mail_t txmail;
        Mutex txmail_mutex;
        Mail<mail_t, 16> &mailbox;
        Thread dt;
        fields setField = l;
        fieldlimits setLimits = {
            {2000, 60000}, // LDR
            {10, 28},      // Temperature (°C)
            {990, 1017}   // Pressure (hPa)
        };  

        Mail<http_struct, 1>txmailbox;
        Mail<http_struct, 1>rxmailbox;
        Thread txThread;
        Thread rxThread;

    void alarm() {
        fields cf;
        char cf_str[64];
        for (uint8_t i = 0; i < 3; i++) {    
            cf = static_cast<fields>(i);
            switch(cf) {
                case l: strcpy(cf_str, "light"); break;
                case t: strcpy(cf_str, "temperature"); break;
                case p: strcpy(cf_str, "preassure"); break;
            };

            if ( setLimits.get_limit(cf).above_upper( mail.get_float(cf) ) ) {
                printf("[display] Warning %s value %f above %f \n", cf_str, mail.get_float(cf), setLimits.get_limit(cf).upper);
                play_alarm("G", 500ms);

            } else if ( setLimits.get_limit(cf).below_lower(mail.get_float(cf)) ) {
                printf("[display] Warning %s value %f below %f \n", cf_str, mail.get_float(cf), setLimits.get_limit(cf).lower);
                play_alarm("C", 500ms);
            } else {};
        }
        
    }

    void play_alarm(const char* note, Kernel::Clock::duration_u32 halfT) {
        buzz.playTone(note, HIGHER_OCTAVE);
        ThisThread::sleep_for(halfT);
        buzz.playTone(note, LOWER_OCTAVE);
        ThisThread::sleep_for(halfT);
        buzz.rest();
    }

    void serial_print() {
        printf("[display] Light = %d ----- Preassure = %04.2f hPa ----- Temperature = %2.2f degC\n", mail.ldr, mail.pressure, mail.temp);
        const char *fs = field_to_string(setField);
        printf("[display] Led bar is showing %s\n", fs);
    }

    void led_bar() {
        uint8_t val[3];
        uint32_t vals;
        uint32_t vals_decoded;
        uint32_t numBits = 24;
        
        vals = setLimits.get_limit(setField).scale_and_quantize(mail.get_float(setField), 0, 24);
        vals_decoded = (1u << vals) - 1;

        for (int b = 0; b < 3; b++) {
            val[b] = (vals_decoded >> (8 * b)) & 0xFF;
        }

        latchedLEDs.write_strip(val[0], LEDGROUP::RED);
        latchedLEDs.write_strip(val[1], LEDGROUP::GREEN);
        latchedLEDs.write_strip(val[2], LEDGROUP::BLUE);
    }

    void get_data() {
        mail_t* payload;
        greenLED = !greenLED;
        payload = mailbox.try_get_for(10s); // blocking

        if (payload) {
            mail = *payload;
            mailbox.free(payload);
        }

        txmail_mutex.lock();
        txmail = mail;
        txmail_mutex.unlock();
    }

    void display_tasks() {

        http_struct v;
        v.tx_len = 3;
        v.rx_len = 1;
        v.tx[0] = {"l", "0"};
        v.tx[1] = {"t", "0"};
        v.tx[2] = {"p", "0"};
        v.rx[0] = {"param", "l"};
        
        Http<http_struct> http(v, txmailbox, rxmailbox, txThread);  

        txThread.start(callback(this, &Display::tx_task));
        rxThread.start(callback(this, &Display::rx_task));

        while(true) { 
            
            get_data(); // blocking till you get display mail 
            alarm();
            serial_print();
            led_bar();
        }
    }

    void enable() {
        latchedLEDs.enable(BLUE);
        latchedLEDs.enable(RED);
        latchedLEDs.enable(GREEN);
        greenLED = 0;
    }

    void self_test() {
        play_alarm("G", 500ms);
        int err;
        
        err = latchedLEDs.write_strip(255, BLUE);
        printf("[display] self test complete\n");
    }

    void tx_task() {
        while (true) {
            // TX
            printf("[tx] waiting for 1...\n");
            ThisThread::flags_wait_any(1);
            printf("[tx] contining...\n");
            ThisThread::flags_clear(1);

            char lbuff[32];
            char tbuff[32];
            char pbuff[32];

            txmail_mutex.lock();
            sprintf(lbuff, "%d", txmail.ldr);
            sprintf(tbuff, "%4.2f", txmail.get_float(t));
            sprintf(pbuff, "%2.2f", txmail.get_float(p));
            txmail_mutex.unlock();

            http_struct *payload = txmailbox.try_alloc_for(60s);
            if (payload == NULL) {
                ThisThread::sleep_for(100ms);
                continue;
            }
            //printf("asked for temp\n");
            payload->tx[0] = {"l", lbuff};
            payload->tx[1] = {"t", tbuff};
            payload->tx[2] = {"p", pbuff};

            osStatus stat = txmailbox.put(payload);
            
            if (stat != osOK) {
                txmailbox.free(payload);
                ThisThread::sleep_for(100ms);
                continue;
            }
            
        }
    }

    void rx_task() {
        while (true) {
            // RX
            http_struct *rxpayload;
            rxpayload = rxmailbox.try_get_for(10s); //blocking 
            string param;
            if (rxpayload != NULL) {
                param = rxpayload->rx[0].val;
                rxmailbox.free(rxpayload);
                //printf("gv = %s, rv = %s\n", gv.c_str(), rv.c_str());
                if (param == "l") setField = l; 
                else if (param == "t") setField = t;
                else if (param == "p") setField = p;
            }
        }
    }

    public:
        Display(Mail<mail_t, 16> &mb) : dt(osPriorityBelowNormal) , mailbox(mb) {
            enable();
            self_test();
            dt.start(callback(this, &Display::display_tasks));
        }

        ~Display() {
            printf("[display] Goodbye!\n");
        }
};