#ifndef __BLEKEYBOARD_MODEL_H
#define __BLEKEYBOARD_MODEL_H

#include "App/Common/DataProc/DataProc.h"
#include "lvgl.h"

namespace Page
{

    class BLEKeyboardModel
    {
    public:
    public:
        void Init();
        void Deinit();

        void GetBLEInfo(char *name, bool *isConnected, bool *isEnabled);
        void SendBLEKeyboardValue(uint8_t *KeyVal);
        void SendBLEMouseMoveValue(uint8_t *MouseVal);
        void SendBLEMouseClickValue(uint16_t MouseClick);

        void SetStatusBarStyle(DataProc::StatusBar_Style_t style);
        void SetStatusBarAppear(bool en);

    private:
        Account *account;

    private:
        static int onEvent(Account *account, Account::EventParam_t *param);
    };

}

#endif
