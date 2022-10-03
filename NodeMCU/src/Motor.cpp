#include <Arduino.h>
#include "Motor.h"

Motor::Motor(bool motorReversed) {
    _motorReversed = motorReversed;
}

void Motor::update(signed short int motorInput) {
    if (motorSpeed != motorInput) {
        if (motorSpeed < motorInput) {
            motorSpeed += SCI;
        } 
        else if (motorSpeed > motorInput) {
            motorSpeed -= SCI;
        }

        if (motorSpeed > 0) {
            motorDir = true;
            if (_motorReversed) {
                motorDir = false;
            }
        }
        else {
            motorDir = false;
            if (_motorReversed) {
                motorDir = true;
            }
        }
    }
}