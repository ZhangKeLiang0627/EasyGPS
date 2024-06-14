#include "TemplateModel.h"

using namespace Page;

void TemplateModel::Init()
{
    account = new Account("TemplateModel", DataProc::Center(), 0, this); // 创建一个账户，注意这里把this传给了userdata

    account->Subscribe("StatusBar");

    account->SetEventCallback(onEvent); // 设置回调函数，好忙啊
}

void TemplateModel::Deinit() // 恢复缺省
{
    if (account)
    {
        delete account; // 删除类
        account = nullptr;
    }
}

int TemplateModel::onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event != Account::EVENT_PUB_PUBLISH) // 如果发行人/出版社没有发布数据
    {
        return Account::RES_UNSUPPORTED_REQUEST; // 则返回不支持请求
    }

    if (strcmp(param->tran->ID, "StatusBar") != 0)
    {
        return Account::RES_PARAM_ERROR;
    }

    TemplateModel *instance = (TemplateModel *)account->UserData;

    return Account::RES_OK;
}

void TemplateModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}

void TemplateModel::SetStatusBarAppear(bool en)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    account->Notify("StatusBar", &info, sizeof(info));
}

uint32_t TemplateModel::GetData()
{
    return lv_tick_get();
}
