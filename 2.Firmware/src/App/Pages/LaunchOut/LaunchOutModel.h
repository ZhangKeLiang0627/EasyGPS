#ifndef __LAUNCHOUT_MODEL_H
#define __LAUNCHOUT_MODEL_H

#include "App/Common/DataProc/DataProc.h"
#include "App/Common/HAL/HAL.h"

namespace Page
{

class LaunchOutModel
{
public:
    void Init();
    void Deinit();
    void PlayMusic(const char* music);
    void SetEncoderEnable(bool en)
    {
        HAL::Encoder_SetEnable(en);
    }
    void SetStatusBarAppear(bool en);

private:
    Account* account;
};

}

#endif
