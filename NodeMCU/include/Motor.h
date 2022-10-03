#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

#define SCI 1       // Speed Change Interval

class Motor {
    public:
        Motor(bool motorReversed);
        void update(signed short int motorInput);
        signed short int motorSpeed;
        bool motorDir;
    private:
        bool _motorReversed;
};

#endif