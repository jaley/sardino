#include "ui.hpp"

namespace ArduinoClient {

/*
    # Display limits
    (0,0) top-left
    (128, 128) bottom-right
*/

void FullScreenMessage::draw(U8G2 &display) const
{
    display.setFont(u8g2_font_logisoso20_tf);
    display.drawStr(4, 40, m_title.c_str());

    display.setFont(u8g2_font_logisoso16_tf);
    display.drawStr(4, 64, m_message.c_str());
}

void ControllerStateDrawable::draw(U8G2 &display) const
{
    display.setFont(u8g2_font_logisoso18_tf);
    display.drawStr(4, 32, m_state.group().m_groupName.c_str());

    display.setFont(u8g2_font_logisoso16_tf);
    String volume(String("Vol: ") + m_state.volume() + String("%"));
    display.drawStr(4, 96, volume.c_str());

    if (m_state.isModified())
    {
        spinner(display);
    }
}

void ControllerStateDrawable::spinner(U8G2 &display) const
{
    const uint8_t spinnerOriginX = 102;
    const uint8_t spinnerOriginY = 122;
    const uint8_t spinnerRadius = 4;
    const uint8_t spinnerNumDots = 3;
    const uint8_t spinnerPad = 2;
    const uint32_t spinnerDurationMs = 1000;

    uint32_t elapsedNow = millis() % spinnerDurationMs;
    uint8_t phase = (elapsedNow * spinnerNumDots) / spinnerDurationMs;

    for (uint8_t i(0); i < spinnerNumDots; ++i)
    {
        uint8_t x = spinnerOriginX + (i * spinnerPad) + (i * 2 * spinnerRadius);

        if (i == phase)
        {
            display.drawDisc(x, spinnerOriginY, spinnerRadius);
        }
        else
        {
            display.drawCircle(x, spinnerOriginY, spinnerRadius);
        }
    }
}

void Ui::setDrawable(const IDrawable *state)
{
    m_state = state;
}

void Ui::redraw()
{
    if (m_state)
    {
        m_display.clearBuffer();
        m_state->draw(m_display);
        m_display.sendBuffer();
    }
}

} // namespace ArduinoClient