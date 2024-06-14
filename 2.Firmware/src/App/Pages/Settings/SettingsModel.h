#ifndef __SETTINGS_MODEL_H
#define __SETTINGS_MODEL_H

#include "App/Common/DataProc/DataProc.h"
#include "lvgl.h"

namespace Page
{

    class SettingsModel
    {
    public:
        typedef enum
        {
            REC_START = DataProc::RECORDER_CMD_START,
            REC_PAUSE = DataProc::RECORDER_CMD_PAUSE,
            REC_CONTINUE = DataProc::RECORDER_CMD_CONTINUE,
            REC_STOP = DataProc::RECORDER_CMD_STOP,
            REC_READY_STOP
        } RecCmd_t;

    public:
        HAL::SportStatus_Info_t sportStatusInfo;

    public:
        void Init();
        void Deinit();

        void PlayMusic(const char *music);
        void SetStatusBarStyle(DataProc::StatusBar_Style_t style);
        void SetStatusBarAppear(bool en);

        void GetWiFiInfo(char *name, char *IP, char *gateWay, char *Mac, bool *isConnected, bool *isAPEnable, bool *isWiFiEnable);
        void SetAPEnable(bool en);
        void SetWiFiEnable(bool en);

        void GetBLEInfo(char *name, bool *isConnected, bool *isEnabled);
        void SetBLEEnable(bool en);

        void SetDisplayBrightness(uint16_t val);
        void GetDisplayBrightness(uint16_t *val);

        void GetPowAutoOff(bool *en);
        void SetPowAutoOff(bool en);

    private:
        Account *account;

    private:
        static int onEvent(Account *account, Account::EventParam_t *param);
    };

}

#endif
