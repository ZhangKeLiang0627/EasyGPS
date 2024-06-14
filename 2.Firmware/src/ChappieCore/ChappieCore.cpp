#include "ChappieCore.h"

/**
 * @brief Init Chappie Core
 *
 * @param delayTime Delay ms before init
 */
void ChappieCore::begin(uint32_t delayTime)
{
    /* Hold a second */
    delay(delayTime);

    /* Init device */
    init();

    /* Init lvgl */
    lvgl.init(&Lcd, &Tp);

    /* print Logo */
    printLOGO();
    _LOG("[ChappieCore] Hi!\r\n");
    _LOG("[ChappieCore] version: %s\r\n", VERSION_SOFTWARE);
    _LOG("[ChappieCore] author: Forairaaaaa\r\n");
    _LOG("[ChappieCore] https://github.com/Forairaaaaa/Chappie-Core\r\n");
}

void ChappieCore::init()
{
    /* Init power contrl */
    Pow.init();
    _LOG("[Power] Voltage:%.2f V\r\n", Pow.readBatVoltage());
    _LOG("[Power] BatPercentage:%d\r\n", Pow.readBatPercentage());

    /* Init RGB led */
    FastLED.showColor(CRGB::Pink, 128);

    /* Init Gps */
    Gps.init();

    /* Init Serial */
    Serial.begin(115200);

#if CHAPPIE_CORE_USE_BLE
    /* Init BLE */
#endif

    /* Init lcd */
    Lcd.init();
    Lcd.setFont(&fonts::efontCN_12);
    /* Print Logo in a cool way */
    Lcd.setCursor(0, 0);
    for (char c : EasyGPS)
    {
        Lcd.printf("%c", c);
        delay(3);
    }
    Lcd.printf("\n ChappieBSP %s :)\n -Author: Forairaaaaa\n", VERSION_SOFTWARE);
    delay(100);
    Lcd.printf(" Project: %s\n -Powerby @kkl\n", FIRMWARE_NAME);
    delay(100);
    Lcd.printf(" " __DATE__ "-" __TIME__ "\n");
    delay(100);
    Lcd.printf(" Please put the equipment in a stable place!\n");
    Lcd.setTextColor(0x00c957);
    Lcd.printf(" 请把设备放在平稳的地方进行初始化!\n");

    /* Init I2C */
    Wire.begin(5, 4);
    Wire.setClock(400000);

    /* Init IMU */
    Imu.init();

    /* Init touchpad */
    Tp.init(&Wire);

    /* Init RTC */
    Rtc.begin();

    /* Init BMP280 */
    Env.init();

    /* Init qmc5883l */
    Mag.init();

    /* Init SD card */
    Sd.init();

    /* Init WiFi */
    Wf.Init();

    /* Init BLE */
#if CHAPPIE_CORE_USE_BLE
    Ble.init();
#endif

    /* Init SmartAssistant */
    saa.init(&Mic, &Speaker);

    /* Fire up */
    // Speaker.setVolume(100);
    // Speaker.tone(9000, 300);

    /* Play tone */
    // Buzz.play(Melody::StartUp);

    FastLED.showColor(CRGB::Black);
}

/**
 * @brief Print logo on serial port
 *
 */
void ChappieCore::printLOGO()
{

    /* Print out LOGO */
    _LOG(Logo.c_str());
    _LOG("\r\n");
}
