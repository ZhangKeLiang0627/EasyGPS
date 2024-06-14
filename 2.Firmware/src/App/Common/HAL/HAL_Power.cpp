#include "HAL.h"
#include "ChappieCore/ChappieCore.h"
#include "App/App.h"


extern ChappieCore Chappie;

/* 上一次操作时间(ms) */
static uint32_t Power_LastHandleTime = 0;

/* 自动关机时间(秒) */
static uint16_t Power_AutoLowPowerTimeout = 60;

/* 自动关机功能使能 */
static bool Power_AutoLowPowerEnable = false;

static bool Power_IsShutdown = false;

static volatile uint16_t Power_ADCValue = 0;

static HAL::Power_CallbackFunction_t Power_EventCallback = nullptr;

#define BATT_MAX_VOLTAGE 4200
#define BATT_MIN_VOLTAGE 3300
#define BATT_FULL_CHARGE_VOLTAGE 4100
/**
 * @brief  电源初始化
 * @param  无
 * @retval 无
 */
void HAL::Power_Init()
{
    /* 设置自动关机 */
    Power_SetAutoLowPowerTimeout(60);
    Power_HandleTimeUpdate();
    Power_SetAutoLowPowerEnable(true);
}

static void Power_ADC_TrigUpdate()
{
    // Power_ADCValue = Chappie.Pow.readBatMilliVoltRaw();
    // _LOG("[Power] VoltageRaw: %d\r\n", Chappie.Pow.readBatMilliVoltRaw());
    _LOG("[Power] Voltage: %.2f V\r\n", Chappie.Pow.readBatVoltage());
    // _LOG("[Power] Percentage: %d\r\n", Chappie.Pow.readBatPercentage());
}

/**
 * @brief  更新操作时间
 * @param  无
 * @retval 无
 */
void HAL::Power_HandleTimeUpdate()
{
    Power_LastHandleTime = millis();
}

/**
 * @brief  设置自动关机时间
 * @param  sec:时间(秒)
 * @retval 无
 */
void HAL::Power_SetAutoLowPowerTimeout(uint16_t sec)
{
    Power_AutoLowPowerTimeout = sec;
}

/**
 * @brief  获取自动关机时间
 * @param  无
 * @retval sec:时间(秒)
 */
uint16_t HAL::Power_GetAutoLowPowerTimeout()
{
    return Power_AutoLowPowerTimeout;
}

/**
 * @brief  设置自动关机功能使能
 * @param  en:使能
 * @retval 无
 */
void HAL::Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

/**
 * @brief  执行关机
 * @param  无
 * @retval 无
 */
void HAL::Power_Shutdown()
{
    Backlight_SetGradual(10, 500);

    /* 关机反悔 */
    for (uint8_t i = 0; i < 50; i++)
    {
        if (Chappie.Tp.isTouched())
        {
            Power_HandleTimeUpdate();
            break;
        }
        delay(100);
    }

    if (millis() - Power_LastHandleTime < (Power_AutoLowPowerTimeout * 1000))
    {
        Backlight_SetGradual(127, 500);
        return;
    }

    /* 执行关机 */

    App_Uninit();

    Chappie.Gps.disable();

    Chappie.Imu.sleep(true);
    Chappie.Env.sleep(true);
    Chappie.Mag.sleep(true);

    Chappie.Speaker.disable();
    Chappie.Speaker.end();
    
    Chappie.Mic.sleep();
    Chappie.Mic.end();

    Chappie.Lcd.sleep();
    Chappie.Tp.disable();

    FastLED.showColor(CRGB::Black);

    // Chappie.Buzz.play(Melody::Message);

    Power_IsShutdown = true;
    _LOG("[Power] the device is sleep!\r\n");

    delay(500);

    // Todo: ESP32低功耗API
    esp_deep_sleep_start();
}

/**
 * @brief  自动关机监控
 * @param  无
 * @retval 无
 */
void HAL::Power_Update()
{
    __IntervalExecute(Power_ADC_TrigUpdate(), 2000);

    // Todo: update Power_LastHandleTime
    if (Chappie.Tp.isTouched())
        Power_HandleTimeUpdate();

    if (!Power_AutoLowPowerEnable)
        return;

    if (Power_AutoLowPowerTimeout == 0)
        return;

    if (millis() - Power_LastHandleTime >= (Power_AutoLowPowerTimeout * 1000))
    {
        Power_Shutdown();
    }
}

void HAL::Power_EventMonitor()
{
    // if (Power.ShutdownReq)
    // {
    //     if (Power.EventCallback)
    //     {
    //         Power.EventCallback();
    //     }
    //     Backlight_SetGradual(0, 500);
    //     digitalWrite(CONFIG_POWER_EN_PIN, LOW);
    //     Serial.println("Power: OFF");
    //     Power.ShutdownReq = false;
    // }
}

void HAL::Power_GetInfo(Power_Info_t *info)
{
    int voltage = (int)(Chappie.Pow.readBatVoltage() * 1000);
    __LimitValue(voltage, BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE);

    int usage = Chappie.Pow.readBatPercentage();
    __LimitValue(usage, 0, 100);

    info->usage = usage;
    info->isCharging = ((usage == 100) ? true : false);
    info->voltage = voltage;
    info->isAutoPowerOff = Power_AutoLowPowerEnable;
}

void HAL::Power_SetEventCallback(Power_CallbackFunction_t callback)
{
    Power_EventCallback = callback;
}