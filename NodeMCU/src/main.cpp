#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "Motor.h"

const char *hostName = "esp-async-wccr";    // Access Point WiFi name
// const char *ssid = "WCCR AP"; // The name of the Wi-Fi network that will be created
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
    // WiFi.begin(ssid, password);

    Serial.print("Access Point Name: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());

    // API Requests
    server.on("/lm", HTTP_POST, [](AsyncWebServerRequest *request) {
        totalPostsAPI++;
        AsyncWebParameter* param = request->getParam("input", true);
        leftMotorInput = abs(param->value().toInt());
        request->send(200);
    });
    
    server.on("/rm", HTTP_POST, [](AsyncWebServerRequest *request) {
        totalPostsAPI++;
        AsyncWebParameter* param = request->getParam("input", true);
        rightMotorInput = param->value().toInt();
        request->send(200);
    });

    // server.on("/t", HTTP_GET, [](AsyncWebServerRequest *request){
    //     Serial.println("test");
    //     request->send_P(200, "text/plain", "Test succesfull");
    // });

    server.begin();
    Serial.println("Access Point setup completed");
}

void loop() {
    delay(loopDelay);

    leftMotor.update(leftMotorInput);
    rightMotor.update(rightMotorInput);

    debugData();
}

void debugData() {
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
