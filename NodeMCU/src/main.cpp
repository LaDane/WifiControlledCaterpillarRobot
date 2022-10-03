#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "Motor.h"

const char *hostName = "esp-async-wccr";    // Access Point WiFi name
const char *password = "spoonman123";   // The password required to connect to it, leave blank for an open network

// API requests
signed short int leftMotorInput = 0;
signed short int rightMotorInput = 0;
signed short int totalPostsAPI = 0;

// Motors
Motor leftMotor(false);
Motor rightMotor(false);

// Delays
int loopDelay = 5;

AsyncWebServer server(80);          // Create AsyncWebServer object on port 80

void setup() {
    Serial.begin(115200);
    Serial.println();
    
    Serial.println("Creating Access Point...");
    WiFi.softAP(hostName, password);

    Serial.print("Access Point Name: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());

    // API Requests
    server.on("/lm", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* param = request->getParam("input", true);
        // Serial.println(param->value());
        leftMotorInput = param->value().toInt();
        request->send(200);
        totalPostsAPI++;
    });
    
    server.on("/rm", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* param = request->getParam("input", true);        
        // Serial.println(param->value());
        rightMotorInput = param->value().toInt();
        request->send(200);
        totalPostsAPI++;
    });

    // server.on("/t", HTTP_GET, [](AsyncWebServerRequest *request){
    //     Serial.println("test");
    //     request->send_P(200, "text/plain", "Test succesfull");
    // });

    server.begin();
    Serial.println("Access Point setup completed");
}

void DebugData() {
    Serial.print("Lmotor : ");
    Serial.print(leftMotor.motorSpeed);
    Serial.print("\t Lforward : ");
    Serial.print(leftMotor.motorDir);

    Serial.print("\t\t Rmotor : ");
    Serial.print(rightMotor.motorSpeed);
    Serial.print("\t Rforward : ");
    Serial.print(rightMotor.motorDir);

    Serial.print( "\t (");
    Serial.print(totalPostsAPI);
    Serial.println(")");
}

void loop() {
    delay(loopDelay);

    leftMotor.update(leftMotorInput);
    rightMotor.update(rightMotorInput);

    // Use absolute value of motor.motorSpeed when controlling motor PWM signal

    // DebugData();
}


