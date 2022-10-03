using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class AppManager : MonoBehaviour {

    [Header("API Settings")]
    public bool transmitData = false;
    public float timeBetweenRequests = 0.25f;


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
}
