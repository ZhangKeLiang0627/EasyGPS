/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <BleGamepad.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  Keyboard.begin();
  Mouse.begin();
  Gamepad.begin();
}

void loop() {
  if(bleDevice.isConnected()) {
    Serial.println("Sending 'Hello world'...");
    Keyboard.print("Hello world");

    delay(1000);

    Serial.println("Sending Enter key...");
    Keyboard.write(KEY_RETURN);

    delay(1000);

    Serial.println("Sending Play/Pause media key...");
    Keyboard.write(KEY_MEDIA_PLAY_PAUSE);

    delay(1000);
  
    Serial.println("Waiting 5 seconds...");
    delay(5000);

    Serial.println("Left click");
    Mouse.click(MOUSE_LEFT);
    delay(500);

    Serial.println("Right click");
    Mouse.click(MOUSE_RIGHT);
    delay(500);

    Serial.println("Scroll wheel click");
    Mouse.click(MOUSE_MIDDLE);
    delay(500);

    Serial.println("Back button click");
    Mouse.click(MOUSE_BACK);
    delay(500);

    Serial.println("Forward button click");
    Mouse.click(MOUSE_FORWARD);
    delay(500);

    Serial.println("Click left+right mouse button at the same time");
    Mouse.click(MOUSE_LEFT | MOUSE_RIGHT);
    delay(500);

    Serial.println("Click left+right mouse button and scroll wheel at the same time");
    Mouse.click(MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE);
    delay(500);  

    Serial.println("Waiting 5 seconds...");
    delay(5000);

    Serial.println("Sending some buttons");
    Gamepad.press(12);
    delay(500);
    Gamepad.press(10);
    delay(500);
    Gamepad.setAxes(0, 0, 512, 512, 512, 512, 0, 0, 0, 0,0,0);
    delay(500);

  }

}
