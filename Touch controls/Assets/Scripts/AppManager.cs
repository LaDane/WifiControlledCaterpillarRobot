using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class AppManager : MonoBehaviour {

    [Header("API Settings")]
    public bool transmitData = false;
    [SerializeField] private float timeBetweenRequests = 0.25f;

    [Header("Motor Settings")]
    [SerializeField] public float timeBetweenUpdates = 0.25f;
    [SerializeField] public int speedChangeInterval = 8;


    public List<MotorInput> motorInputs = new List<MotorInput>();
    //private bool startedCoroutines = false;


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

    //private void Update() {
    //    if (!startedCoroutines) {
    //        for (int i = 0; i < motorInputs.Count; i++) {
    //            StartCoroutine(SendInputApiCheck(motorInputs[i]));
    //        }
    //        startedCoroutines = true;
    //    }
    //}


    //private IEnumerator SendInputApiCheck(MotorInput motorInput) {
    //    while(true) {
    //        yield return new WaitForSeconds(timeBetweenRequests);
            
    //        if (transmitData && motorInput.input != motorInput.lastInput) {
    //            motorInput.lastInput = motorInput.input;
    //            StartCoroutine(SendInputApi(motorInput));
    //        }
    //    }
    //}

    public IEnumerator SendInputApi(MotorInput motorInput) {
        WWWForm miForm = new WWWForm();
        //miForm.AddField("input", motorInput.input.ToString());
        miForm.AddField("input", motorInput.calculatedMotorSpeed.ToString());

        using (UnityWebRequest www = UnityWebRequest.Post(motorInput.inputApiEndpoint, miForm)) {
            yield return www.SendWebRequest();
        }
        //Debug.Log($"{motorInput.motorName} | {motorInput.calculatedMotorSpeed}");
    }
}
