#ifndef WEB_H
#define WEB_H

#include "common.hpp"

#include <HttpClient.h>


namespace ArduinoClient {

const uint16_t HTTPS_PORT = 443;

struct KeywordParam {
    String parameter;
    String value;
};

class Web : public Component
{
public:
    Web(Client& client, const String& host, const String& user, const String& password)
    : Component("Web"), m_httpClient(client, host, HTTPS_PORT), 
        m_user(user), m_password(password) {}
    ~Web() {}

    String get(const String& path);

    // TODO: implement this
    String post(const String& path, KeywordParam const* params, size_t numParams);

private:
    HttpClient m_httpClient;
    const String& m_user;
    const String& m_password;
};

} // namespace ArduinoClient

#endif // WEB_H