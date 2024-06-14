#include "StartUp.h"
#include "HardwareSerial.h"

using namespace Page;

Startup::Startup()
{
}

Startup::~Startup()
{
}

void Startup::onCustomAttrConfig()
{
    SetCustomCacheEnable(false);                        // 设置是否手动管理缓存
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置自定义动画属性
}

void Startup::onViewLoad()
{
    Model.Init();
    Serial.println("[ChappieCore] StartUpLoad!");
    View.Create(_root);
    lv_timer_t *timer = lv_timer_create(onTimer, 2000, this); // 定时2s
    lv_timer_set_repeat_count(timer, 1);                      // 只用执行一次：大概是因为启动动画也就2s
}

void Startup::onViewDidLoad()
{
    Serial.println("[ChappieCore] StartUpOk!");
}

void Startup::onViewWillAppear()
{
    Model.PlayMusic("Startup");
    lv_anim_timeline_start(View.ui.anim_timeline);
    Serial.println("[ChappieCore] StartUpAnimOk!");
}

void Startup::onViewDidAppear()
{
    lv_obj_fade_out(_root, 500, 1500);
    Serial.println("[ChappieCore] StartUpAnimOut!");
}

void Startup::onViewWillDisappear()
{
    Serial.println("[ChappieCore] StartUpWillDisappear!");
}

void Startup::onViewDidDisappear()
{
    // Model.SetStatusBarAppear(true);
    Serial.println("[ChappieCore] StartUpDidDisappear!");
}

void Startup::onViewUnload()
{
    View.Delete();
    //    Model.SetEncoderEnable(true);
    Serial.println("[ChappieCore] StartUpUnload!");

    Model.Deinit();
}

void Startup::onViewDidUnload()
{
    Serial.println("[ChappieCore] StartUpDidUnload!");
}

void Startup::onTimer(lv_timer_t *timer)
{                                                    // 2s定时，然后执行这个函数，实现页面跳转
    Startup *instance = (Startup *)timer->user_data; // 实例等于this //在onViewLoad里将this指针给了timer->user_data
    Serial.println("[ChappieCore] StartUpOnTimer!");

    instance->_Manager->Replace("Pages/_Template"); //"Pages/Dialplate" "Pages/_Template"// 这个地方实现了启动界面后跳转到Dialplate界面
}

void Startup::onEvent(lv_event_t *event)
{
    Startup *instance = (Startup *)lv_event_get_user_data(event);
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
