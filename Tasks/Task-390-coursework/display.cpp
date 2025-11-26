#include "display.hpp"
#include "cmsis_os2.h"
#include "http.hpp"
#include "uop_msb.h"
// this is toggled when the display thread recieves new mail
DigitalOut greenLED(LED1);
Buzzer buzz;
void Display::alarm() {
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

            aq.call(callback(this, &Display::play_alarm), "G", 500ms);

        } else if ( setLimits.get_limit(cf).below_lower(mail.get_float(cf)) ) {
            printf("[display] Warning %s value %f below %f \n", cf_str, mail.get_float(cf), setLimits.get_limit(cf).lower);
            play_alarm("C", 500ms);
        } else {};
    }
    
}

void Display::play_alarm(const char* note, Kernel::Clock::duration_u32 halfT) {
    buzz.playTone(note, HIGHER_OCTAVE);
    ThisThread::sleep_for(halfT);
    buzz.rest();
    buzz.playTone(note, LOWER_OCTAVE);
    ThisThread::sleep_for(halfT);
    buzz.rest();
}

void Display::serial_print() {
    printf("[display] Light = %d ----- Preassure = %04.2f hPa ----- Temperature = %2.2f degC\n", mail.ldr, mail.pressure, mail.temp);
    setField_mutex.lock();
    const char *fs = field_to_string(setField);
    setField_mutex.unlock();
    printf("[display] Led bar is showing %s\n", fs);
}

void Display::led_bar() {
    uint8_t val[3];
    uint32_t vals;
    uint32_t vals_decoded;
    uint32_t numBits = 24;
    
    setField_mutex.lock();
    vals = setLimits.get_limit(setField).scale_and_quantize(mail.get_float(setField), 0, 24);
    setField_mutex.unlock();
    vals_decoded = (1u << vals) - 1;

    for (int b = 0; b < 3; b++) {
        val[b] = (vals_decoded >> (8 * b)) & 0xFF;
    }

    latchedLEDs.write_strip(val[0], LEDGROUP::RED);
    latchedLEDs.write_strip(val[1], LEDGROUP::GREEN);
    latchedLEDs.write_strip(val[2], LEDGROUP::BLUE);
}

void Display::get_data() {
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

void Display::display_tasks() {

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

    while(dtrunning) { 
        
        get_data(); // blocking till you get display mail 
        alarm();
        serial_print();
        led_bar();
    }
}

void Display::enable() {
    latchedLEDs.enable(BLUE);
    latchedLEDs.enable(RED);
    latchedLEDs.enable(GREEN);
    greenLED = 0;
}

void Display::self_test() {
    play_alarm("G", 500ms);
    int err;
    
    err = latchedLEDs.write_strip(255, BLUE);
    printf("[display] self test complete\n");
}

const char* Display::field_to_string(fields f) {
    switch(f) {
        case l: return "light"; break;
        case t: return "temperature"; break;
        case p: return "preassure"; break;
    };
}

void Display::tx_task() {
    while (txrunning) {
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

void Display::rx_task() {
    while (rxrunning) {
        // RX
        http_struct *rxpayload;
        rxpayload = rxmailbox.try_get_for(10s); //blocking 
        string param;
        if (rxpayload != NULL) {
            param = rxpayload->rx[0].val;
            rxmailbox.free(rxpayload);
            //printf("gv = %s, rv = %s\n", gv.c_str(), rv.c_str());
            setField_mutex.lock();
            if (param == "l") setField = l; 
            else if (param == "t") setField = t;
            else if (param == "p") setField = p;
            setField_mutex.unlock();
        }
    }
}


Display::Display(Mail<mail_t, 16> &mb) : dt(osPriorityBelowNormal) , mailbox(mb), dtrunning(true), txrunning(true), rxrunning(true), alarmt(osPriorityAboveNormal) {
    enable();
    self_test();
    dt.start(callback(this, &Display::display_tasks));
    alarmt.start(callback(&aq, &EventQueue::dispatch_forever));
}

Display::~Display() {
    dtrunning = false;
    txrunning = false;
    rxrunning = false;
    dt.join();
    txThread.join();
    rxThread.join();
    printf("[display] Goodbye!\n");
}
