#ifndef __SMARTASSISTANT_MODEL_H
#define __SMARTASSISTANT_MODEL_H

#include "App/Common/DataProc/DataProc.h"
#include "lvgl.h"

namespace Page
{

    class SmartAssistantModel
    {
    public:
    public:
        void Init();
        void Deinit();

        void SetStatusBarStyle(DataProc::StatusBar_Style_t style);
        void SetSmartAssistantBegin(bool en);
        void GetSmartAssistantInfo(char *inputText, char *outputText, bool *isRunning, bool *isListening);

    private:
        Account *account;

    private:
        static int onEvent(Account *account, Account::EventParam_t *param);
    };

}

#endif
