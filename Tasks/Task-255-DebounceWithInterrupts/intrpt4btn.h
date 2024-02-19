#include "uop_msb.h"
#include <chrono>
using namespace uop_msb;

class Intrpt4btn {
    InterruptIn btn;            //to detect rising and falling edgedes
    Timeout tm;                 //to interrupt when debounce has passed
    bool edge;                  //button edge at which to call func
    Callback<void()> func;
    //state machine - functions to attach to interrupts and tickers
    void rs();
    void d1();
    void fl();
    void d2();

    public:
    Intrpt4btn(PinName pin, bool edge, Callback<void()> func);

};