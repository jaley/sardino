#include "common.hpp"

namespace ArduinoClient {

const uint16_t MAX_LOG_LENGTH = 80;

const char* LEVEL_INFO = "INF";
const char* LEVEL_ERROR = "ERR";


void Component::setup()
{
    info("Initialized (no-op)");
}

void Component::doLog(const String& level, const String& msg) const {
    char out[MAX_LOG_LENGTH];
    sprintf(out, "[%s] %s: %s", level.c_str(), m_name.c_str(), msg.c_str());

    Serial.println(out);
}

void Component::info(const String& msg) const {
    doLog(LEVEL_INFO, msg);
}

void Component::error(const String& msg) const {
    doLog(LEVEL_ERROR, msg);
}

}
