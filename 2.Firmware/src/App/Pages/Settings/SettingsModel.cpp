#include "SettingsModel.h"

using namespace Page;

void SettingsModel::Init()
{
    account = new Account("SettingsModel", DataProc::Center(), 0, this); // 创建一个账户，注意这里把this传给了userdata
    account->Subscribe("StatusBar");
    account->Subscribe("WiFi");
    account->Subscribe("BLE");
    account->Subscribe("Backlight");
    // account->Subscribe("MusicPlayer");
    account->Subscribe("Power");
    account->SetEventCallback(onEvent); // 设置回调函数，好忙啊
}

void SettingsModel::Deinit() // 恢复缺省
{
    if (account)
    {
        delete account; // 删除类
        account = nullptr;
    }
}

void SettingsModel::GetWiFiInfo(
    char *name, char *IP, char *gateWay, char *Mac, bool *isConnected, bool *isAPEnabled, bool *isWiFiEnabled)
{
    HAL::WiFi_Info_t wifi = {0};
    account->Pull("WiFi", &wifi, sizeof(wifi));

    snprintf(name, 20, "%s", wifi.name);
    snprintf(IP, 20, "%s", wifi.IP);
    snprintf(gateWay, 20, "%s", wifi.gateWay);
    snprintf(Mac, 20, "%s", wifi.Mac);

    *isAPEnabled = wifi.isAPEnabled ? true : false;

    *isWiFiEnabled = wifi.isWiFiEnabled ? true : false;

    *isConnected = wifi.isConnected ? true : false;

    printf("[Model] GetWiFiInfo\r\n");
}

void SettingsModel::SetAPEnable(bool en)
{
    HAL::WiFi_Info_t info = {0};
    info.isAPEnabled = en;

    if (en)
        info.isWiFiEnabled = !en; // AP开，STA关

    account->Notify("WiFi", &info, sizeof(info)); // 通知
    printf("[Model] SetAPEnable\r\n");
}

void SettingsModel::SetWiFiEnable(bool en)
{
    HAL::WiFi_Info_t info = {0};
    info.isWiFiEnabled = en;

    if (en)
        info.isAPEnabled = !en; // STA开，AP关

    account->Notify("WiFi", &info, sizeof(info)); // 通知
    printf("[Model] SetWiFiEnable\r\n");
}

void SettingsModel::GetBLEInfo(char *name, bool *isConnected, bool *isEnabled)
{
    HAL::BLE_Info_t ble = {0};
    account->Pull("BLE", &ble, sizeof(ble));

    snprintf(name, 20, "%s", ble.name);

    *isConnected = ble.isConnected ? true : false;
    *isEnabled = ble.isEnabled ? true : false;

    printf("[Model] GetBLEInfo\r\n");
}

void SettingsModel::SetBLEEnable(bool en)
{
    HAL::BLE_Info_t info = {0};
    info.DC = true; // Cmd
    info.isEnabled = en;

    account->Notify("BLE", &info, sizeof(info)); // 通知
    if (en)
        printf("[Model] SetBLEEnable!\n");
    else
        printf("[Model] SetBLESleep!\n");
}

void SettingsModel::GetDisplayBrightness(uint16_t *val)
{
    uint16_t info = 0;
    account->Pull("Backlight", &info, sizeof(info));

    *val = info;

    printf("[Model] GetDisplayBrightness\r\n");
}

void SettingsModel::SetDisplayBrightness(uint16_t val)
{
    uint16_t info = val;

    account->Notify("Backlight", &info, sizeof(info)); // 通知

    printf("[Model] SetDisplayBrightness\r\n");
}

void SettingsModel::GetPowAutoOff(bool *en)
{
    HAL::Power_Info_t power = {0};

    account->Pull("Power", &power, sizeof(power));

    *en = power.isAutoPowerOff;
}

void SettingsModel::SetPowAutoOff(bool en)
{
    HAL::Power_Info_t info = {0};

    info.isAutoPowerOff = en;

    account->Notify("Power", &info, sizeof(info)); // 通知
}

int SettingsModel::onEvent(Account *account, Account::EventParam_t *param)
{
    // ...
    return Account::RES_OK;
}

void SettingsModel::PlayMusic(const char *music)
{
    DataProc::MusicPlayer_Info_t info;
    DATA_PROC_INIT_STRUCT(info); // 这里搞了个宏定义，就是用memset把这个info给初始化为0

    info.music = music;
    account->Notify("MusicPlayer", &info, sizeof(info)); // 通知
}

void SettingsModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}

void SettingsModel::SetStatusBarAppear(bool en)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    account->Notify("StatusBar", &info, sizeof(info));
}
