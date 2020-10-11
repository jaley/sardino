# Arduino Sonos Controller

This is an unnecessarily elaborate solution to a very minor problem: needing to unlock my phone to turn up the TV volume.

I have a few Sonos smart speakers, including a Playbar, which is connected to my TV via optical cable. That disables the TV volume controls, and the idea is that you configure the Playbar to intercept the IR volume control signals from your TV remote. My TV remote isn't infrared, however, it uses some kind of radio wireless protocol, so that doesn't work.

That means I need to find my phone, unlock it and open the Sonos app to adjust the TV volume, which was getting annoying. This repo is a project aimed at putting a volume control knob on my coffee table.

## Hardware

The physical remote hardware used consists of:

* Arduino MKR 1010 WiFi
* Rotary Encoder (with push button)
* Pimoroni Monochrome OLED display
* LiPo battery

## Arduino Code

I used Platform IO to get a repeatable Arduino build and dependency management. There's some very unidiomatic C++ code in the `arduino-client` folder.

Note that `secrets.h` isn't checked in, as that's how I've deployed the WiFi credentials and proxy service shared secrets. It needs adding to the include path with the symbols in the `ArduinoClient::Secrets` namespace defined for this code to build.

## Proxy Service

The official Sonos APIs very sensible use OAuth2 authentication flow to enable you to connect devices and software to your home setup. Little bit ambitious for an electronics project, so I decided to do that flow in a web app and authenticate my Arduino controller using a shared secret instead.

The web app is a simple Clojure ring/http-kit based service deployed in a Docker container to Azure App Service. It's under the `sonos-proxy` folder.

