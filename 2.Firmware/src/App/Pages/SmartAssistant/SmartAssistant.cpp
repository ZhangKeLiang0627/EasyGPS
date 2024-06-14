#include "SmartAssistant.h"

using namespace Page;

SmartAssistant::SmartAssistant()
    : timer(nullptr)
{
}

SmartAssistant::~SmartAssistant()
{
}

void SmartAssistant::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void SmartAssistant::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    Model.Init();
    View.Create(_root);

    AttachEvent(lv_scr_act());
    AttachEvent(View.ui.btnCont.btn); // 添加事件回调
}

void SmartAssistant::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
}

void SmartAssistant::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");

    // lv_indev_wait_release(lv_indev_get_act());  // lv_indev_get_act获取当前活动的输入设备 // lv_indev_wait_release等待当前输入设备的释放
    lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    LV_ASSERT_NULL(group);

    lv_group_set_wrap(group, false); // lv_group_set_wrap设置组对象的循环模式，以便在焦点移动时确定是否循环到组的开头，这对于导航控件和处理焦点移动时的行为非常有用

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP); // transparent 透明的清澈的

    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    lv_obj_fade_in(_root, 350, 0);
    View.AppearAnimStart(); // 开始加载进入该page的动画，出场动画 // 填flase是动画正着放，true是反着放
}

void SmartAssistant::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    timer = lv_timer_create(onTimerUpdate, 1000, this); // 这里设置一个1s的定时器，软定时器，用于在onTimerUpdate里update
}

void SmartAssistant::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_remove_all_objs(group);

    lv_timer_del(timer); // 删除该页面的软定时器
}

void SmartAssistant::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");
}

void SmartAssistant::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    View.Delete(); // 删除该页面的UI指针，释放容器的内存
    Model.Deinit();

    lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数
}

void SmartAssistant::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void SmartAssistant::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void SmartAssistant::Update()
{
    // LV_LOG_USER("Hi!\n");
    char SaaInputText[128] = {0};
    char SaaOutputText[256] = {0};
    bool SaaIsRunning = false;
    bool SaaIsListening = false;

    Model.GetSmartAssistantInfo(SaaInputText, SaaOutputText, &SaaIsRunning, &SaaIsListening);
    if (SaaIsRunning == true)
    {
        if (strcmp(SaaInputText, "none") == 0)
            // lv_label_set_text(View.ui.labelCont.inputText, "Q: ..");
            lv_label_set_text(View.ui.labelCont.inputText, "Q: 请你按下按钮对着麦克风提问吧.");
        else
        {
            lv_label_set_text_fmt(View.ui.labelCont.inputText, "Q: %s", SaaInputText);
        }

        if (strcmp(SaaOutputText, "none") == 0)
            // lv_label_set_text(View.ui.labelCont.outputText, "A: ..");
            lv_label_set_text(View.ui.labelCont.outputText, "A: 还没有得到回答呢.");
        else
        {
            lv_label_set_text_fmt(View.ui.labelCont.outputText, "A: %s", SaaOutputText);
        }

        if (SaaIsListening)
            lv_label_set_text_fmt(View.ui.labelCont.label, "%s", "I'M LISTENING...");
        else
            lv_label_set_text_fmt(View.ui.labelCont.label, "%s", "THINKING...");
    }
    else if (SaaIsRunning == false)
    {
        lv_label_set_text_fmt(View.ui.labelCont.label, "%s", "CLICK IT!");

        // 将按键使能，可以再次点击
        lv_obj_clear_state(View.ui.btnCont.btn, LV_STATE_DISABLED);
        // lv_label_set_text(View.ui.labelCont.inputText, "Q: ..");
        // lv_label_set_text(View.ui.labelCont.outputText, "A: ..");
    }
}

void SmartAssistant::onTimerUpdate(lv_timer_t *timer)
{
    SmartAssistant *instance = (SmartAssistant *)timer->user_data;

    instance->Update();
}

void SmartAssistant::onEvent(lv_event_t *event)
{
    SmartAssistant *instance = (SmartAssistant *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.btnCont.btn)
        {

            instance->Model.SetSmartAssistantBegin(true);

            lv_label_set_text_fmt(instance->View.ui.labelCont.label, "%s", "I'M LISTENING...");
            instance->View.AppearAnimClick();

            // 将按钮锁定，防止多次点击
            lv_obj_add_state(instance->View.ui.btnCont.btn, LV_STATE_DISABLED);
        }
    }

    if (code == LV_EVENT_GESTURE)
    {
        lv_indev_wait_release(lv_indev_get_act());

        switch (lv_indev_get_gesture_dir(lv_indev_get_act()))
        {
        case LV_DIR_LEFT:
            instance->_Manager->Pop();
            break;
        case LV_DIR_RIGHT:
            instance->_Manager->Pop();
            break;
        case LV_DIR_TOP:

            break;
        case LV_DIR_BOTTOM:

            break;
        default:
            break;
        }
    }
}
