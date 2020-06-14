#include "web.hpp"

namespace ArduinoClient {

void Web::get(const String& path)
{
    info("GET " + path);

    m_httpClient.beginRequest();
    m_httpClient.get(path);
    m_httpClient.sendBasicAuth(m_user, m_password);
    m_httpClient.endRequest();

    info(String("Response: ") + m_httpClient.responseStatusCode() + 
         String(" - ") + m_httpClient.responseBody());
}

} // namespace ArduinoClient