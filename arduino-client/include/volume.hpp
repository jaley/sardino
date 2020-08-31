#ifndef VOLUME_H
#define VOLUME_H

#include "common.hpp"

#include <Encoder.h>

namespace ArduinoClient {

const int32_t POSITION_UNSET(-999);

class Volume : public Component
{
public:
    Volume(uint32_t encoderPinA, uint32_t encoderPinB)
    : Component("Volume"), 
        m_left(Encoder(encoderPinA, encoderPinB)),
        m_right(Encoder(encoderPinB, encoderPinA)),
        m_positionLeft(POSITION_UNSET),
        m_positionRight(POSITION_UNSET) {}
    ~Volume() {}

    int32_t read();

private:
    Encoder m_left, m_right;
    int32_t m_positionLeft, m_positionRight;
};


} // namespace ArduinoClient

#endif // VOLUME_H