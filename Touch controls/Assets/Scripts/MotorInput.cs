using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MotorInput {

    public string motorName;
    public int maxPWM;
    public float input;
    public float lastInput;
    public int calculatedMotorSpeed;
    public string inputApiEndpoint;

    public MotorInput(string motorName, int maxPWM, float input, float lastInput, int calculatedMotorSpeed, string inputApiEndpoint) {
        this.motorName = motorName;
        this.maxPWM = maxPWM;
        this.input = input;
        this.lastInput = lastInput;
        this.calculatedMotorSpeed = calculatedMotorSpeed;
        this.inputApiEndpoint = inputApiEndpoint;
    }
}
