#include "CompassModel.h"

using namespace Page;

void CompassModel::Init()
{
    account = new Account("CompassModel", DataProc::Center(), 0, this); // 创建一个账户，注意这里把this传给了userdata

    account->Subscribe("StatusBar");
    account->Subscribe("MAG");

    account->SetEventCallback(onEvent); // 设置回调函数，好忙啊
}

void CompassModel::Deinit() // 恢复缺省
{
    if (account)
    {
        delete account; // 删除类
        account = nullptr;
    }
}

void CompassModel::GetMAGInfo(
    int *dir,
    int *x,
    int *y,
    int *z,
    bool *isCalibrated)
{
    HAL::MAG_Info_t mag = {0};

    account->Pull("MAG", &mag, sizeof(mag));

    *dir = mag.deg;
    *x = mag.x;
    *y = mag.y;
    *z = mag.z;
    *isCalibrated = mag.isCalibrated;
}

void CompassModel::SetMAGCalibration(void) 
{
    HAL::MAG_Info_t mag = {0};

    mag.isCalibrated = true;

    account->Notify("MAG", &mag, sizeof(mag));
}

int CompassModel::onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event != Account::EVENT_PUB_PUBLISH) // 如果发行人/出版社没有发布数据
    {
        return Account::RES_UNSUPPORTED_REQUEST; // 则返回不支持请求
    }

    if (strcmp(param->tran->ID, "StatusBar") != 0)
    {
        return Account::RES_PARAM_ERROR;
    }

    CompassModel *instance = (CompassModel *)account->UserData;

    return Account::RES_OK;
}

void CompassModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}
