#include "wifi.hpp"

#include <SPI.h>

namespace ArduinoClient {

void Wifi::connectAndWait()
{
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(m_ssid);     // print the network name (SSID);

    int status = WL_IDLE_STATUS;
    while (status != WL_CONNECTED)
    {
        Serial.print(".");
        status = WiFi.begin(m_ssid.c_str(), m_secret.c_str());
        delay(500);
    }
    Serial.println("connected!");

    // Log the network details
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

} // namespace ArduinoClient