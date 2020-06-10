#include "sonos.hpp"

namespace ArduinoClient {

void Sonos::getGroups(Group** out, size_t numGroups)
{
    info("Getting groups listing");
}

uint8_t Sonos::getVolume(const String& groupId)
{
    info("Getting volume for group: " + groupId);
    return 0;
}

void Sonos::setVolume(const String& groupId, uint8_t volume)
{
    info("Setting volume for group " + groupId + "to level: " + volume);
}


} // namespace ArduinoClient