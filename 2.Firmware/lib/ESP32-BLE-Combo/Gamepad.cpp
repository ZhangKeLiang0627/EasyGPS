#include "BleGamepad.h"
Gamepad_ Gamepad;

void Gamepad_::resetButtons() {
  bleDevice.resetButtons();
}


void Gamepad_::begin(void)
{
  bleDevice.begin();
}

void Gamepad_::end(void)
{
  bleDevice.end();
}

void Gamepad_::setAxes(int16_t x, int16_t y, int16_t a1, int16_t a2, int16_t a3, int16_t a4, int16_t a5, int16_t a6, signed char hat1, signed char hat2, signed char hat3, signed char hat4)
{
  bleDevice.setAxes(x,y,a1,a2,a3,a4,a5,a6,hat1,hat2,hat3,hat4);
}

// indexed button (1..128)
void Gamepad_::press(uint8_t b)
{
  bleDevice.pressButton(b);

}

// indexed button (1..128)
void Gamepad_::release(uint8_t b)
{
  bleDevice.releaseButton(b);
}

// indexed button (1..128)
bool Gamepad_::isPressed(uint8_t b)
{
  return bleDevice.isPressedButton(b);
}

