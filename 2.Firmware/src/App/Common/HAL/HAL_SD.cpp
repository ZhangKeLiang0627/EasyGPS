#include "HAL.h"
#include "App/Config/Config.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

static bool SD_IsReady = false;
static uint32_t SD_CardSize = 0;

static HAL::SD_CallbackFunction_t SD_EventCallback = nullptr;

/*
 * User provided date time callback function.
 * See SdFile::dateTimeCallback() for usage.
 */
static void SD_GetDateTime(uint16_t *date, uint16_t *time)
{
    // User gets date and time from GPS or real-time
    // clock in real callback function
    HAL::Clock_Info_t clock;
    HAL::Clock_GetInfo(&clock);

    // return date using FAT_DATE macro to format fields
    // *date = FAT_DATE(clock.year, clock.month, clock.day);

    // return time using FAT_TIME macro to format fields
    // *time = FAT_TIME(clock.hour, clock.minute, clock.second);
}

static bool SD_CheckDir(const char *path)
{
    bool retval = true;
    if (!SD.exists(path))
    {
        printf("SD: Auto create path \"%s\"...", path);
        retval = SD.mkdir(path);
        printf(retval ? "success" : "failed");
    }
    return retval;
}

bool HAL::SD_Init()
{
    // ...
    bool retval = true;

    retval = Chappie.Sd.init();
    
    return retval;
}

bool HAL::SD_GetReady()
{
    SD_IsReady = Chappie.Sd.isInited();

    return SD_IsReady;
}

float HAL::SD_GetCardSizeMB()
{
#define CONV_MB(size) (size / 1048576.0f)
    return CONV_MB(SD.cardSize());
}

const char *HAL::SD_GetTypeName()
{
    const char *type = "Unknown";

    SD_CardSize = SD.cardSize();

    if (!SD_CardSize)
    {
        goto failed;
    }

    switch (SD.cardType())
    {
    case CARD_MMC:
        type = "SD1";
        break;

    case CARD_SD:
        type = "SD2";
        break;

    case CARD_SDHC:
        type = (SD_CardSize < 70000000) ? "SDHC" : "SDXC";
        break;

    default:
        break;
    }

failed:
    return type;
}

static void SD_Check(bool isInsert)
{
    if (isInsert)
    {
        bool ret = HAL::SD_Init();

        if (ret && SD_EventCallback)
        {
            SD_EventCallback(true);
        }

        HAL::Audio_PlayMusic(ret ? "DeviceInsert" : "Error");
    }
    else
    {
        SD_IsReady = false;

        if (SD_EventCallback)
        {
            SD_EventCallback(false);
            SD_CardSize = 0;
        }

        HAL::Audio_PlayMusic("DevicePullout");
    }
}

void HAL::SD_SetEventCallback(SD_CallbackFunction_t callback)
{
    SD_EventCallback = callback;
}

void HAL::SD_Update()
{
    // bool isInsert = (digitalRead(CONFIG_SD_CD_PIN) == LOW);
    bool isInsert = 0;
    __ValueMonitor(isInsert, SD_Check(isInsert));
}
