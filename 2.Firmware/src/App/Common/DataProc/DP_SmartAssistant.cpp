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

    if (param->size != sizeof(HAL::Saa_Info_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    if (param->event == Account::EVENT_SUB_PULL)
    {
        HAL::Saa_Info_t *info = (HAL::Saa_Info_t *)param->data_p;

        HAL::SmartAssistant_GetInfo(info);

        printf("[DP] SmartAssistantPULL\r\n");
    }
    else if (param->event == Account::EVENT_NOTIFY)
    {

        HAL::SmartAssistant_SetBegin(true); 

        printf("[DP] SmartAssistantNOTOFIY\r\n");
    }

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(SmartAssistant)
{
    account->SetEventCallback(onEvent);
}
