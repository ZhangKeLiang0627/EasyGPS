#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;
static HAL::CommitFunc_t CommitFunc = nullptr;
static void *UserData = nullptr;

static int Mag_x = 0, Mag_y = 0, Mag_z = 0, Mag_deg = 0; // MAG数据
static bool Mag_isCalibrated = false;                    // MAG数据是否矫正

bool HAL::MAG_Init()
{
    // ...
    return 0;
}

void HAL::MAG_SetCommitCallback(CommitFunc_t func, void *userData)
{
    CommitFunc = func;
    UserData = userData;
}

void HAL::MAG_SetCalibration(void)
{
    if (Mag_isCalibrated)
        return;

    _LOG("[HAL] MAG CALIBRATING. Keep moving your sensor...\n");
    Chappie.Mag.calibrate();
    Mag_isCalibrated = true;
    _LOG("[HAL] MAG CALIBRATION is FINISH!\n");
    // Chappie.Mag.setCalibrationOffsets(Chappie.Mag.getCalibrationOffset(0), Chappie.Mag.getCalibrationOffset(1), Chappie.Mag.getCalibrationOffset(2));
    // Chappie.Mag.setCalibrationScales(Chappie.Mag.getCalibrationScale(0), Chappie.Mag.getCalibrationScale(1), Chappie.Mag.getCalibrationScale(2));
    _LOG("DONE. Copy the lines below and paste it into your projects sketch.);\n");
    _LOG("compass.setCalibrationOffsets(%.2f, %.2f, %.2f);\n", Chappie.Mag.getCalibrationOffset(0), Chappie.Mag.getCalibrationOffset(1), Chappie.Mag.getCalibrationOffset(2));
    _LOG("compass.setCalibrationScales(%.2f, %.2f, %.2f);\n", Chappie.Mag.getCalibrationScale(0), Chappie.Mag.getCalibrationScale(1), Chappie.Mag.getCalibrationScale(2));
}

void HAL::MAG_Update()
{
    Chappie.Mag.read();
    Mag_deg = Chappie.Mag.getAzimuth();

    Mag_x = Chappie.Mag.getX();
    Mag_y = Chappie.Mag.getY();
    Mag_z = Chappie.Mag.getZ();


    // _LOG("[MAG] x:%d, y:%d, z:%d\r\n", Mag_x, Mag_y, Mag_z);

    if (CommitFunc)
    {
        MAG_Info_t magInfo;
        magInfo.x = Mag_x;
        magInfo.y = Mag_y;
        magInfo.z = Mag_z;
        magInfo.deg = Mag_deg;
        magInfo.isCalibrated = Mag_isCalibrated;

        CommitFunc(&magInfo, UserData);
    }
}

void HAL::MAG_GetInfo(HAL::MAG_Info_t *info)
{
    info->x = Mag_x;
    info->y = Mag_y;
    info->z = Mag_z;
    info->deg = Mag_deg;
    info->isCalibrated = Mag_isCalibrated;
}