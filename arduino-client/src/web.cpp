#include "common.hpp"
#include "web.hpp"

namespace ArduinoClient {

const String FORM_CONTENT_TYPE("application/x-www-form-urlencoded");

String Web::checkResponse()
{
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

String Web::get(const String& path)
{
    info("GET " + path);

    m_httpClient.beginRequest();
    m_httpClient.get(path);
    m_httpClient.sendBasicAuth(m_user, m_password);
    m_httpClient.endRequest();

    return checkResponse();
}

String Web::post(const String& path, const String &body)
{
    info(String("POST ") + path);

    m_httpClient.beginRequest();
    m_httpClient.post(path);
    m_httpClient.sendBasicAuth(m_user, m_password);
    m_httpClient.sendHeader("Content-Type", FORM_CONTENT_TYPE);
    m_httpClient.sendHeader("Content-Length", body.length());
    m_httpClient.print(body);
    m_httpClient.endRequest();

    return checkResponse();
}


} // namespace ArduinoClient