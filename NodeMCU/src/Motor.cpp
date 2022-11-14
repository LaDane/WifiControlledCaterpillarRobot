#include <Arduino.h>
#include "Motor.h"

Motor::Motor(bool motorReversed, int ENA, int IN1, int IN2) {
    _motorReversed = motorReversed;
    _ENA = ENA;
    _IN1 = IN1;
    _IN2 = IN2;
}

void Motor::setup() {
    pinMode(_ENA, OUTPUT);
    pinMode(_IN1, OUTPUT);
    pinMode(_IN2, OUTPUT);
}

void Motor::update(signed short int motorInput) {
    if (motorSpeed != motorInput) {
        if (motorSpeed < motorInput) {
            motorSpeed += SCI;
        } 
        else if (motorSpeed > motorInput) {
            motorSpeed -= SCI;
        }

        // Direction
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

        //Output signal to GPIO pins
        if (motorDir) {
            digitalWrite(_IN1, HIGH);
            digitalWrite(_IN2, LOW);
        }
        else {
            digitalWrite(_IN1, LOW);
            digitalWrite(_IN2, HIGH);
        }
        analogWrite(_ENA, abs(motorSpeed));
    }
}