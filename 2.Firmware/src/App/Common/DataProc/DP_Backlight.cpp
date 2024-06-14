#include "DataProc.h"
#include "../HAL/HAL.h"
#include "App/Config/Config.h"

static void onTimer(Account *account)
{
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

    if (param->size != sizeof(uint16_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    if (param->event == Account::EVENT_SUB_PULL)
    {
        uint16_t *info = (uint16_t *)param->data_p;

        *info = HAL::Backlight_GetValue();

        printf("[DP] BacklightPULL\r\n");
    }
    else if (param->event == Account::EVENT_NOTIFY)
    {
        uint16_t *info = (uint16_t *)param->data_p;

        HAL::Backlight_SetValue(constrain(*info, 10, 255)); // 限制最低亮度为10，不要完全黑屏

        printf("[DP] BacklightNOTOFIY\r\n");
    }

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(Backlight)
{
    account->SetEventCallback(onEvent);
}
