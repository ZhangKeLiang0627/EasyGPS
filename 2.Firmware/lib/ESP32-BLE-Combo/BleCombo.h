#ifndef ESP32_BLE_KEYBOARD_H
#define ESP32_BLE_KEYBOARD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#if defined(USE_NIMBLE)

#include "NimBLECharacteristic.h"
#include "NimBLEHIDDevice.h"

#define BLEDevice NimBLEDevice
#define BLEServerCallbacks NimBLEServerCallbacks
#define BLECharacteristicCallbacks NimBLECharacteristicCallbacks
#define BLEHIDDevice NimBLEHIDDevice
#define BLECharacteristic NimBLECharacteristic
#define BLEAdvertising NimBLEAdvertising
#define BLEServer NimBLEServer

#else

#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"
#include "Print.h"

#endif // USE_NIMBLE

#define BLE_KEYBOARD_VERSION "0.0.4"
#define BLE_KEYBOARD_VERSION_MAJOR 0
#define BLE_KEYBOARD_VERSION_MINOR 0
#define BLE_KEYBOARD_VERSION_REVISION 4

typedef uint8_t MediaKeyReport[2];

const MediaKeyReport KEY_MEDIA_NEXT_TRACK = {1, 0};
const MediaKeyReport KEY_MEDIA_PREVIOUS_TRACK = {2, 0};
const MediaKeyReport KEY_MEDIA_STOP = {4, 0};
const MediaKeyReport KEY_MEDIA_PLAY_PAUSE = {8, 0};
const MediaKeyReport KEY_MEDIA_MUTE = {16, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};
const MediaKeyReport KEY_MEDIA_WWW_HOME = {128, 0};
const MediaKeyReport KEY_MEDIA_LOCAL_MACHINE_BROWSER = {0, 1}; // Opens "My Computer" on Windows
const MediaKeyReport KEY_MEDIA_CALCULATOR = {0, 2};
const MediaKeyReport KEY_MEDIA_WWW_BOOKMARKS = {0, 4};
const MediaKeyReport KEY_MEDIA_WWW_SEARCH = {0, 8};
const MediaKeyReport KEY_MEDIA_WWW_STOP = {0, 16};
const MediaKeyReport KEY_MEDIA_WWW_BACK = {0, 32};
const MediaKeyReport KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION = {0, 64}; // Media Selection
const MediaKeyReport KEY_MEDIA_EMAIL_READER = {0, 128};

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class Mouse_;
class Keyboard_;
class Gamepad_;

class BleCombo : public Print, public BLEServerCallbacks, public BLECharacteristicCallbacks
{
  friend class Mouse_;
  friend class Keyboard_;
  friend class Gamepad_;

private:
  uint8_t _buttons;
  uint8_t _buttonsGamepad[16]; // 2x64 -> 128 bytes) [0: 0-64, 1->64-128]
  BLEHIDDevice *hid;
  BLECharacteristic *inputKeyboard;
  BLECharacteristic *outputKeyboard;
  BLECharacteristic *inputMediaKeys;
  BLECharacteristic *inputMouse;
  BLECharacteristic *outputMouse;
  BLECharacteristic *inputGamepad;
  BLECharacteristic *outputGamepad;

  BLEAdvertising *advertising;
  KeyReport _keyReport;
  MediaKeyReport _mediaKeyReport;
  std::string deviceName;
  std::string deviceManufacturer;
  uint8_t batteryLevel;
  bool connected = false;
  uint32_t _delay_ms = 7;
  void delay_ms(uint64_t ms);
  void buttons(const uint16_t b);

  uint16_t vid = 0x05ac;
  uint16_t pid = 0x820a;
  uint16_t version = 0x0210;

  static bool isInitialized;

public:
  BleCombo(std::string deviceName = "BL-Combo (KMG)", std::string deviceManufacturer = "JMCResearch.com", uint8_t batteryLevel = 100);
  void setBatteryLevel(uint8_t level);
  void setName(std::string deviceName);
  void setDelay(uint32_t ms);
  bool isConnected(void);

protected:
  void begin(void);
  void end(void);
  void sendReport(KeyReport *keys);
  void sendReport(MediaKeyReport *keys);
  size_t press(uint8_t k);
  size_t press(const MediaKeyReport k);
  size_t pressMouse(const uint16_t b);
  size_t release(uint8_t k);
  size_t release(const MediaKeyReport k);
  size_t releaseMouse(const uint16_t b);
  size_t write(uint8_t c);
  size_t write(const MediaKeyReport c);
  size_t write(const uint8_t *buffer, size_t size);
  void releaseAll(void);

  // keypad magic
  void resetButtons();
  void setAxes(int16_t x, int16_t y, int16_t a1, int16_t a2, int16_t a3, int16_t a4, int16_t a5, int16_t a6, signed char hat1, signed char hat2, signed char hat3, signed char hat4);
  size_t pressButton(uint8_t b);
  size_t releaseButton(uint8_t b);
  bool isPressedButton(uint8_t b);

  void click(const uint16_t b);
  void move(signed char x, signed char y, signed char wheel = 0, signed char hWheel = 0);
  void wheel(signed char wheel = 0, signed char hWheel = 0);
  bool isPressed(const uint16_t b);

  void set_vendor_id(uint16_t vid);
  void set_product_id(uint16_t pid);
  void set_version(uint16_t version);
  virtual void onStarted(BLEServer *pServer){};
  virtual void onConnect(BLEServer *pServer) override;
  virtual void onDisconnect(BLEServer *pServer) override;
  virtual void onWrite(BLECharacteristic *me) override;
};

extern BleCombo bleDevice;
#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_KEYBOARD_H
