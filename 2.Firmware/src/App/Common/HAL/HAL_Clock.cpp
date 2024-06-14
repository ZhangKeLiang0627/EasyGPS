#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

void HAL::Clock_Init()
{
    // ...
}

void HAL::Clock_GetInfo(Clock_Info_t *info)
{
    // Get RTC time
    I2C_BM8563_TimeTypeDef timeStruct;

    // Get RTC Date
    I2C_BM8563_DateTypeDef dateStruct;

    Chappie.Rtc.getTime(&timeStruct);
    Chappie.Rtc.getDate(&dateStruct);

    info->year = dateStruct.year;
    info->month = dateStruct.month;
    info->day = dateStruct.date;
    info->week = dateStruct.weekDay;
    info->hour = timeStruct.hours;
    info->minute = timeStruct.minutes;
    info->second = timeStruct.seconds;
    info->millisecond = 0;
}

void HAL::Clock_SetInfo(const Clock_Info_t *info)
{
    // Set RTC time
    I2C_BM8563_TimeTypeDef timeStruct;
    timeStruct.hours = info->hour;
    timeStruct.minutes = info->minute;
    timeStruct.seconds = info->second;
    Chappie.Rtc.setTime(&timeStruct);

    // Set RTC Date
    I2C_BM8563_DateTypeDef dateStruct;
    dateStruct.month = info->month;
    dateStruct.date = info->day;
    dateStruct.year = info->year;
    Chappie.Rtc.setDate(&dateStruct);
}

const char *HAL::Clock_GetWeekString(uint8_t week)
{
    const char *week_str[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    return week < 7 ? week_str[week] : "";
}
