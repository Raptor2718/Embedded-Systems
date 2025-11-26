#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H
#include <string>
#include <cstdint>


enum fields: uint8_t {l = 0, p = 1, t = 2};

const char* field_to_string(fields f) {
    switch(f) {
        case l: return "light"; break;
        case t: return "temperature"; break;
        case p: return "preassure"; break;
    };
}

class limits {
    public:
        float lower;
        float upper;

        bool in_range(float x) const {
            return x >= lower && x <= upper;
        }

        float normalize(float x) const {
            if (x < lower) x = lower;
            if (x > upper) x = upper;
            return (x - lower) / (upper - lower);
        }

        uint32_t scale_and_quantize(float x, uint32_t o_min, uint32_t o_max) {
            if (x < lower) x = lower;
            if (x > upper) x = upper;

            float norm = (x - lower) / (upper - lower);
            float q = (float)o_min + norm * (float)(o_max - o_min);

            return static_cast<uint32_t>(q + 0.5f); 
        }


        bool above_upper(float x) {
            if (x > upper) {
                return true;
            } else return false;
        }
        bool below_lower(float x) {
            if (x < lower) {
                return true;
            } else return false;
        }

        limits() = default;

        limits(float lo, float up) : lower(lo), upper(up) {};
};


typedef struct fieldlimits {
    limits ldr;
    limits temp;
    limits pressure;

    limits& get_limit(fields f)
    {
        switch (f) {
            case l:     return ldr;
            case p:     return pressure;
            case t:     return temp;
        }
        // fallback required to silence warnings
        return ldr;
    }
} fieldlimits;


class mail_t {
    public:
        unsigned short ldr;
        float temp;
        float pressure;

        mail_t() = default;
        
        mail_t(unsigned short l, float t, float p) 
        : ldr(l), temp(t), pressure(p) {};

        mail_t(const mail_t &other)
        : ldr(other.ldr), temp(other.temp), pressure(other.pressure) {}

        float get_float(fields f) {
            switch (f) {
            case l:     return (float)ldr;
            case p:     return pressure;
            case t:     return temp;
            }
            // fallback required to silence warnings
            return ldr;
        }
};

struct dict_obj {
    std::string key;
    std::string val;
};
struct http_struct {
    dict_obj tx[3];
    dict_obj rx[1];
    int tx_len = 3;
    int rx_len = 1;
};

#endif