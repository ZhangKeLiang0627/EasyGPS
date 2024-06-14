#include "HAL.h"

/**
 * @brief Task to handle hal timer
 *
 * @param param
 */
static void task_hal_update_handler(void *param)
{
    while (1)
    {
        HAL::HAL_Update();
        delay(10);
    }
    vTaskDelete(NULL);
}

void HAL::HAL_Init(void)
{
    // Buzz_init();
    // Audio_Init();
    Power_Init();

    xTaskCreate(task_hal_update_handler, "Hal", 1024 * 5, nullptr, 5, nullptr);
}

void HAL::HAL_Update(void)
{
    __IntervalExecute(HAL::IMU_Update(), 200);
    __IntervalExecute(HAL::MAG_Update(), 50);
    __IntervalExecute(HAL::Power_Update(), 500);
    __IntervalExecute(HAL::WiFi_Update(), 100);
    __IntervalExecute(HAL::GPS_Update(), 500);
    // __IntervalExecute(HAL::ENV_Update(), 100);
}