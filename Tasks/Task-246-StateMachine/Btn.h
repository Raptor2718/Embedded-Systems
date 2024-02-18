
#include "uop_msb.h"
#include <chrono>
#include <ratio>

class Btn {
    
    Timer tmr;
    microseconds tm;
    DigitalIn& btn;
    typedef enum {WAITING_FOR_PRESS, DEBOUNCE_1, WAITING_FOR_RELEASE, DEBOUNCE_2} state;
    state btn_state;
    state active_state;
    bool edge;
    int btn_val;

public:
    Btn(DigitalIn& btn_pin, bool edge);
    bool pressed();
};