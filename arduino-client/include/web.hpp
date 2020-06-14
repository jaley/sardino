#ifndef WEB_H
#define WEB_H

#include <HttpClient.h>

#include "common.hpp"

namespace ArduinoClient {

const uint16_t HTTPS_PORT = 443;

class Web : public Component
{
public:
    Web(Client& client, const String& host, const String& user, const String& password)
    : Component("Web"), m_httpClient(client, host, HTTPS_PORT), 
        m_user(user), m_password(password) {}
    ~Web() {}

    void get(const String& path);

private:
    HttpClient m_httpClient;
    const String& m_user;
    const String& m_password;
};

} // namespace ArduinoClient

#endif // WEB_H