#include "common.hpp"

#include "secrets.h"
#include "sonos.hpp"
#include "wifi.hpp"
#include "web.hpp"
#include "volume.hpp"
#include "ui.hpp"

using namespace ArduinoClient;

// WiFi
Wifi WIFI(
    ArduinoClient::Secrets::WIFI_SSID, 
    ArduinoClient::Secrets::WIFI_SECRET_KEY
);

// Encoder
const byte ROTARY_ENCODER_INTERRUPT_PIN_A(0);
const byte ROTARY_ENCODER_INTERRUPT_PIN_B(1);
Volume VOLUME(ROTARY_ENCODER_INTERRUPT_PIN_A, ROTARY_ENCODER_INTERRUPT_PIN_B);

// Display
U8G2_SH1107_PIMORONI_128X128_F_4W_HW_SPI U8G2_DISPLAY(U8G2_R0, A1, A2, U8X8_PIN_NONE);
Ui UI(U8G2_DISPLAY);


void setup()
{
    // Init and wait for serial
    Serial.begin(9600);
    while (!Serial);

    U8G2_DISPLAY.begin();
    UI.setDrawable(&CONNECTING);
    UI.redraw();

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
    Serial.println("Volume Change: " + String(VOLUME.read()));
}

void testUi()
{
    UI.redraw();
}

void loop()
{
    // testWeb();
    // testEncoder();
    testUi();

    Serial.println("looping...");
    delay(10000);
}

