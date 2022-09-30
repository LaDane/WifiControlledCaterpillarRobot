#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

const char *hostName = "esp-async-wccr";
const char *ssid = "WCCR AP"; // The name of the Wi-Fi network that will be created
const char *password = "spoonman123";   // The password required to connect to it, leave blank for an open network

// API request inputs
float leftMotorInput = 0;
float rightMotorInput = 0;

// Motor values
// L298N max PWM = 256
signed short int speedChangeInterval = 8;

float leftMotorTargetSpeed = 0;
signed short int leftMotorSpeed = 0;
bool leftMotorForward = true;

float rightMotorTargetSpeed = 0;
signed short int rightMotorSpeed = 0;
bool rightMotorForward = true;

// Delays
int loopDelay = 25;

AsyncWebServer server(80);          // Create AsyncWebServer object on port 80

void setup() {
    Serial.begin(115200);
    Serial.println();
    
    Serial.println("Creating Access Point...");
    WiFi.softAP(hostName, password);
    // WiFi.begin(ssid, password);

    Serial.print("Access Point Name: ");
    Serial.println(WiFi.softAPSSID());
    
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());

    // API Requests
    server.on("/lm", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* param1 = request->getParam("input", true);
        // Serial.print("API /lm = ");
        // Serial.println(param1->value());
        leftMotorInput = param1->value().toFloat();
        request->send(200);
    });
    
    server.on("/rm", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* param2 = request->getParam("input", true);
        // Serial.print("API /rm = ");
        // Serial.println(param2->value());
        rightMotorInput = param2->value().toFloat();
        request->send(200);
    });

    server.on("/t", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("test");
        request->send_P(200, "text/plain", "Test succesfull");
    });

    server.begin();
    Serial.println("Setup completed");
}

void loop() {
    delay(loopDelay);
    
    // Motor Target speeds
    leftMotorTargetSpeed = leftMotorInput * 256;
    rightMotorTargetSpeed = rightMotorInput * 256;

    // ACTUAL MOTOR SPEEDS
    // Left motor buffers
    if (leftMotorInput == 0 && abs(leftMotorSpeed) == 8) {
        leftMotorSpeed = 0;
    }
    else if (leftMotorInput == 1 && abs(leftMotorSpeed) == 248) {
        leftMotorSpeed = 256;
    }
    // Left motor speed calculations
    if (leftMotorSpeed < leftMotorTargetSpeed && leftMotorSpeed + speedChangeInterval < leftMotorTargetSpeed) {
        leftMotorSpeed = leftMotorSpeed + speedChangeInterval;
    } 
    else if (leftMotorSpeed > leftMotorTargetSpeed && leftMotorSpeed - speedChangeInterval > leftMotorTargetSpeed) {
        leftMotorSpeed = leftMotorSpeed - speedChangeInterval;
    }

    // Right motor speed adjustments
    if (rightMotorInput == 0 && abs(rightMotorSpeed) == 8) {
        rightMotorSpeed = 0;
    }
    else if (rightMotorInput == 1 && abs(rightMotorSpeed) == 248) {
        rightMotorSpeed = 256;
    }
    if (rightMotorSpeed < rightMotorTargetSpeed && rightMotorSpeed + speedChangeInterval < rightMotorTargetSpeed) {
        rightMotorSpeed = rightMotorSpeed + speedChangeInterval;
    } 
    else if(rightMotorSpeed > rightMotorTargetSpeed && rightMotorSpeed - speedChangeInterval > rightMotorTargetSpeed) {
        rightMotorSpeed = rightMotorSpeed - speedChangeInterval;
    }

    // USE ABS() TO GET CORRECT VALUE FOR MOTOR PWN SIGNAL

    // Motor direction
    if (leftMotorSpeed > 0) {
        leftMotorForward = true;
    } else {
        leftMotorForward = false;
    }

    if (rightMotorSpeed > 0) {
        rightMotorForward = true;
    } else {
        rightMotorForward = false;
    }


    // Debug printing
    // Left motor
    Serial.print("lmI=");
    Serial.print(leftMotorInput);

    Serial.print(" lmTS=");
    Serial.print(leftMotorTargetSpeed);

    Serial.print(" lmS=");
    Serial.print(leftMotorSpeed);

    Serial.print(" lmF=");
    Serial.print(leftMotorForward);

    // Right motor
    Serial.print(" | rmI=");
    Serial.print(rightMotorInput);

    Serial.print(" rmTS=");
    Serial.print(rightMotorTargetSpeed);

    Serial.print(" rmS=");
    Serial.print(rightMotorSpeed);

    Serial.print(" rmF=");
    Serial.println(rightMotorForward);
}









// Blink LED

// #define LED 2

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(LED, OUTPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(LED, HIGH);
//   Serial.println("LED is on");

//   delay(1000);

//   digitalWrite(LED, LOW);
//   Serial.println("LED is off");

//   delay(1000);
// }