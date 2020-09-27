#ifndef ARDUINO_CLIENT_COMMON_H
#define ARDUINO_CLIENT_COMMON_H

#include <Arduino.h>

// Arduino.h redefines STL symbols... Worst
#undef max
#undef min

namespace ArduinoClient {

/**
 * Common base class to add logging to to controller components
 */
class Component
{
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

/**
 * Clamp a given value between minimum and maximum values.
 */
inline uint32_t clamp(int32_t minVal, int32_t val, int32_t maxVal)
{
    if (val < minVal)
    {
        return minVal;
    }
    else if (val > maxVal)
    {
        return maxVal;
    }

    return val;
}

// Fixing the maximum number of rooms we support globally, to avoid
// need for dynamic allocation
const size_t MAX_SONOS_ROOMS(5);


} // namespace ArduinoClient

#endif // ARDUINO_CLIENT_COMMON_H