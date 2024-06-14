#include "Template.h"
#include "HardwareSerial.h"

using namespace Page;

Template::Template()
    : timer(nullptr), lastFocus(nullptr), ClickCount(0)
{
}

Template::~Template()
{
}

void Template::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    Serial.println("[ChappieCore] onCustomAttrConfig!");

    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void Template::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    Serial.println("[ChappieCore] onViewLoad!");

    View.Create(_root);
    Model.Init();
    AttachEvent(View.ui.btnCont.btnSayHi); // 添加事件回调 // 两个button
    AttachEvent(View.ui.btnCont.btnMenu);
}

void Template::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
    Serial.println("[ChappieCore] onViewDidLoad!");
}

void Template::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");
    Serial.println("[ChappieCore] onViewWillAppear!");

    Model.SetStatusBarAppear(true);

    // lv_indev_wait_release(lv_indev_get_act());  // lv_indev_get_act获取当前活动的输入设备 // lv_indev_wait_release等待当前输入设备的释放
    lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    LV_ASSERT_NULL(group);

    lv_group_set_wrap(group, false); // lv_group_set_wrap设置组对象的循环模式，以便在焦点移动时确定是否循环到组的开头，这对于导航控件和处理焦点移动时的行为非常有用

    lv_group_add_obj(group, View.ui.btnCont.btnSayHi); // lv_group_add_obj用于向组对象中添加控件，这样组对象就可以管理该控件的交互和事件处理
    lv_group_add_obj(group, View.ui.btnCont.btnMenu);

    // if (lastFocus)
    // {
    //     lv_group_focus_obj(lastFocus); // lv_group_focus_obj用于设置组对象中的焦点控件 // 相当于选择吧，选择这个按钮时，会出现某些变化
    // }
    // else
    // {
    //     lv_group_focus_obj(View.ui.btnCont.btnSayHi); // lastFocus==NULL的时候，刚刚开机时，聚焦在中间的播放键（是播放键吧，记录键？hh
    // }

    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    View.AppearAnimStart(false); // 开始加载进入该page的动画，出场动画 // 填flase是动画正着放，true是反着放
}

void Template::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    Serial.println("[ChappieCore] onViewDidAppear!");

    timer = lv_timer_create(onTimerUpdate, 1000, this); // 这里设置一个1s的定时器，软定时器，用于在onTimerUpdate里update
}

void Template::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");
    Serial.println("[ChappieCore] onViewWillDisappear!");

    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    // lastFocus = lv_group_get_focused(group);

    lv_group_remove_all_objs(group);

    lv_timer_del(timer); // 删除该页面的软定时器
}

void Template::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");
    Serial.println("[ChappieCore] onViewDidDisappear!");
}

void Template::onViewUnload()
{
    LV_LOG_USER("onViewUnload");
    Serial.println("[ChappieCore] onViewUnload!");

    View.Delete(); // 删除该页面的UI指针，释放容器的内存
    Model.Deinit();
}

void Template::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
    Serial.println("[ChappieCore] onViewDidUnload!");
}

void Template::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Template::Update()
{
    LV_LOG_USER("Hi!\n");
    Serial.println("[Template] Hi!");
}

void Template::onTimerUpdate(lv_timer_t *timer)
{
    Template *instance = (Template *)timer->user_data;

    instance->Update();
}

void Template::onEvent(lv_event_t *event)
{
    Template *instance = (Template *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.btnCont.btnSayHi)
        {
            lv_label_set_text_fmt(instance->View.ui.topInfo.labelHello, "%02d", ++instance->ClickCount);
            instance->View.AppearAnimSayHi();
        }
        else if (obj == instance->View.ui.btnCont.btnMenu)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/Dialplate");
            }
        }
    }
}
