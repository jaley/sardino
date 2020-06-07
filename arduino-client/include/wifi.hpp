#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFiNINA.h>

namespace ArduinoClient
{

    class Wifi
    {
    public:
        Wifi(const String& ssid, const String& secret)
        : m_ssid(ssid), m_secret(secret) {};
        ~Wifi() {};

        // Connects to WiFi and blocks until connected
        void connectAndWait();

        // Get a reference to the NINA client
        inline WiFiClient& client() {
            return m_wifi;
        }

    private:
        const String& m_ssid;
        const String& m_secret;

        WiFiClient m_wifi;
    };

};

#endif // WIFI_H