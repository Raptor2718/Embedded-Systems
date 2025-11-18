#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

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
};

#endif