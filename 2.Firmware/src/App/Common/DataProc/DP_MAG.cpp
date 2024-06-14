#include "DataProc.h"
#include "../HAL/HAL.h"

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

    if (param->size != sizeof(HAL::MAG_Info_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    if (param->event == Account::EVENT_SUB_PULL)
    {
        HAL::MAG_Info_t *info = (HAL::MAG_Info_t *)param->data_p;

        HAL::MAG_GetInfo(info);

        printf("[DP] MAG PULL\r\n");
    }
    else if (param->event == Account::EVENT_NOTIFY)
    {
        HAL::MAG_Info_t *info = (HAL::MAG_Info_t *)param->data_p;

        if (info->isCalibrated)
            HAL::MAG_SetCalibration(); // 进行MAG校准

        printf("[DP] MAG NOTOFIY\r\n");
    }

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(MAG)
{
    account->SetEventCallback(onEvent);

    // HAL::MAG_SetCommitCallback([](void *info, void *userData)
    //                            {
    //    Account* account = (Account*)userData;
    //    return account->Commit(info, sizeof(HAL::MAG_Info_t)); },
    //                            account);
}
