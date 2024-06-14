#ifndef _CHAPPIECORE_H_
#define _CHAPPIECORE_H_

/* Core */
#include <Arduino.h>
#include "Wire.h"

/* Part */
#include "CTP/ChappieCTP.hpp"
#include "IMU/ChappieIMU.hpp"
#include "Power/ChappiePower.hpp"
#include "SD/ChappieSD.hpp"
#include "ENV/ChappieENV.hpp"
#include "Utility/BM8563/I2C_BM8563.h"
#include "Lvgl/ChappieLvgl.hpp"
#include "Buzzer/EmmaBuzzer.hpp"
#include "Button/EmmaButton.hpp"
#include "Mic/EmmaMicPDM.hpp"
#include "Speaker/EmmaSpeaker.hpp"
#include "RGBLED/EmmaRGBLED.hpp"
#include "MAG/ChappieMAG.hpp"
#include "WiFi/ConfigWiFi.h"
#include "BLE/ChappieBLE.hpp"
#include "GPS/ChappieGPS.hpp"
#include "SmartAssistantAPI/SmartAssistantAPI.hpp"

/* Config */
#include "ChappieCore_config.h"

/* ChappieCore class */
class ChappieCore
{
private:
public:
    /* Basic functions */
    void begin(uint32_t delayTime = 100);

    void init();

    void printLOGO();

public:
    const std::string Logo = R"(
  ___ _  _  __  ____ ____ __ ____ 
 / __) )( \/ _\(  _ (  _ (  |  __)
( (__) __ (    \) __/) __/)( ) _) 
 \___)_)(_|_/\_(__) (__) (__|____)
)";

    const std::string EasyGPS = R"(
  ____  __  ___  _  _  ____  ____ ___
 |  __)/ _\( __\( \/ )/ ___)(  _ ( __\
  ) _)/ /_)_\ \  \  /( (_/ | ) __/\ \
 |____)_/\_\___/ (__) \____/(__) \___/
)";

    LGFX_ChappieCore Lcd;

    ChappieCTP Tp;

    ChappieIMU Imu;

    ChappieSD Sd;

    ChappiePower Pow;

    ChappieLvgl lvgl;

    I2C_BM8563 Rtc = I2C_BM8563(I2C_BM8563_DEFAULT_ADDRESS, Wire);

    ChappieENV Env;

    ChappieMAG Mag;

    ChappieGPS Gps;

    EmmaButton Button;

    EmmaSpeaker Speaker;

    EmmaMicPDM Mic;

    EmmaBuzzer Buzz;

    EmmaRGBLED RGBLed = EmmaRGBLED();

    // Net --WiFi --Bluet
    ConfigWiFi Wf;

    SmartAssistantAPI saa;

#if CHAPPIE_CORE_USE_BLE
    ChappieBLE Ble;
#endif
};

/**
 * @brief Define the way to format logout
 *
 */
#define _LOG(format, args...) printf(format, ##args)

#endif