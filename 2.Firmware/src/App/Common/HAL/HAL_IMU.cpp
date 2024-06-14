#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

static HAL::CommitFunc_t CommitFunc = nullptr;
static void *UserData = nullptr;

void HAL::IMU_Init(void)
{
    // ...
}

void HAL::IMU_SetCommitCallback(CommitFunc_t func, void *userData)
{
    CommitFunc = func;
    UserData = userData;
}

void HAL::IMU_Update()
{
    IMU_Info_t imuInfo = {0};
    // float data[3] = {0};
    Chappie.Imu.getMotion6(
        &imuInfo.ax, &imuInfo.ay, &imuInfo.az,
        &imuInfo.gx, &imuInfo.gy, &imuInfo.gz);

    // Chappie.Imu.getYawPitchRoll(data[0], data[1], data[2]); // 使用此函数会闪退重启

    // imuInfo.ax = (int16_t)data[0];
    // imuInfo.ay = (int16_t)data[1];
    // imuInfo.az = (int16_t)data[2];

    //    Serial.printf(
    //        "ax = %d, ay = %d, az = %d, gx = %d, gy = %d, gz = %d\r\n",
    //        imuInfo.ax, imuInfo.ay, imuInfo.az, imuInfo.gx, imuInfo.gy, imuInfo.gz
    //    );

    // imuInfo.steps = Chappie.Imu.dmpGetSampleStepSizeMS();

    if (CommitFunc)
    {
        CommitFunc(&imuInfo, UserData);
    }
}