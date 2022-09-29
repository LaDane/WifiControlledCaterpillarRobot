#include <Arduino.h>


// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// // #include <ESPAsyncWebServer.h>
// #include "ESPAsyncWebServer.h"

#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

const char *hostName = "esp-async-wccr";
const char *ssid = "WCCR AP"; // The name of the Wi-Fi network that will be created
const char *password = "spoonman123";   // The password required to connect to it, leave blank for an open network
float leftMotorInput = 0;
float rightMotorInput = 0;

AsyncWebServer server(80);          // Create AsyncWebServer object on port 80


void setup() {
    Serial.begin(115200);
    Serial.println();
    
    // Creating wifi access point
    Serial.println("Creating Access Point...");
    // WiFi.softAP(ssid);
    // WiFi.begin(ssid, password);

    // WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(hostName, password);
    // WiFi.begin(ssid, password);

    Serial.print("Access Point Name: ");
    Serial.println(WiFi.softAPSSID());
    
    // IPAddress IP = WiFi.softAPIP();
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());

    // API Requests
    server.on("/lm", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* param1 = request->getParam("input", true);
        leftMotorInput = param1->value().toFloat();
    });
    
    server.on("/rm", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* param2 = request->getParam("input", true);
        rightMotorInput = param2->value().toFloat();
    });

    server.on("/t", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("test");
        request->send_P(200, "text/plain", "Test succesfull");
    });

    server.begin();
}

void loop() {
    // delay(1000);
    // Serial.print("LMI = ");
    // Serial.print(leftMotorInput);
    // Serial.print(" | RMI = ");
    // Serial.print(rightMotorInput);
    // Serial.print(" | Connected clients = ");
    // Serial.println();
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