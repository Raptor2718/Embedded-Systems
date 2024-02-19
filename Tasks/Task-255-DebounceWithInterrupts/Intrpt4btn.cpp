#include "uop_msb.h"
#include <chrono>
#include <intrpt4btn.h>

using namespace uop_msb;
//constructor
Intrpt4btn::Intrpt4btn(PinName pin, bool edge, Callback<void()> func) : btn(pin), edge(edge), func(func)  {
    btn.rise(callback(this, &Intrpt4btn::rs));
};

// ****************************
// State machine for button A *
// ****************************
//interrupt at rising edge
void Intrpt4btn::rs() {
    btn.rise(nullptr);
    tm.attach(callback(this, &Intrpt4btn::d1), 50ms);
    if (!edge) {
        Intrpt4btn::func();                         //call the function if active high
    }
}
//interrupt after first debounce
void Intrpt4btn::d1() {
    tm.detach();
    btn.fall(callback(this, &Intrpt4btn::fl));
}
//interrupt at falling edge
void Intrpt4btn::fl() {
    btn.fall(nullptr);
    tm.attach(callback(this, &Intrpt4btn::d2), 50ms);
    
    if (edge) {                                     
        Intrpt4btn::func();                             //call the function if active low
    }
}
//interrupt after second debounce
void Intrpt4btn::d2() {
    tm.detach();
    btn.rise(callback(this, &Intrpt4btn::rs));
}