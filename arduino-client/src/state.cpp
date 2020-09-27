#include "state.hpp"

namespace ArduinoClient {

//
// RoomState
//

void RoomState::setVolume(int32_t volume)
{
    m_volume = volume;
    m_lastModified = millis();
}

void RoomState::commit()
{
    m_lastModified = 0;
}

const Group& RoomState::group() const
{
    return m_group;
}

int32_t RoomState::volume() const
{
    return m_volume;
}

uint64_t RoomState::lastModified() const
{
    return m_lastModified;
}

boolean RoomState::isModified() const
{
    return m_lastModified != 0;
}

//
// SystemState
//

void SystemState::refresh()
{
    info("Updating room list...");
    m_numRooms = m_sonos.getGroups(m_rooms);
    m_activeGroupIndex = 0;
    updateActiveRoom();
}

void SystemState::updateActiveRoom()
{
    const Group &activeGroup = m_rooms[m_activeGroupIndex];
    info("Retreiving room state for: " + activeGroup.m_groupName);

    uint8_t volume(m_sonos.getVolume(activeGroup.m_groupId));
    m_activeRoom = RoomState(activeGroup, volume);
}

void SystemState::nextRoom()
{
    info("Advancing to next room");
    m_activeGroupIndex = (m_activeGroupIndex + 1) % m_numRooms;
    updateActiveRoom();
}

void SystemState::apply()
{
    info("Applying volume changes for room: " + m_activeRoom.group().m_groupName);
    m_sonos.setVolume(m_activeRoom.group().m_groupId, m_activeRoom.volume());
}

RoomState& SystemState::activeRoom()
{
    return m_activeRoom;
}


}

// namespace ArduinoClient