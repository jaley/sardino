#ifndef WEB_H
#define WEB_H

#include "common.hpp"

#include <HttpClient.h>


namespace ArduinoClient {

const uint16_t HTTPS_PORT = 443;

class KeywordParam {
public:
    KeywordParam(String key, String val)
    : parameter(key), value(val) {}
    ~KeywordParam() {}

public:
    const String parameter;
    const String value;
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