#include "DataProc.h"
#include "App/Utils/Filters/Filters.h"
#include "../HAL/HAL.h"

static void onTimer(Account* account)
{
   static bool lastStatus = false;

//    HAL::Power_Info_t power;
//    HAL::Power_GetInfo(&power);
//    if (power.isCharging != lastStatus)
//    {
//        DataProc::MusicPlayer_Info_t info;
//        DATA_PROC_INIT_STRUCT(info);
//        info.music = power.isCharging ? "BattChargeStart" : "BattChargeEnd";
//        account->Notify("MusicPlayer", &info, sizeof(info));

//        lastStatus = power.isCharging;
//    }
}

static int onEvent(Account* account, Account::EventParam_t* param)
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

   if (param->size != sizeof(HAL::Power_Info_t))
   {
       return Account::RES_SIZE_MISMATCH;
   }

   if (param->event == Account::EVENT_SUB_PULL)
   {
       HAL::Power_Info_t powerInfo;
       HAL::Power_GetInfo(&powerInfo);

       memcpy(param->data_p, &powerInfo, param->size);

    //    printf("[DP] PowerPULL\r\n");
   }
   else if (param->event == Account::EVENT_NOTIFY)
   {
       HAL::Power_Info_t *info = (HAL::Power_Info_t *)param->data_p;

       HAL::Power_SetAutoLowPowerEnable(info->isAutoPowerOff);

    //    printf("[DP] PowerNOTOFIY\r\n");
   }



    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(Power)
{
//    account->Subscribe("MusicPlayer");
   account->SetEventCallback(onEvent);
//    account->SetTimerPeriod(500);
}
