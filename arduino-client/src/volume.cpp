#include "volume.hpp"

namespace ArduinoClient {

int32_t Volume::read()
{
    // Get encoder state
    int32_t newPosition = m_encoder.read();
    int32_t delta(0);

    // If encoder has moved, calculate delta and reset cached state
    if (newPosition != m_position)
    {
        delta = m_position - newPosition;
        m_position = newPosition;
    }

    return delta;
}

} // namespace ArduinoClient