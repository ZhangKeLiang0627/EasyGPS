#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

/**
 * @brief  BLE初始化
 * @param  无
 * @retval 无
 */
void HAL::BLE_Init()
{
    // ...
}

void HAL::BLE_Update(void)
{
    // ...
}

void HAL::BLE_SetBegin(void)
{
    Chappie.Ble.begin();
}

void HAL::BLE_SetSleep(void)
{
    Chappie.Ble.sleep();
}

void HAL::BLE_SetKeyboardValue(uint8_t *c)
{
    printf("[BLE] c[0]: %d, c[1]: %d\n", c[0], c[1]);
    uint16_t Ret = Chappie.Ble.write(c);

    printf("[BLE] Ret: %d\n", Ret);
}

void HAL::BLE_SetMouseMoveValue(uint8_t *c)
{
    printf("[BLE] c[0]: %d, c[1]: %d, c[2]: %d, c[3]: %d\n", c[0], c[1], c[2], c[3]);
    Chappie.Ble.move(c);
}

void HAL::BLE_SetMouseClickValue(uint16_t b)
{
    printf("[BLE] b: %d\n", b);
    Chappie.Ble.click(b);
}

void HAL::BLE_GetInfo(HAL::BLE_Info_t *info)
{
    info->name = Chappie.Ble.GetName();
    info->isConnected = Chappie.Ble.isConnected();
    info->isEnabled = Chappie.Ble.isEnabled();
}