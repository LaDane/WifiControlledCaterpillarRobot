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

    [SerializeField] private JoystickSide joystickSide;
    [SerializeField] private Image joystickBg;
    [SerializeField] private Image joystickHandle;
    [SerializeField] private float verticalInput;

    private Vector2 posInput;
    private Vector2 startPos;

    private void Start() {
        startPos = new Vector2(joystickHandle.rectTransform.anchoredPosition.x, joystickHandle.rectTransform.anchoredPosition.y);
    }

    private void Update() {
        switch (joystickSide) {
            case JoystickSide.joystickLeft: AppManager.Instance.leftStickInput = verticalInput; break;
            case JoystickSide.joystickRight: AppManager.Instance.rightStickInput = verticalInput; break;
        }
    }

    public void OnDrag(PointerEventData eventData) {
        if (RectTransformUtility.ScreenPointToLocalPointInRectangle(
            joystickBg.rectTransform,
            eventData.position,
            eventData.pressEventCamera,
            out posInput)) 
        {
            // Calcualte vertical movement of touch input
            verticalInput = (posInput.y / joystickBg.rectTransform.sizeDelta.y) * 2.4f;
            if (verticalInput > 1) {
                verticalInput = 1;
            }
            else if (verticalInput < -1) {
                verticalInput = -1;
            }
            verticalInput = (float)System.Math.Round(verticalInput, 2);

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
