#include "BleMouse.h"
Mouse_ Mouse;

void Mouse_::begin()
{
    bleDevice.begin();
}

void Mouse_::end()
{
    bleDevice.end();
}

void Mouse_::click(uint16_t b)
{
    bleDevice.click(b);
}

void Mouse_::move(int x, int y, signed char wheel, signed char hwheel)
{
    bleDevice.move(x, y, wheel, hwheel);
}

void Mouse_::wheel(signed char wheel, signed char hwheel)
{
    bleDevice.wheel(wheel, hwheel);
}

void Mouse_::press(uint16_t b)
{
    bleDevice.pressMouse(b);
}

void Mouse_::release(uint16_t b)
{
    bleDevice.releaseMouse(b);
}

void Mouse_::releaseAll()
{
    bleDevice.release(MOUSE_ALL_ALL);
}

bool Mouse_::isPressed(uint16_t b)
{
    return bleDevice.isPressed(b);
}