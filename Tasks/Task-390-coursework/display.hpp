#include "EventQueue.h"
#include "Kernel.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "uop_msb.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <atomic>

// my headers
#include "common_types.hpp"


// a class for checking dispmail - and displaying 
// alarm, light bar, serial print 
class Display {
    private:
        mail_t mail;
        // the mail shared by the threads dt and txThread
        mail_t txmail;
        Mutex txmail_mutex;

        Mail<mail_t, 16> &mailbox;
        Mail<http_struct, 1>txmailbox;
        Mail<http_struct, 1>rxmailbox;

        Thread dt;
        Thread txThread;
        Thread rxThread;
        Thread alarmt;
        EventQueue aq;
        volatile std::atomic<bool> dtrunning;
        volatile std::atomic<bool> txrunning;
        volatile std::atomic<bool> rxrunning;

        fields setField = l;
        Mutex setField_mutex;
        fieldlimits setLimits = {
            {2000, 60000}, // LDR
            {10, 28},      // Temperature (°C)
            {990, 1017}   // Pressure (hPa)
        };  

        
        // funcions diretly associated with threads
        void display_tasks();
        void tx_task();
        void rx_task();

        // helper function
        void alarm();
        void play_alarm(const char* note, Kernel::Clock::duration_u32 halfT);
        void serial_print();
        void led_bar();
        void get_data();
        void enable();
        void self_test();
        const char* field_to_string(fields f);
        

    public:
        Display(Mail<mail_t, 16> &mb);
        ~Display();
};