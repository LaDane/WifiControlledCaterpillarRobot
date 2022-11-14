#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

#define SCI 1       // Speed Change Interval

class Motor {
    public:
        Motor(bool motorReversed, int ENA, int IN1, int IN2);
        void setup();
        void update(signed short int motorInput);
        signed short int motorSpeed;
        bool motorDir;
    private:
        bool _motorReversed;
        int _ENA;
        int _IN1;
        int _IN2;
};

#endif