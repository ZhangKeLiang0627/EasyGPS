#include "DialplateModel.h"

using namespace Page;

void DialplateModel::Init()
{
    account = new Account("DialplateModel", DataProc::Center(), 0, this); // 创建一个账户，注意这里把this传给了userdata
    account->Subscribe("SportStatus");                                    // 订阅了以下数据
    account->Subscribe("Recorder");
    account->Subscribe("StatusBar");
    account->Subscribe("GPS");
    // account->Subscribe("MusicPlayer");
    account->SetEventCallback(onEvent); // 设置回调函数，好忙啊
}

void DialplateModel::Deinit() // 恢复缺省
{
    if (account)
    {
        delete account; // 删除类
        account = nullptr;
    }
}

bool DialplateModel::GetGPSReady()
{
    HAL::GPS_Info_t gps;
    if (account->Pull("GPS", &gps, sizeof(gps)) != Account::RES_OK) // 从发行人/出版社拉取数据，大概是这么个翻译
    {
        return false;
    }
    return (gps.satellites > 0);
}

int DialplateModel::onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event != Account::EVENT_PUB_PUBLISH) // 如果发行人/出版社没有发布数据
    {
        return Account::RES_UNSUPPORTED_REQUEST; // 则返回不支持请求
    }

    if (strcmp(param->tran->ID, "SportStatus") != 0 || param->size != sizeof(HAL::SportStatus_Info_t))
    {
        return Account::RES_PARAM_ERROR;
    }

    DialplateModel *instance = (DialplateModel *)account->UserData;
    memcpy(&(instance->sportStatusInfo), param->data_p, param->size);

    return Account::RES_OK;
}

void DialplateModel::RecorderCommand(RecCmd_t cmd)
{
    if (cmd != REC_READY_STOP)
    {
        DataProc::Recorder_Info_t recInfo;
        DATA_PROC_INIT_STRUCT(recInfo);
        recInfo.cmd = (DataProc::Recorder_Cmd_t)cmd;
        recInfo.time = 1000;
        account->Notify("Recorder", &recInfo, sizeof(recInfo));
    }

    DataProc::StatusBar_Info_t statInfo;
    DATA_PROC_INIT_STRUCT(statInfo);
    statInfo.cmd = DataProc::STATUS_BAR_CMD_SET_LABEL_REC;

    switch (cmd)
    {
    case REC_START:
    case REC_CONTINUE:
        statInfo.param.labelRec.show = true;
        statInfo.param.labelRec.str = "REC";
        break;
    case REC_PAUSE:
        statInfo.param.labelRec.show = true;
        statInfo.param.labelRec.str = "PAUSE";
        break;
    case REC_READY_STOP:
        statInfo.param.labelRec.show = true;
        statInfo.param.labelRec.str = "STOP";
        break;
    case REC_STOP:
        statInfo.param.labelRec.show = false;
        break;
    default:
        break;
    }

    account->Notify("StatusBar", &statInfo, sizeof(statInfo));
}

void DialplateModel::PlayMusic(const char *music)
{
    // DataProc::MusicPlayer_Info_t info;
    // DATA_PROC_INIT_STRUCT(info); // 这里搞了个宏定义，就是用memset把这个info给初始化为0

    // info.music = music;
    // account->Notify("MusicPlayer", &info, sizeof(info)); // 通知
}

void DialplateModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    
    account->Notify("StatusBar", &info, sizeof(info));
}

void DialplateModel::SetStatusBarAppear(bool en)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    account->Notify("StatusBar", &info, sizeof(info));
}