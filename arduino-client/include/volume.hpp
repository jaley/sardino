#ifndef VOLUME_H
#define VOLUME_H

#include "common.hpp"

#include <Encoder.h>

namespace ArduinoClient {

const int32_t POSITION_UNSET(0);

class Volume : public Component
{
public:
    Volume(uint32_t encoderPinA, uint32_t encoderPinB)
    : Component("Volume"), 
        m_encoder(Encoder(encoderPinA, encoderPinB)),
        m_position(POSITION_UNSET) {}
    ~Volume() {}

    int32_t read();

private:
    Encoder m_encoder;
    int32_t m_position;
};


} // namespace ArduinoClient

#endif // VOLUME_H