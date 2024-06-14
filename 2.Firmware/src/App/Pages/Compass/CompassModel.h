#ifndef __COMPASS_MODEL_H
#define __COMPASS_MODEL_H

#include "App/Common/DataProc/DataProc.h"
#include "lvgl.h"

namespace Page
{

    class CompassModel
    {
    public:
        void GetMAGInfo(
            int *dir,
            int *x,
            int *y,
            int *z,
            bool *isCalibrated);

        void SetMAGCalibration(void);

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
