#include "common.hpp"
#include "sonos.hpp"

#include <ArduinoJson.h>

namespace ArduinoClient {


size_t Sonos::getGroups(std::array<Group, MAX_SONOS_ROOMS> &out)
{
    info("Getting groups listing");
    const String groupsList = m_client.get("/arduino/api/groups");

    const uint16_t numFieldsPerGroup(3);
    const uint16_t groupSize(150);

    const size_t groupListSize = 
        JSON_ARRAY_SIZE(MAX_SONOS_ROOMS) + 
        MAX_SONOS_ROOMS * JSON_OBJECT_SIZE(numFieldsPerGroup) + 
        MAX_SONOS_ROOMS * groupSize;

    DynamicJsonDocument doc(groupListSize);
    deserializeJson(doc, groupsList.c_str());

    const size_t numGroupsReturned(doc.size());
    for(uint32_t i(0); i < numGroupsReturned; ++i)
    {
        const JsonObject group = doc[i];
        const char* groupId = group["group-id"];
        const char* groupName = group["group-name"];

        out[i].m_groupId = String(groupId);
        out[i].m_groupName = String(groupName);
    }

    info(String("Returned group list size: ") + numGroupsReturned);
    return numGroupsReturned;
}

uint8_t Sonos::getVolume(const String& groupId)
{
    info(String("Getting volume for group: ") + groupId);
    const String volumeResponse = m_client.get(
        String("/arduino/api/") + groupId + String("/volume")
    );

    const size_t capacity = JSON_OBJECT_SIZE(3) + 30;

    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, volumeResponse);

    uint8_t vol(0);
    if(doc.containsKey("volume"))
    {
        vol = doc["volume"];
    }

    return vol;
}

void Sonos::setVolume(const String& groupId, uint8_t volume)
{
    info(String("Setting volume for group ") + groupId);

    String body = String("volume=") + volume;
    m_client.post(String("/arduino/api/") + groupId + String("/volume"), body);
}


} // namespace ArduinoClient