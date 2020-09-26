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