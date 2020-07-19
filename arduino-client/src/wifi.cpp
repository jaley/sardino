#include "common.hpp"
#include "wifi.hpp"

#include <SPI.h>

namespace ArduinoClient {

void Wifi::connectAndWait()
{
    info("Attempting to connect to Network named: " + m_ssid);

    int status = WL_IDLE_STATUS;
    while (status != WL_CONNECTED)
    {
        status = WiFi.begin(m_ssid.c_str(), m_secret.c_str());
        delay(250);
        info(String("status = ") + status);
    }
    info("connected!");

    // Log the network details
    info(String("SSID: ") + WiFi.SSID());
    info(String("IP Address: ") + WiFi.localIP());
}

} // namespace ArduinoClient