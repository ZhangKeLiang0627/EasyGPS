#include "DataProc.h"
#include "../HAL/HAL.h"
#include "App/Config/Config.h"

using namespace DataProc;

static void onTimer(Account *account)
{
}

static void onNotify(Account *account, HAL::BLE_Info_t *info)
{
    switch (info->DC)
    {
    // Cmd
    case true:
    {
        if (info->isEnabled)
            HAL::BLE_SetBegin();
        else
            HAL::BLE_SetSleep();
        break;
    }
    // Data
    case false:
    {
        if (info->KeyVal[0] | info->KeyVal[1])
            HAL::BLE_SetKeyboardValue(info->KeyVal);

        if (info->MouseVal[0] | info->MouseVal[1] | info->MouseVal[2] | info->MouseVal[3])
            HAL::BLE_SetMouseMoveValue(info->MouseVal);

        if (info->MouseClick)
            HAL::BLE_SetMouseClickValue(info->MouseClick);
        break;
    }

    default:
        break;
    }
}

static int onEvent(Account *account, Account::EventParam_t *param)
{

    if (param->event == Account::EVENT_TIMER)
    {
        onTimer(account);
        return Account::RES_OK;
    }

    if (param->event != Account::EVENT_SUB_PULL && param->event != Account::EVENT_NOTIFY)
    {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    if (param->size != sizeof(HAL::BLE_Info_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    switch (param->event)
    {
    case Account::EVENT_SUB_PULL:
    {
        HAL::BLE_Info_t *info = (HAL::BLE_Info_t *)param->data_p;
        HAL::BLE_GetInfo(info);
        printf("[DP] BLE PULL\r\n");
        break;
    }
    case Account::EVENT_NOTIFY:
    {
        onNotify(account, (HAL::BLE_Info_t *)param->data_p);
        printf("[DP] BLE NOTOFIY\r\n");
        break;
    }
    default:
        break;
    }

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(BLE)
{
    account->SetEventCallback(onEvent);
}
