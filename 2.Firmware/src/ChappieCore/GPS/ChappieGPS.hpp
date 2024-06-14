/**
 * @file ChappieGPS.hpp
 * @author kkl
 * @brief Reference: https://github.com/mikalhart/TinyGPSPlus
 * @version 0.1
 * @date 2023-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <Arduino.h>
#include "TinyGPS++.h"

#define GPS_SERIAL Serial1
#define DEBUG_SERIAL Serial
#define GPS_USE_TRANSPARENT 0

#define GPS_ENABLE_PIN 18

class ChappieGPS : public TinyGPSPlus
{
private:
public:
    ChappieGPS() {}
    ~ChappieGPS() {}

    inline void init()
    {
        // describe version
        printf("GPS: TinyGPS++ library v. ");
        printf(TinyGPSPlus::libraryVersion());
        printf(" by Mikal Hart\r\n");

        // en
        enable();

        delay(1000);

        // initialize
        GPS_SERIAL.begin(9600);
    }

    /* For hardware enable pin if exist */
    inline void enable() { setCtrlPin(0); }
    inline void disable() { setCtrlPin(1); }
    inline void setCtrlPin(uint8_t level)
    {
        gpio_reset_pin((gpio_num_t)GPS_ENABLE_PIN);
        gpio_set_direction((gpio_num_t)GPS_ENABLE_PIN, GPIO_MODE_OUTPUT_OD);
        gpio_set_level((gpio_num_t)GPS_ENABLE_PIN, level);
    }
};
