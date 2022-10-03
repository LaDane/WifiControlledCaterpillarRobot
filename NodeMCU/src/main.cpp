#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

const char *hostName = "esp-async-wccr";
const char *ssid = "WCCR AP"; // The name of the Wi-Fi network that will be created
const char *password = "spoonman123";   // The password required to connect to it, leave blank for an open network

// API request inputs
signed short int totalPostsAPI = 0;
signed short int leftMotorInput = 0;
signed short int rightMotorInput = 0;

// Motor values
// L298N max PWM = 256
signed short int speedChangeInterval = 1;

// float leftMotorTargetSpeed = 0;
signed short int leftMotorSpeed = 0;
bool leftMotorForward = true;

// float rightMotorTargetSpeed = 0;
signed short int rightMotorSpeed = 0;
bool rightMotorForward = true;

// Delays
int loopDelay = 5;

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
        totalPostsAPI++;
        AsyncWebParameter* param = request->getParam("input", true);

        // param->value().toInt() > 0 ? leftMotorForward = true : leftMotorForward = false;
        leftMotorInput = abs(param->value().toInt());
        request->send(200);
    });
    
    server.on("/rm", HTTP_POST, [](AsyncWebServerRequest *request) {
        totalPostsAPI++;
        AsyncWebParameter* param = request->getParam("input", true);

        // param->value().toInt() > 0 ? rightMotorForward = true : rightMotorForward = false;
        rightMotorInput = param->value().toInt();
        request->send(200);
    });

    // server.on("/t", HTTP_GET, [](AsyncWebServerRequest *request){
    //     Serial.println("test");
    //     request->send_P(200, "text/plain", "Test succesfull");
    // });

    server.begin();
    Serial.println("Setup completed");
}


void loop() {
    delay(loopDelay);

    // leftMotorSpeed < leftMotorInput ? leftMotorSpeed += speedChangeInterval : leftMotorSpeed -= speedChangeInterval;
    // leftMotorSpeed > 0 ? leftMotorForward = true : leftMotorForward = false;
    
    if (leftMotorSpeed < leftMotorInput) {
        leftMotorSpeed = leftMotorSpeed + speedChangeInterval;
    }
    else if (leftMotorSpeed > leftMotorInput) {
        leftMotorSpeed = leftMotorSpeed - speedChangeInterval;
    }
    else {
        leftMotorSpeed = leftMotorInput;
    }
    if (leftMotorSpeed > 0) {
        leftMotorForward = true;
    }
    else {
        leftMotorForward = false;
    }

    if (rightMotorSpeed < rightMotorInput) {
        rightMotorSpeed = rightMotorSpeed + speedChangeInterval;
    }
    else if (rightMotorSpeed > rightMotorInput) {
        rightMotorSpeed = rightMotorSpeed - speedChangeInterval;
    }
    else {
        rightMotorSpeed = rightMotorInput;
    }
    if (rightMotorSpeed > 0) {
        rightMotorForward = true;
    }
    else {
        rightMotorForward = false;
    }

    // rightMotorSpeed < rightMotorInput ? rightMotorSpeed += speedChangeInterval : rightMotorSpeed -= speedChangeInterval;
    // rightMotorSpeed > 0 ? rightMotorForward = true : rightMotorForward = false;


    Serial.print("Lmotor : ");
    Serial.print(leftMotorSpeed);
    Serial.print("\t Lforward : ");
    Serial.print(leftMotorForward);

    Serial.print("\t\t Rmotor : ");
    Serial.print(rightMotorSpeed);
    Serial.print("\t Rforward : ");
    Serial.print(rightMotorForward);

    Serial.print( "\t (");
    Serial.print(totalPostsAPI);
    Serial.println(")");
}


// old loop
// void loop() {
//     delay(loopDelay);
    
//     // Motor Target speeds
//     leftMotorTargetSpeed = leftMotorInput * 256;
//     rightMotorTargetSpeed = rightMotorInput * 256;

//     // ACTUAL MOTOR SPEEDS
//     // Left motor buffers
//     if (leftMotorInput == 0 && abs(leftMotorSpeed) == 8) {
//         leftMotorSpeed = 0;
//     }
//     else if (leftMotorInput == 1 && abs(leftMotorSpeed) == 248) {
//         leftMotorSpeed = 256;
//     }
//     // Left motor speed calculations
//     if (leftMotorSpeed < leftMotorTargetSpeed && leftMotorSpeed + speedChangeInterval < leftMotorTargetSpeed) {
//         leftMotorSpeed = leftMotorSpeed + speedChangeInterval;
//     } 
//     else if (leftMotorSpeed > leftMotorTargetSpeed && leftMotorSpeed - speedChangeInterval > leftMotorTargetSpeed) {
//         leftMotorSpeed = leftMotorSpeed - speedChangeInterval;
//     }

//     // Right motor speed adjustments
//     if (rightMotorInput == 0 && abs(rightMotorSpeed) == 8) {
//         rightMotorSpeed = 0;
//     }
//     else if (rightMotorInput == 1 && abs(rightMotorSpeed) == 248) {
//         rightMotorSpeed = 256;
//     }
//     if (rightMotorSpeed < rightMotorTargetSpeed && rightMotorSpeed + speedChangeInterval < rightMotorTargetSpeed) {
//         rightMotorSpeed = rightMotorSpeed + speedChangeInterval;
//     } 
//     else if(rightMotorSpeed > rightMotorTargetSpeed && rightMotorSpeed - speedChangeInterval > rightMotorTargetSpeed) {
//         rightMotorSpeed = rightMotorSpeed - speedChangeInterval;
//     }

//     // USE ABS() TO GET CORRECT VALUE FOR MOTOR PWN SIGNAL

//     // Motor direction
//     if (leftMotorSpeed > 0) {
//         leftMotorForward = true;
//     } else {
//         leftMotorForward = false;
//     }

//     if (rightMotorSpeed > 0) {
//         rightMotorForward = true;
//     } else {
//         rightMotorForward = false;
//     }


//     // Debug printing
//     // Left motor
//     Serial.print("lmI=");
//     Serial.print(leftMotorInput);

//     Serial.print(" lmTS=");
//     Serial.print(leftMotorTargetSpeed);

//     Serial.print(" lmS=");
//     Serial.print(leftMotorSpeed);

//     Serial.print(" lmF=");
//     Serial.print(leftMotorForward);

//     // Right motor
//     Serial.print(" | rmI=");
//     Serial.print(rightMotorInput);

//     Serial.print(" rmTS=");
//     Serial.print(rightMotorTargetSpeed);

//     Serial.print(" rmS=");
//     Serial.print(rightMotorSpeed);

//     Serial.print(" rmF=");
//     Serial.print(rightMotorForward);

//     // API counter
//     Serial.print( " | (");
//     Serial.print(totalPostsAPI);
//     Serial.println(")");
// }









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