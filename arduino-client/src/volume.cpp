#include "volume.hpp"

namespace ArduinoClient {

// TODO: Make a better smoothing function for encoder deltas
const float DELTA_SMOOTHING = 0.5f;

int32_t smooth(int32_t delta)
{
    return (int32_t) delta * DELTA_SMOOTHING;
}

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

    return smooth(delta);
}

} // namespace ArduinoClient