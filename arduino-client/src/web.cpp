#include "common.hpp"
#include "web.hpp"

namespace ArduinoClient {

String Web::get(const String& path)
{
    info("GET " + path);

    m_httpClient.beginRequest();
    m_httpClient.get(path);
    m_httpClient.sendBasicAuth(m_user, m_password);
    m_httpClient.endRequest();

    const int status = m_httpClient.responseStatusCode();
    const String body = m_httpClient.responseBody();

    info(String("Response: ") + status);
    if (status != 200)
    {
        error("Proxy service returned bad status: " + body);
        return String();
    }
    
    return body;
}

String Web::post(const String& path, KeywordParam const* params, size_t numParams)
{
    return String("TODO");
}


} // namespace ArduinoClient