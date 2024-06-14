#ifndef __POPCAT_MODEL_H
#define __POPCAT_MODEL_H

#include "App/Common/DataProc/DataProc.h"

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

namespace Page
{

class PopcatModel
{
public:
    

public:
    void Init();
    void Deinit();

    void SetStatusBarStyle(DataProc::StatusBar_Style_t style);

private:
    Account *account;

private:
    static int onEvent(Account *account, Account::EventParam_t *param);
};

}

#endif
