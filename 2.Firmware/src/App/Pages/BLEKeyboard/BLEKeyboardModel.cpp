#include "BLEKeyboardModel.h"

using namespace Page;

void BLEKeyboardModel::Init()
{
    account = new Account("BLEKeyboardModel", DataProc::Center(), 0, this); // 创建一个账户，注意这里把this传给了userdata

    account->Subscribe("StatusBar");
    account->Subscribe("BLE");

    account->SetEventCallback(onEvent); // 设置回调函数，好忙啊
}

void BLEKeyboardModel::Deinit() // 恢复缺省
{
    if (account)
    {
        delete account; // 删除类
        account = nullptr;
    }
}

int BLEKeyboardModel::onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event != Account::EVENT_PUB_PUBLISH) // 如果发行人/出版社没有发布数据
    {
        return Account::RES_UNSUPPORTED_REQUEST; // 则返回不支持请求
    }

    if (strcmp(param->tran->ID, "StatusBar") != 0)
    {
        return Account::RES_PARAM_ERROR;
    }

    BLEKeyboardModel *instance = (BLEKeyboardModel *)account->UserData;

    return Account::RES_OK;
}

void BLEKeyboardModel::GetBLEInfo(char *name, bool *isConnected, bool *isEnabled)
{
    HAL::BLE_Info_t ble = {0};
    account->Pull("BLE", &ble, sizeof(ble));

    snprintf(name, 20, "%s", ble.name);

    *isConnected = ble.isConnected ? true : false;
    *isEnabled = ble.isEnabled ? true : false;

    // printf("[Model] GetBLEInfo\n");
}

void BLEKeyboardModel::SendBLEKeyboardValue(uint8_t *KeyVal)
{
    HAL::BLE_Info_t ble = {0};

    ble.DC = false;
    ble.KeyVal[0] = KeyVal[0];
    ble.KeyVal[1] = KeyVal[1];

    account->Notify("BLE", &ble, sizeof(ble));

    // printf("[Model] SendBLEValue\n");
}
void BLEKeyboardModel::SendBLEMouseMoveValue(uint8_t *MouseVal)
{
    HAL::BLE_Info_t ble = {0};

    ble.DC = false;
    ble.MouseVal[0] = MouseVal[0];
    ble.MouseVal[1] = MouseVal[1];
    ble.MouseVal[2] = MouseVal[2];
    ble.MouseVal[3] = MouseVal[3];

    account->Notify("BLE", &ble, sizeof(ble));

    // printf("[Model] SendBLEMouseValue\n");
}

void BLEKeyboardModel::SendBLEMouseClickValue(uint16_t MouseClick)
{
    HAL::BLE_Info_t ble = {0};

    ble.DC = false;
    ble.MouseClick = MouseClick;

    account->Notify("BLE", &ble, sizeof(ble));
    // printf("[Model] SendBLEMouseClickValue\n");
}

void BLEKeyboardModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}

void BLEKeyboardModel::SetStatusBarAppear(bool en)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    account->Notify("StatusBar", &info, sizeof(info));
}