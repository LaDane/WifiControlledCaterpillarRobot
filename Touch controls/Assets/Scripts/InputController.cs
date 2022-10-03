using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.Networking;
using UnityEngine.UI;

//[System.Serializable]
//public enum JoystickSide {
//    joystickLeft,
//    joystickRight
//}

public class InputController : MonoBehaviour, IDragHandler, IPointerDownHandler, IPointerUpHandler {

    //[Header("API")]
    //[SerializeField] private string inputApiEndpoint;

    [Header("Joystick")]
    //[SerializeField] private JoystickSide joystickSide;
    [SerializeField] private Image joystickBg;
    [SerializeField] private Image joystickHandle;

    [Header("Motor")]
    [SerializeField] private MotorInput motorInput;
    //[SerializeField] private string motorName;
    //[SerializeField] private int maxPWM;

    //[Header("Slider Values")]
    //[SerializeField] private float motorInput.input;

    private Vector2 posInput;           // slider touch pos
    private Vector2 startPos;           // slider start pos
    //private int moterInputID;           // array index of motor in AppManager
    private bool startedCoroutines = false;
    private bool validData = true;

    private void Start() {
        startPos = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, joystickHandle.rectTransform.anchoredPosition.y);

        // Add MotorInput object to singleton
        //AppManager.Instance.motorInputs.Add(
        //    new MotorInput(
        //        motorName,
        //        maxPWM,
        //        0, 
        //        0,
        //        0,
        //        inputApiEndpoint
        //    )
        //);

        //for (int i = 0; i < AppManager.Instance.motorInputs.Count; i++) {
        //    if (AppManager.Instance.motorInputs[i].motorName.Equals(motorName)) {
        //        moterInputID = i;
        //    }
        //}
    }

    private void Update() {
        if (!startedCoroutines) {
            StartCoroutine(UpdateCalculatedMotorSpeed());
            StartCoroutine(SendApiDataTimer());
            startedCoroutines = true;
        }

        //AppManager.Instance.motorInputs[moterInputID].input = motorInput.input;
    }

    private IEnumerator UpdateCalculatedMotorSpeed() {
        while(true) {
            //yield return new WaitForSeconds(AppManager.Instance.timeBetweenUpdates);
            yield return null;        // wait 1 frame

            float motorTargetSpeed = motorInput.input * motorInput.maxPWM;

            // Motor speed adjustment calculations
            if (AppManager.Instance.transmitData) {
                if (motorInput.input == 0 && Mathf.Abs(motorInput.calculatedMotorSpeed) == 8) {
                    motorInput.calculatedMotorSpeed = 0;
                    //StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }
                else if (Mathf.Abs(motorInput.input) == 1 && Mathf.Abs(motorInput.calculatedMotorSpeed) == 248) {
                    motorInput.calculatedMotorSpeed = 256 * Mathf.RoundToInt(motorInput.input);
                    //StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }

                if (motorInput.calculatedMotorSpeed < motorTargetSpeed &&
                    motorInput.calculatedMotorSpeed + AppManager.Instance.speedChangeInterval < motorTargetSpeed) {

                    motorInput.calculatedMotorSpeed += AppManager.Instance.speedChangeInterval;
                    //StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }
                else if (motorInput.calculatedMotorSpeed > motorTargetSpeed &&
                    motorInput.calculatedMotorSpeed - AppManager.Instance.speedChangeInterval > motorTargetSpeed) {

                    motorInput.calculatedMotorSpeed -= AppManager.Instance.speedChangeInterval;
                    //StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }
            }
            if (motorInput.calculatedMotorSpeed != 0) {
                validData = true;
            }
        }
    }

    private IEnumerator SendApiDataTimer() {
        while(true) {
            yield return new WaitForSeconds(AppManager.Instance.timeBetweenRequests);
            //if (AppManager.Instance.transmitData) {
            if (AppManager.Instance.transmitData && validData) {
                StartCoroutine(SendApiDataRequest());
            }
        }
    }

    private IEnumerator SendApiDataRequest() {
        WWWForm motorInputForm = new WWWForm();
        motorInputForm.AddField("input", motorInput.calculatedMotorSpeed.ToString());

        using (UnityWebRequest www = UnityWebRequest.Post(motorInput.inputApiEndpoint, motorInputForm)) {
            yield return www.SendWebRequest();
        }
        if (motorInput.calculatedMotorSpeed == 0) {
            validData = false;
        }
        //motorInput.lastInput = motorInput.input;
        Debug.Log($"{motorInput.motorName} | {motorInput.calculatedMotorSpeed}");
    }

    public void OnDrag(PointerEventData eventData) {
        if (RectTransformUtility.ScreenPointToLocalPointInRectangle(
            joystickBg.rectTransform,
            eventData.position,
            eventData.pressEventCamera,
            out posInput)) 
        {
            //Debug.Log(posInput);
            // Calcualte vertical movement of touch input
            motorInput.input = (posInput.y / joystickBg.rectTransform.sizeDelta.y) * 2.4f;
            if (motorInput.input > 1) {
                motorInput.input = 1;
            }
            else if (motorInput.input < -1) {
                motorInput.input = -1;
            }
            //motorInput.input = (float)System.Math.Round(motorInput.input, 2);

            // joystickHandle movement
            joystickHandle.rectTransform.anchoredPosition = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, posInput.y);
        }
    }

    public void OnPointerDown(PointerEventData eventData) {
        OnDrag(eventData);
    }

    public void OnPointerUp(PointerEventData eventData) {
        posInput = Vector2.zero;
        motorInput.input = 0;
        joystickHandle.rectTransform.anchoredPosition = startPos;
    }
}
