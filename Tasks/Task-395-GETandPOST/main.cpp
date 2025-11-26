#include "ThisThread.h"
#include "Thread.h"
#include "lorawan_types.h"
#include "uop_msb.h"
#include <cstdio>
#include <iostream>
#include <string>
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "http.hpp"

using namespace std;

// ======== HARDWARE ========


DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut redLED(LED3);

struct dict_obj {
    string key;
    string val;
};
struct http_struct {
    dict_obj tx[1];
    dict_obj rx[3];
    int tx_len = 1;
    int rx_len = 3;
};

Mail<http_struct, 1>txmailbox;
Mail<http_struct, 1>rxmailbox;
Thread txThread;
Thread rxThread;
void tx_task();
void rx_task();

int main() {
    printf("\r\nBasic HTTP server example\r\n");
    
    http_struct v;
    v.tx_len = 1;
    v.rx_len = 3;
    v.tx[0] = {"temp", "0"};
    v.rx[0] = {"red", "0"};
    v.rx[1] = {"green", "0"};
    v.rx[2] = {"blue", "0"};
    
    Http<http_struct> http(v, txmailbox, rxmailbox, txThread);  

    txThread.start(callback(tx_task));
    rxThread.start(callback(rx_task));
    // main can idle
    while (true) ThisThread::sleep_for(1s);
}



void tx_task() {
    while (true) {
        // TX
        printf("[tx] waiting for 1...\n");
        ThisThread::flags_wait_any(1);
        printf("[tx] contining...\n");
        ThisThread::flags_clear(1);
        
        float t = env.getTemperature();
        char buff[6];
        sprintf(buff, "%5.3f", t);
        
        http_struct *payload = txmailbox.try_alloc_for(60s);
        if (payload == NULL) {
            ThisThread::sleep_for(100ms);
            continue;
        }
        //printf("asked for temp\n");
        payload->tx[0] = {"temp", buff};
        
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
        string rv;
        string gv;
        string bv;
        if (rxpayload != NULL) {
            rv = rxpayload->rx[0].val;
            gv = rxpayload->rx[1].val;
            bv = rxpayload->rx[2].val;
            rxmailbox.free(rxpayload);
            //printf("gv = %s, rv = %s\n", gv.c_str(), rv.c_str());
            if (rv == "1") redLED = 1; else redLED = 0;
            if (gv == "1") greenLED = 1; else greenLED = 0;
            blueLED = (bv == "1") ? 1:0;
        }
    }
}