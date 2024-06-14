/**
 * @file ChappieMAG.hpp
 * @author kkl
 * @brief Reference: https://github.com/mprograms/QMC5883LCompass
 * @version 0.1
 * @date 2024-03-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <QMC5883LCompass.h>

class ChappieMAG : public QMC5883LCompass
{
private:
public:
    ChappieMAG() {}
    ~ChappieMAG() {}

    inline void init()
    {
        // initialize device
        QMC5883LCompass::init();
        QMC5883LCompass::setSmoothing(10, true);
        setMagneticDeclination(-3, 0);
        printf("[MAG] init!\r\n");
    }

    /**
     * @brief Set Mag to sleep
     *
     * @param en
     * @return none
     */
    inline void sleep(bool en)
    {
        if (!en)
            return; // en == false
        // reset device
        QMC5883LCompass::setReset();
        QMC5883LCompass::setMode(0, 0, 0, 0);
    }
};
