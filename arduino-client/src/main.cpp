#include "common.hpp"

#include "secrets.h"
#include "sonos.hpp"
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

    ArduinoClient::Sonos sonos(web);

    ArduinoClient::Group groups[5] = {}; 
    sonos.getGroups(groups, 5);

    Serial.println(String("Got groups: ") + groups[0].m_groupName);

    Serial.println("looping...");
    delay(10000);
}
