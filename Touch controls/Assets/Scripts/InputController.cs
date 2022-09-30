using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

[System.Serializable]
public enum JoystickSide {
    joystickLeft,
    joystickRight
}

public class InputController : MonoBehaviour, IDragHandler, IPointerDownHandler, IPointerUpHandler {

    [Header("API")]
    [SerializeField] private string inputApiEndpoint;

    [Header("Joystick")]
    [SerializeField] private JoystickSide joystickSide;
    [SerializeField] private Image joystickBg;
    [SerializeField] private Image joystickHandle;

    [Header("Motor")]
    [SerializeField] private string motorName;
    [SerializeField] private int maxPWM;

    [Header("Slider Values")]
    [SerializeField] private float verticalInput;

    private Vector2 posInput;           // slider touch pos
    private Vector2 startPos;           // slider start pos
    private int moterInputID;           // array index of motor in AppManager
    private bool startedMotorCalculations = false;

    private void Start() {
        startPos = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, joystickHandle.rectTransform.anchoredPosition.y);

        // Add MotorInput object to singleton
        AppManager.Instance.motorInputs.Add(
            new MotorInput(
                motorName,
                maxPWM,
                0, 
                0,
                0,
                inputApiEndpoint
            )
        );

        for (int i = 0; i < AppManager.Instance.motorInputs.Count; i++) {
            if (AppManager.Instance.motorInputs[i].motorName.Equals(motorName)) {
                moterInputID = i;
            }
        }
    }

    private void Update() {
        if (!startedMotorCalculations) {
            StartCoroutine(UpdateCalculatedMotorSpeed());
            startedMotorCalculations = true;
            Debug.Log("Here");
        }

        AppManager.Instance.motorInputs[moterInputID].input = verticalInput;
    }

    private IEnumerator UpdateCalculatedMotorSpeed() {
        while(true) {
            yield return new WaitForSeconds(AppManager.Instance.timeBetweenUpdates);
            //yield return null;        // wait 1 frame

            float motorTargetSpeed = verticalInput * maxPWM;

            if (AppManager.Instance.transmitData) {
                if (verticalInput == 0 && Mathf.Abs(AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed) == 8) {
                    AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed = 0;
                    StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }
                else if (Mathf.Abs(verticalInput) == 1 && Mathf.Abs(AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed) == 248) {
                    AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed = 256 * Mathf.RoundToInt(verticalInput);
                    StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }

                if (AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed < motorTargetSpeed &&
                    AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed + AppManager.Instance.speedChangeInterval < motorTargetSpeed) {
                
                    AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed += AppManager.Instance.speedChangeInterval;
                    StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }
                else if (AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed > motorTargetSpeed &&
                    AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed - AppManager.Instance.speedChangeInterval > motorTargetSpeed) {
                
                    AppManager.Instance.motorInputs[moterInputID].calculatedMotorSpeed -= AppManager.Instance.speedChangeInterval;
                    StartCoroutine(AppManager.Instance.SendInputApi(AppManager.Instance.motorInputs[moterInputID]));
                }
            }
        }
        // 16 * 16 = 256
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
            verticalInput = (posInput.y / joystickBg.rectTransform.sizeDelta.y) * 2.4f;
            if (verticalInput > 1) {
                verticalInput = 1;
            }
            else if (verticalInput < -1) {
                verticalInput = -1;
            }
            //verticalInput = (float)System.Math.Round(verticalInput, 2);

            // joystickHandle movement
            joystickHandle.rectTransform.anchoredPosition = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, posInput.y);
        }
    }

    public void OnPointerDown(PointerEventData eventData) {
        OnDrag(eventData);
    }

    public void OnPointerUp(PointerEventData eventData) {
        posInput = Vector2.zero;
        verticalInput = 0;
        joystickHandle.rectTransform.anchoredPosition = startPos;
    }
}
