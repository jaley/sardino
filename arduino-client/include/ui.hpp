#ifndef UI_H
#define UI_H

#include "common.hpp"
#include "state.hpp"

#include <U8g2lib.h>
#include <SPI.h>

namespace ArduinoClient {

/**
 * Interface for objects that know how to render themselves on the display
 */
class IDrawable
{
public:
    virtual void draw(U8G2 &display) const = 0;
};

/**
 * A text alert that takes up the whole display
 */
class FullScreenMessage : public IDrawable
{
public:
    FullScreenMessage(String title, String message)
    : m_title(title), m_message(message) {}
    ~FullScreenMessage() {}

    void draw(U8G2 &display) const;

private:
    String m_title;
    String m_message;
};

class ControllerStateDrawable : public IDrawable
{
public:
    ControllerStateDrawable(const RoomState &state)
    : m_state(state) {}

    void draw(U8G2 &display) const;

private:
    void spinner(U8G2 &display) const;

private:
    const RoomState m_state;
};

// Unused drawable state
const FullScreenMessage BLANK("", "");

/**
 * Holds current UI state and redraws when called by main loop
 */
class Ui : public Component
{
public:
    Ui(U8G2 &display)
    : Component("UI"), m_state(&BLANK), m_display(display) {}
    ~Ui() {}

    void setDrawable(const IDrawable *state);
    void redraw();

private:
    const IDrawable* m_state;
    U8G2 m_display;
};


} // namespace ArduinoClient



#endif // UI_H