#ifndef __TEMPLATE_MODEL_H
#define __TEMPLATE_MODEL_H

#include "App/Common/DataProc/DataProc.h"
#include "lvgl.h"

namespace Page
{

class TemplateModel
{
public:
    uint32_t TickSave;
    uint32_t GetData();
public:
    void Init();
    void Deinit();

    void SetStatusBarStyle(DataProc::StatusBar_Style_t style);
    void SetStatusBarAppear(bool en);

private:
    Account *account;

private:
    static int onEvent(Account *account, Account::EventParam_t *param);
};

}

#endif
