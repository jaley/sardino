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
 * Controller state, shared among components.
 */
class ControllerState
{
public:
    ControllerState(String room, uint32_t volume, boolean updating)
    : m_currentRoom(room), m_volume(volume), m_updatingNow(updating) {}
    ~ControllerState() {}

    inline void setCurrentRoom(String room)
    {
        m_currentRoom = room;
    }

    inline void setVolume(uint32_t volume)
    {
        m_volume = volume;
    }

    inline void setUpdatingNow(boolean updating)
    {
        m_updatingNow = updating;
    }

    inline String currentRoom() const
    {
        return m_currentRoom;
    }

    inline uint32_t volume() const
    {
        return m_volume;
    }

    inline boolean isUpdating() const
    {
        return m_updatingNow;
    }

private:
    String m_currentRoom;
    uint32_t m_volume;
    boolean m_updatingNow;
};

} // namespace ArduinoClient

#endif // ARDUINO_CLIENT_COMMON_H