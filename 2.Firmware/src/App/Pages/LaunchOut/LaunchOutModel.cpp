#include "LaunchOutModel.h"

using namespace Page;

void LaunchOutModel::Init()
{
    account = new Account("LaunchOutModel", DataProc::Center(), 0, this);
    account->Subscribe("MusicPlayer");
    account->Subscribe("StatusBar");
}

void LaunchOutModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}

void LaunchOutModel::PlayMusic(const char* music)
{
    DataProc::MusicPlayer_Info_t info;
    DATA_PROC_INIT_STRUCT(info); 
    info.music = music;
    account->Notify("MusicPlayer", &info, sizeof(info));
}

void LaunchOutModel::SetStatusBarAppear(bool en)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    account->Notify("StatusBar", &info, sizeof(info));
}
