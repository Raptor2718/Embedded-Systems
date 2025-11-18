
#include "mbed.h"
#include "SPL06-001.h"
#include "common_types.hpp"

using namespace std::chrono;

class Sensor {
    private:
        Thread st;
        EventQueue sq;
        SPL06_001_SPI dev;
        AnalogIn ldr;
        milliseconds sample_period = 500ms;
        Mail<mail_t, 16>&mailbox;
    
    
    void sample() {
        mail_t* mail = mailbox.try_alloc();
        if (mail == NULL) {
            return;
        }
        mail->ldr = ldr.read_u16();
        mail->pressure = dev.getPressure();
        mail->temp = dev.getTemperature();

        osStatus stat = mailbox.put(mail);

        if (stat != osOK) {
            mailbox.free(mail);
            return;
        }
    }

    public:
        Sensor(Mail<mail_t, 16> &mb,
           milliseconds sp,
           PinName mosi, PinName miso, PinName sclk, PinName cs,
           PinName ldr_pin)
        : st(osPriorityAboveNormal),
          sq(),
          dev(mosi, miso, sclk, cs),
          ldr(ldr_pin),
          sample_period(sp),
          mailbox(mb)
        {
            dev.initialize();
            st.start(callback(&sq, &EventQueue::dispatch_forever));

            sq.call_every(sp, callback(this, &Sensor::sample));
        }
};