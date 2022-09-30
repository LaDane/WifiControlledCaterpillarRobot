using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class AppManager : MonoBehaviour {

    [Header("API settings")]
    [SerializeField] private float timeBetweenRequests = 0.25f;
    [SerializeField] private string leftMotorEndpoint = "192.168.4.1/lm";
    [SerializeField] private string rightMotorEndpoint = "192.168.4.1/rm";

    [Header("Touch input variables")]
    public bool transmitData = false;
    public float leftStickInput;
    public float rightStickInput;

    private float lastLeftStickInput;
    private float lastRightStickInput;

    private static AppManager _instance;
    public static AppManager Instance {
        get {
            return _instance;
        }
        private set {
            _instance = value;
        }
    }

    private void Awake() {
        if (Instance == null) {
            Instance = this;
            return;
        }
        Destroy(gameObject);
    }

    private void Start() {
        StartCoroutine(leftMotorAPI());
        StartCoroutine(rightMotorAPI());
    }

    private IEnumerator leftMotorAPI() {
        while(true) {
            yield return new WaitForSeconds(timeBetweenRequests);
            if (transmitData) {

                if (leftStickInput != lastLeftStickInput) {
                    WWWForm lmForm = new WWWForm();
                    lmForm.AddField("input", leftStickInput.ToString());
                    using (UnityWebRequest www = UnityWebRequest.Post(leftMotorEndpoint, lmForm)) {
                        yield return www.SendWebRequest();
                    }
                    lastLeftStickInput = leftStickInput;
                    //Debug.Log($"Sent API request to Left Motor | {leftStickInput}");
                }
            }
        }
    }

    private IEnumerator rightMotorAPI() {
        while (true) {
            yield return new WaitForSeconds(timeBetweenRequests);
            if (transmitData) {

                if (rightStickInput != lastRightStickInput) {
                    WWWForm rmForm = new WWWForm();
                    rmForm.AddField("input", rightStickInput.ToString());
                    using (UnityWebRequest www = UnityWebRequest.Post(rightMotorEndpoint, rmForm)) {
                        yield return www.SendWebRequest();
                    }
                    lastRightStickInput = rightStickInput;
                    //Debug.Log($"Sent API request to Right Motor | {rightStickInput}");
                }
            }
        }
    }
}
