#ifndef SONOS_H
#define SONOS_H

#include "common.hpp"

namespace ArduinoClient {

struct Group {
    String m_groupName;
    String m_groupId;
};

class Sonos : public Component
{
public:
    Sonos(
        const String& proxyHost, 
        const String& proxyCertPubKey, 
        const String& proxyUser, 
        const String& proxyPassword
    ) : Component("Sonos"), 
        m_proxyHost(proxyHost), m_proxyCertPubKey(proxyCertPubKey), 
        m_proxyUser(proxyUser), m_proxyPassword(proxyPassword) {}
    ~Sonos() {}

    void getGroups(Group** out, size_t numGroups);
    uint8_t getVolume(const String& groupId);
    void setVolume(const String& groupId, uint8_t volume);


private:
    const String& m_proxyHost;
    const String& m_proxyCertPubKey;
    const String& m_proxyUser;
    const String& m_proxyPassword;
};

} // namespace ArduinoClient

#endif // SONOS_H