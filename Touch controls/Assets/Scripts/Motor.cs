using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class Motor {
    public float input;
    public int output;
    public int lastSentOutput;
    public int maxPWM = 255;
    public string ApiEndpoint;
}
