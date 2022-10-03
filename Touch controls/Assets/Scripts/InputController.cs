using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.Networking;
using UnityEngine.UI;

public class InputController : MonoBehaviour, IDragHandler, IPointerDownHandler, IPointerUpHandler {

    [Header("Joystick")]
    [SerializeField] private Image joystickBg;
    [SerializeField] private Image joystickHandle;

    [Header("Motor")]
    [SerializeField] private Motor motor;

    private Vector2 posInput;           // slider touch pos
    private Vector2 startPos;           // slider start pos

    private void Start() {
        startPos = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, joystickHandle.rectTransform.anchoredPosition.y);
        StartCoroutine(ApiDataTimer());
    }

    private void Update() {
        if (!AppManager.Instance.transmitData) {
            return;
        }
        motor.output = Mathf.RoundToInt(motor.input * motor.maxPWM);
    }

    private IEnumerator ApiDataTimer() {
        while(true) {
            yield return new WaitForSeconds(AppManager.Instance.timeBetweenRequests);
            //if (AppManager.Instance.transmitData) {
            if (AppManager.Instance.transmitData && motor.output != motor.lastSentOutput) {
                StartCoroutine(SendApiDataRequest());
            }
        }
    }

    private IEnumerator SendApiDataRequest() {
        WWWForm motorInputForm = new WWWForm();
        motorInputForm.AddField("input", motor.output.ToString());

        using (UnityWebRequest www = UnityWebRequest.Post(motor.ApiEndpoint, motorInputForm)) {
            yield return www.SendWebRequest();
        }
        motor.lastSentOutput = motor.output;
        Debug.Log($"{motor.output}");
    }

    public void OnDrag(PointerEventData eventData) {
        if (RectTransformUtility.ScreenPointToLocalPointInRectangle(
            joystickBg.rectTransform,
            eventData.position,
            eventData.pressEventCamera,
            out posInput)) 
        {
            // Calcualte vertical movement of touch input
            motor.input = (posInput.y / joystickBg.rectTransform.sizeDelta.y) * 2.4f;
            if (motor.input > 1) {
                motor.input = 1;
            }
            else if (motor.input < -1) {
                motor.input = -1;
            }

            // joystickHandle movement
            joystickHandle.rectTransform.anchoredPosition = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, posInput.y);
        }
    }

    public void OnPointerDown(PointerEventData eventData) {
        OnDrag(eventData);
    }

    public void OnPointerUp(PointerEventData eventData) {
        posInput = Vector2.zero;
        motor.input = 0;
        joystickHandle.rectTransform.anchoredPosition = startPos;
    }
}
