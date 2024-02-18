#include "Btn.h"
#include "uop_msb.h"
#include <chrono>
#include <ratio>

Btn::Btn(DigitalIn& btn_pin, bool edge) : btn(btn_pin), edge(edge){
    btn_state = WAITING_FOR_PRESS;
    active_state = edge ? WAITING_FOR_RELEASE: WAITING_FOR_PRESS;
}

bool Btn::pressed() {
    //reading inputs and storing result
    tm = tmr.elapsed_time();
    btn_val = btn;

    //conditionals
    switch (Btn::btn_state) {
            case WAITING_FOR_PRESS:
                if (btn_val == 1) {
                    btn_state = DEBOUNCE_1;
                    tmr.reset();
                    tmr.start();
                    if (active_state == WAITING_FOR_PRESS) return 1;
                }
                return 0;
            break;

            case DEBOUNCE_1:
                if (tm >= 50ms) {
                    btn_state = WAITING_FOR_RELEASE;
                    tmr.stop();
                }
                return 0;
            break;

            case WAITING_FOR_RELEASE:
                if (btn_val == 0) {
                    btn_state = DEBOUNCE_2;
                    tmr.reset();
                    tmr.start();
                    if (active_state == WAITING_FOR_RELEASE) return 1;        
                }
                return 0;
            break;

            case DEBOUNCE_2:
                if (tm >= 50ms) {
                    btn_state = WAITING_FOR_PRESS;
                    tmr.stop();
                }
                return 0;
            break;            
        }
}