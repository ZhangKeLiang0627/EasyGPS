#include "BleKeyboard.h"

Keyboard_ Keyboard;

void Keyboard_::begin()
{
    bleDevice.begin();
}

void Keyboard_::end()
{
    bleDevice.end();
}

size_t Keyboard_::write(uint8_t k)
{
    return bleDevice.write(k);
}

size_t Keyboard_::write(const MediaKeyReport m)
{
    return bleDevice.write(m);
}

size_t Keyboard_::write(const uint8_t *buffer, size_t size)
{
    return bleDevice.write(buffer, size);
}

size_t Keyboard_::press(uint8_t k)
{
    return bleDevice.press(k);
}

size_t Keyboard_::press(const MediaKeyReport m)
{
    return bleDevice.press(m);
}

size_t Keyboard_::release(uint8_t k)
{
    return bleDevice.release(k);
}

size_t Keyboard_::release(const MediaKeyReport m)
{
    return bleDevice.release(m);
}

void Keyboard_::releaseAll()
{
    bleDevice.releaseAll();
}