#include <Arduino.h>

#include "secrets.h"
#include "wifi.hpp"
#include "web.hpp"

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
    ArduinoClient::Web web(
        WIFI.client(),
        ArduinoClient::Secrets::PROXY_HOSTNAME,
        ArduinoClient::Secrets::PROXY_AUTH_USER,
        ArduinoClient::Secrets::PROXY_AUTH_PASS
    );

    web.get("/arduino/api/groups");

    Serial.println("looping...");
    delay(10000);
}
