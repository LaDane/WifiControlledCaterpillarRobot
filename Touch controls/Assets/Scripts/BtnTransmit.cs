using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BtnTransmit : MonoBehaviour {

    [SerializeField] private Image btnImage;

    private void Start() {
        btnImage.color = Color.red;
    }

    public void ToggleTransmitBtn() {
        AppManager.Instance.transmitData = !AppManager.Instance.transmitData;
        if (AppManager.Instance.transmitData) {
            btnImage.color = Color.green;
        }
        else {
            btnImage.color = Color.red;
        }
    }
}
