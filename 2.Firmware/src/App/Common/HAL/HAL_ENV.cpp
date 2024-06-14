#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;
static HAL::CommitFunc_t CommitFunc = nullptr;
static void *UserData = nullptr;

void HAL::ENV_Init(void)
{
    // ...
}

void HAL::ENV_SetCommitCallback(CommitFunc_t func, void *userData)
{
    CommitFunc = func;
    UserData = userData;
}

void HAL::ENV_Update(void)
{
    MAG_Info_t magInfo = {0};

    magInfo.x = (int32_t)(Chappie.Env.readPressure());
    magInfo.y = (int32_t)(Chappie.Env.readAltitude());
    magInfo.z = (int32_t)(Chappie.Env.readTemperature());
    // Serial.printf("%d,%d,%d\n", mag.m.x, mag.m.y, mag.m.z);

    if (CommitFunc)
    {
        CommitFunc(&magInfo, UserData);
    }
}