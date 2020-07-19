#ifndef SONOS_H
#define SONOS_H

#include "common.hpp"
#include "web.hpp"

namespace ArduinoClient {

struct Group {
    String m_groupName;
    String m_groupId;
};

class Sonos : public Component
{
public:
    Sonos(Web& client) 
    : Component("Sonos"), m_client(client) {}
    ~Sonos() {}

    size_t getGroups(Group* out, size_t numGroups);
    uint8_t getVolume(const String& groupId);
    void setVolume(const String& groupId, uint8_t volume);


private:
    Web& m_client;
};

} // namespace ArduinoClient

#endif // SONOS_H