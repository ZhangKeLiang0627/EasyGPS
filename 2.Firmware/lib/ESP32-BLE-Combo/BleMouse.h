#include <BleCombo.h>

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_BACK 8
#define MOUSE_FORWARD 16
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)
#define MOUSE_ALL_ALL (MOUSE_ALL | MOUSE_BACK | MOUSE_FORWARD)

class Mouse_
{
public:
    void begin(void);
    void end(void);
    void click(uint16_t b = MOUSE_LEFT);
    void move(int x, int y, signed char wheel = 0, signed char hwheel = 0);
    void wheel(signed char wheel = 0, signed char hwheel = 0);
    void press(uint16_t b = MOUSE_LEFT);
    void release(uint16_t b = MOUSE_LEFT);
    void releaseAll(void);
    bool isPressed(uint16_t b = MOUSE_LEFT);
};

extern Mouse_ Mouse;
