#include "volume.hpp"

namespace ArduinoClient {

int32_t Volume::read()
{
    // Get encoder states
    int32_t newLeft, newRight;
    newLeft = m_left.read();
    newRight = m_right.read();

    int32_t delta(0);

    info("New State: " + String(newLeft) + ", " + String(newRight));

    // If encoder has moved, calculate delta and reset cached state
    if (newLeft != m_positionLeft || newRight != m_positionRight)
    {
        m_positionLeft = newLeft;
        m_positionRight = newRight;
        delta = newRight - newLeft;

        info("Encoder state: " + String(m_positionLeft) + ", " + String(m_positionRight));
        info("Delta: " + String(delta));
    }

    return delta;
}

} // namespace ArduinoClient