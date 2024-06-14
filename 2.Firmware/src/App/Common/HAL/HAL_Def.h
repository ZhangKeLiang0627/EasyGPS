#ifndef __HAL_DEF_H
#define __HAL_DEF_H

#include <stdint.h>

namespace HAL
{

    /* Clock */
    typedef struct
    {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t week;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint16_t millisecond;
    } Clock_Info_t;

    /* GPS */
    typedef struct
    {
        double longitude;
        double latitude;
        float altitude;
        float course;
        float speed;
        int16_t satellites;
        bool isVaild;
        Clock_Info_t clock;
    } GPS_Info_t;

    /* MAG */
    typedef struct
    {
        int16_t deg;
        int16_t x;
        int16_t y;
        int16_t z;
        bool isCalibrated;
    } MAG_Info_t;

    /* IMU */
    typedef struct
    {
        int16_t ax;
        int16_t ay;
        int16_t az;
        int16_t gx;
        int16_t gy;
        int16_t gz;
        int16_t steps;
    } IMU_Info_t;

    /* SportStatus */
    typedef struct
    {
        uint32_t lastTick;

        float weight;

        float speedKph;
        float speedMaxKph;
        float speedAvgKph;

        union
        {
            uint32_t totalTimeUINT32[2];
            uint64_t totalTime;
        };

        float totalDistance;

        union
        {
            uint32_t singleTimeUINT32[2];
            uint64_t singleTime;
        };

        float singleDistance;
        float singleCalorie;

    } SportStatus_Info_t;

    /* Power */
    typedef struct
    {
        uint16_t voltage;
        uint8_t usage;
        bool isCharging;
        bool isAutoPowerOff;
    } Power_Info_t;

    /* WiFi */
    typedef struct
    {
        /* pull */
        char *name;
        char *IP;
        char *gateWay;
        char *Mac;
        bool isConnected;

        /* notify */
        bool isAPEnabled;
        bool isWiFiEnabled;

    } WiFi_Info_t;

    /* BLE */
    typedef struct
    {
        /* pull */
        char *name;
        bool isConnected;

        /* notify */
        uint16_t MouseClick; // Data
        uint8_t MouseVal[4]; // Data
        uint8_t KeyVal[2];   // Data
        bool isEnabled;      // Cmd
        uint8_t DC;          // Data->0 / Cmd->1

    } BLE_Info_t;

    /* SmartAssistant */
    typedef struct
    {
        /* pull */
        char *inputText;
        char *outputText;
        bool isRunning;
        bool isListening;
        /* notify */
        // ...

    } Saa_Info_t;
}

#endif
