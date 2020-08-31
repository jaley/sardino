#include "common.hpp"

#include "secrets.h"
#include "sonos.hpp"
#include "wifi.hpp"
#include "web.hpp"
#include "volume.hpp"

using namespace ArduinoClient;


Wifi WIFI(
    ArduinoClient::Secrets::WIFI_SSID, 
    ArduinoClient::Secrets::WIFI_SECRET_KEY
);

const byte ROTARY_ENCODER_INTERRUPT_PIN_A(0);
const byte ROTARY_ENCODER_INTERRUPT_PIN_B(1);
Volume VOLUME(ROTARY_ENCODER_INTERRUPT_PIN_A, ROTARY_ENCODER_INTERRUPT_PIN_B);


void setup()
{
    // Init and wait for serial
    Serial.begin(9600);
    while (!Serial);

    WIFI.connectAndWait();
}

void testWeb()
{
   Web web(
        WIFI.client(),
        Secrets::PROXY_HOSTNAME,
        Secrets::PROXY_AUTH_USER,
        Secrets::PROXY_AUTH_PASS
    );

    Sonos sonos(web);

    Group groups[5] = {}; 
    sonos.getGroups(groups, 5);

    Serial.println(String("Got groups: ") + groups[0].m_groupName);
}

void testEncoder()
{
    VOLUME.read();
    // Serial.println("Volume Change: " + String(VOLUME.read()));
}

void loop()
{
    // testWeb();

    testEncoder();

    // Serial.println("looping...");
    // delay(10000);
}

