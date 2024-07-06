/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  Keyboard.begin();
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
  }

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}
