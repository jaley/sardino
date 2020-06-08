#ifndef ARDUINO_CLIENT_COMMON_H
#define ARDUINO_CLIENT_COMMON_H

#include <Arduino.h>

namespace ArduinoClient {

class Component {
public:
    virtual void setup();
protected:
    Component(const String name)
    : m_name(name) {}

    void info(const String& msg) const;
    void error(const String& msg) const;
private:
    void doLog(const String& level, const String& msg) const;
    const String m_name;    
};

} // namespace ArduinoClient

#endif // ARDUINO_CLIENT_COMMON_H