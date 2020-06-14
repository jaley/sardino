#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFiNINA.h>

#include "common.hpp"

namespace ArduinoClient {

class Wifi : public Component
{
public:
    Wifi(const String& ssid, const String& secret)
    : Component("Wifi"), m_ssid(ssid), m_secret(secret) {};
    ~Wifi() {};

    // Connects to WiFi and blocks until connected
    void connectAndWait();

    // Get a reference to the NINA client
    inline WiFiSSLClient& client() {
        return m_wifi;
    }

private:
    const String& m_ssid;
    const String& m_secret;

    WiFiSSLClient m_wifi;
};


}; // namespace ArduinoClient

#endif // WIFI_H