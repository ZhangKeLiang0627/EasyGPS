#include "SmartAssistantModel.h"

using namespace Page;

void SmartAssistantModel::Init()
{
    account = new Account("SmartAssistantModel", DataProc::Center(), 0, this); // 创建一个账户，注意这里把this传给了userdata

    account->Subscribe("StatusBar");
    account->Subscribe("SmartAssistant");

    account->SetEventCallback(onEvent); // 设置回调函数，好忙啊
}

void SmartAssistantModel::Deinit() // 恢复缺省
{
    if (account)
    {
        delete account; // 删除类
        account = nullptr;
    }
}

int SmartAssistantModel::onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event != Account::EVENT_PUB_PUBLISH) // 如果发行人/出版社没有发布数据
    {
        return Account::RES_UNSUPPORTED_REQUEST; // 则返回不支持请求
    }

    if (strcmp(param->tran->ID, "StatusBar") != 0)
    {
        return Account::RES_PARAM_ERROR;
    }

    SmartAssistantModel *instance = (SmartAssistantModel *)account->UserData;

    return Account::RES_OK;
}

void SmartAssistantModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}

void SmartAssistantModel::SetSmartAssistantBegin(bool en)
{
    HAL::Saa_Info_t info = {0};

    account->Notify("SmartAssistant", &info, sizeof(info)); // 通知

    printf("[Model] SetSmartAssistantBegin\r\n");
}

void SmartAssistantModel::GetSmartAssistantInfo(
    char *inputText, char *outputText, bool *isRunning, bool *isListening)
{
    HAL::Saa_Info_t info = {0};
    account->Pull("SmartAssistant", &info, sizeof(info));

    snprintf(inputText, 128, "%s", info.inputText);
    snprintf(outputText, 256, "%s", info.outputText);

    *isRunning = info.isRunning ? true : false;
    *isListening = info.isListening ? true : false;

    printf("[Model] GetSmartAssistantInfo\r\n");
}
