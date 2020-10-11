#ifndef WEB_H
#define WEB_H

#include "common.hpp"

#include<array>
#include <HttpClient.h>


namespace ArduinoClient {

// TODO: Increase this?
const uint32_t MAX_POST_PARAMS(1);

const uint16_t HTTPS_PORT = 443;

class Web : public Component
{
public:
    Web(Client& client, const String& host, const String& user, const String& password)
    : Component("Web"), m_httpClient(client, host, HTTPS_PORT), 
        m_user(user), m_password(password) {}
    ~Web() {}

    String get(const String& path);
    String post(const String& path, const String& body);

private:
    String checkResponse();

private:
    HttpClient m_httpClient;
    const String& m_user;
    const String& m_password;
};

} // namespace ArduinoClient

#endif // WEB_H