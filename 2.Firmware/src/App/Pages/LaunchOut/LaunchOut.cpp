#include "LaunchOut.h"

using namespace Page;

LaunchOut::LaunchOut()
{
}

LaunchOut::~LaunchOut()
{
}

void LaunchOut::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(false);                        // 设置是否手动管理缓存
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置自定义动画属性
}

void LaunchOut::onViewLoad()
{
    Model.Init();
    View.Create(_root);
    lv_timer_t *timer = lv_timer_create(onTimer, 2500, this);
    lv_timer_set_repeat_count(timer, 1);
}

void LaunchOut::onViewDidLoad()
{
}

void LaunchOut::onViewWillAppear()
{
    Model.PlayMusic("Startup");
    lv_anim_timeline_set_reverse(View.ui.anim_timeline, false);
    lv_anim_timeline_start(View.ui.anim_timeline);
}

void LaunchOut::onViewDidAppear()
{
    lv_obj_fade_out(_root, 500, 2000);
}

void LaunchOut::onViewWillDisappear()
{
}

void LaunchOut::onViewDidDisappear()
{
    // Model.SetStatusBarAppear(true);
}

void LaunchOut::onViewUnload()
{
    View.Delete();
    Model.Deinit();
}

void LaunchOut::onViewDidUnload()
{
}

void LaunchOut::onTimer(lv_timer_t *timer)
{                                                        // 2.5s定时，然后执行这个函数，实现页面跳转
    LaunchOut *instance = (LaunchOut *)timer->user_data; // 实例等于this //在onViewLoad里将this指针给了timer->user_data

    instance->_Manager->Replace("Pages/WaveTable"); //"Pages/Dialplate" "Pages/_Template"// 这个地方实现了启动界面后跳转到Dialplate界面
}

void LaunchOut::onEvent(lv_event_t *event)
{
    LaunchOut *instance = (LaunchOut *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (obj == instance->_root)
    {
        if (code == LV_EVENT_LEAVE)
        {
            // instance->Manager->Pop();
        }
    }
}
