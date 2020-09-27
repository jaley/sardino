#ifndef STATE_H
#define STATE_H

#include "common.hpp"
#include "sonos.hpp"

namespace ArduinoClient {

/**
 * Controller state, shared among components.
 */
class RoomState
{
public:
    RoomState()
    : m_group(Group()), m_volume(0), m_updatingNow(false), m_lastModified(0) {}
    RoomState(const Group &room, uint32_t initialVolume)
    : m_group(room), m_volume(initialVolume), m_updatingNow(false), m_lastModified(0) {}

    void setVolume(int32_t volume);
    void commit();

    const Group& group() const;
    int32_t volume() const;
    uint64_t lastModified() const;
    boolean isModified() const;


private:
    Group m_group;
    int32_t m_volume;
    boolean m_updatingNow;
    uint64_t m_lastModified;
};

/**
 * Represents state of Sonos system across all rooms (groups)
 */
class SystemState : public Component
{
public:
    SystemState(Sonos sonosApi)
    : Component("State"), m_sonos(sonosApi), m_numRooms(0), m_activeGroupIndex(0) {}

    void refresh();
    void nextRoom();
    void apply();

    RoomState& activeRoom();

private:
    void updateActiveRoom();

private:
    Sonos m_sonos;
    std::array<Group, MAX_SONOS_ROOMS> m_rooms;
    size_t m_numRooms;
    uint32_t m_activeGroupIndex;
    RoomState m_activeRoom;
};

} // namespace ArduinoClient

#endif // STATE_H