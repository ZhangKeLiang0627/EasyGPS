#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

void HAL::GPS_Init()
{
    // ...
}

void HAL::GPS_Update()
{
#if CONFIG_GPS_BUF_OVERLOAD_CHK && !GPS_USE_TRANSPARENT
    int available = GPS_SERIAL.available();
    DEBUG_SERIAL.printf("GPS: Buffer available = %d", available);
    if (available >= SERIAL_RX_BUFFER_SIZE / 2)
    {
        DEBUG_SERIAL.print(", maybe overload!");
    }
    DEBUG_SERIAL.println();
#endif

    while (GPS_SERIAL.available() > 0)
    {
        char c = GPS_SERIAL.read();

        // 转发GPS的原始数据
        // printf("%c", c);

#if GPS_USE_TRANSPARENT
        DEBUG_SERIAL.write(c);
#endif
        Chappie.Gps.encode(c);
    }

    // test
    if (Chappie.Gps.location.isValid())
    {
        __IntervalExecute(FastLED.showColor(CRGB::Green), 1000);
    }
    else
    {
        __IntervalExecute(FastLED.showColor(CRGB::OrangeRed), 1000);
    }

    if (Chappie.Gps.satellites.isValid())
    {
        uint16_t satellites = Chappie.Gps.satellites.value();

        if (satellites >= 1 && satellites < 3)
        {
            __IntervalExecute(FastLED.showColor(CRGB::RosyBrown), 1000);
        }
        else if (satellites >= 3 && satellites < 5)
        {
            __IntervalExecute(FastLED.showColor(CRGB::PowderBlue), 1000);
        }
        else if (satellites >= 7)
        {
            __IntervalExecute(FastLED.showColor(CRGB::Snow), 1000);
        }
    }
    // else
    // {
    //     __IntervalExecute(FastLED.showColor(CRGB::OrangeRed), 1000);
    // }

#if GPS_USE_TRANSPARENT
    while (DEBUG_SERIAL.available() > 0)
    {
        GPS_SERIAL.write(DEBUG_SERIAL.read());
    }
#endif
}

bool HAL::GPS_GetInfo(GPS_Info_t *info)
{
    memset(info, 0, sizeof(GPS_Info_t));

    info->isVaild = Chappie.Gps.location.isValid();
    info->longitude = Chappie.Gps.location.lng();
    info->latitude = Chappie.Gps.location.lat();
    info->altitude = Chappie.Gps.altitude.meters();
    info->speed = Chappie.Gps.speed.kmph();
    info->course = Chappie.Gps.course.deg();

    info->clock.year = Chappie.Gps.date.year();
    info->clock.month = Chappie.Gps.date.month();
    info->clock.day = Chappie.Gps.date.day();
    info->clock.hour = Chappie.Gps.time.hour();
    info->clock.minute = Chappie.Gps.time.minute();
    info->clock.second = Chappie.Gps.time.second();
    info->satellites = Chappie.Gps.satellites.value();

    // info->isVaild = true;
    // info->longitude = 10;
    // info->latitude = 20;
    // info->altitude = 30;
    // info->speed = 10;
    // info->course = 10;

    // info->clock.year = 2024;
    // info->clock.month = 4;
    // info->clock.day = 1;
    // info->clock.hour = 6;
    // info->clock.minute = 6;
    // info->clock.second = 6;
    // info->satellites = 9;

    return info->isVaild;
}

bool HAL::GPS_LocationIsValid()
{
    return Chappie.Gps.location.isValid();
}

double HAL::GPS_GetDistanceOffset(GPS_Info_t *info, double preLong, double preLat)
{
    return Chappie.Gps.distanceBetween(info->latitude, info->longitude, preLat, preLong);
}