using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public enum JoystickSide {
    joystickLeft,
    joystickRight
}

public class InputController : MonoBehaviour {


    [SerializeField] private Joystick joystick;
    [SerializeField] private JoystickSide joystickSide;
    [SerializeField] private string test;

    private void Update() {
        switch (joystickSide) {
            case JoystickSide.joystickLeft:
            Debug.Log($"Left : {joystick.Vertical}");
            break;

            case JoystickSide.joystickRight:
            Debug.Log($"Right : {joystick.Vertical}");
            break;
        }
    }
}
