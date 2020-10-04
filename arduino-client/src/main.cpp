#include "common.hpp"

#include <Bounce2.h>

// Not checked in - provides ArduinoClient::Secrets
#include "secrets.h"

#include "sonos.hpp"
#include "state.hpp"
#include "ui.hpp"
#include "volume.hpp"
#include "web.hpp"
#include "wifi.hpp"

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

// Room change control
Button ROOM_TOGGLE = Button();

// Display
U8G2_SH1107_PIMORONI_128X128_F_4W_HW_SPI U8G2_DISPLAY(U8G2_R0, A1, A2, U8X8_PIN_NONE);
Ui UI(U8G2_DISPLAY);

// System state
Web WEB(
    WIFI.client(),
    Secrets::PROXY_HOSTNAME,
    Secrets::PROXY_AUTH_USER,
    Secrets::PROXY_AUTH_PASS
);
Sonos SONOS(WEB);
SystemState STATE(SONOS);

/**
 * Helper to render a status message on the display
 */
void message(String title, String msg)
{
    const FullScreenMessage message(title, msg);
    UI.setDrawable(&message);
    UI.redraw();
}

/**
 * Read current system state and redraw UI to reflect that
 */
void redraw()
{
    const ControllerStateDrawable newState(STATE.activeRoom());
    UI.setDrawable(&newState);
    UI.redraw();
}

void setup()
{
    // Init and wait for serial
    Serial.begin(9600);
    while (!Serial);

    U8G2_DISPLAY.begin();

    // Connect to WiFi
    message("Connecting", "WiFi...");
    WIFI.connectAndWait();

    // Retrieve current room listings
    message("Connecting", "Sonos...");
    STATE.refresh();

    // Connect room toggle button
    ROOM_TOGGLE.attach(A3, INPUT_PULLUP);
    ROOM_TOGGLE.interval(2);
    ROOM_TOGGLE.setPressedState(LOW);

    // Render home state
    redraw();
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
    // sonos.getGroups(groups, 5);

    Serial.println(String("Got groups: ") + groups[0].m_groupName);
}

void testEncoder()
{
    uint32_t newVolume = clamp(0, STATE.activeRoom().volume() + VOLUME.read(), 100);
    STATE.activeRoom().setVolume(newVolume);
}

void loop()
{
    // Update room control if button pressed
    ROOM_TOGGLE.update();
    if(ROOM_TOGGLE.pressed())
    {
        STATE.nextRoom();
    }

    // testWeb();
    testEncoder();
    redraw();

    // delay(10);
}
