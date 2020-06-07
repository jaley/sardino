#include <Arduino.h>

#include "secrets.h"
#include "wifi.hpp"

ArduinoClient::Wifi WIFI(
    ArduinoClient::Secrets::WIFI_SSID, 
    ArduinoClient::Secrets::WIFI_SECRET_KEY
);

void setup()
{
    // Init and wait for serial
    Serial.begin(9600);
    while (!Serial);

    WIFI.connectAndWait();
}

void loop()
{
    Serial.println("idle...");
    delay(10000);
}
