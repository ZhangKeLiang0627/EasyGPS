/**
 * @file ChappiePower.hpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-03-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <Arduino.h>

#define CHAPPIE_PWR_CTRL GPIO_NUM_9
#define CHAPPIE_BATM_ADC GPIO_NUM_8
#define CHAPPIE_BATM_EN GPIO_NUM_18

class ChappiePower
{
private:
    inline void _enableBatMeasure() { digitalWrite(CHAPPIE_BATM_EN, 0); }
    inline void _disableBatMeasure() { digitalWrite(CHAPPIE_BATM_EN, 1); }

public:
    ChappiePower() {}
    ~ChappiePower() {}

    inline void init()
    {
        gpio_reset_pin(CHAPPIE_BATM_ADC);
        pinMode(CHAPPIE_BATM_ADC, INPUT);

        analogReadResolution(12);
    }

    inline void powerOff()
    {
        // gpio_reset_pin(CHAPPIE_PWR_CTRL);
        // gpio_set_direction(CHAPPIE_PWR_CTRL, GPIO_MODE_OUTPUT_OD);
        // /* Double click */
        // gpio_set_level(CHAPPIE_PWR_CTRL, 0);
        // delay(100);
        // gpio_set_level(CHAPPIE_PWR_CTRL, 1);
        // delay(100);
        // gpio_set_level(CHAPPIE_PWR_CTRL, 0);
        // delay(100);
        // gpio_set_level(CHAPPIE_PWR_CTRL, 1);
        // delay(1000);
    }

    inline float readBatVoltage()
    {
        /* Calculate the real bat voltage */
        float voltage = (float)map(readBatMilliVoltRaw(), 0, 4096, 0, 3300);
        // printf("voltage1:%.2f ", voltage);

        voltage = voltage * 0.00144; // 1 / 680.0 * 980.0 / 1000.0
        // printf("voltage2:%.2f\r\n", voltage);

        return voltage;
    }

    inline uint32_t readBatMilliVoltRaw()
    {
        uint32_t ret = analogRead(CHAPPIE_BATM_ADC);

        return ret;
    }

    inline uint8_t readBatPercentage()
    {
        /* Asume 0~100% to be 3.2~4.2V */
        uint8_t ret = map((long)(readBatVoltage() * 1000), 3200, 4200, 0, 100);

        return ret;
    }
};
