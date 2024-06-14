#include "Popcat.h"

using namespace Page;

Popcat::Popcat()
    : timer(nullptr)
{
}

Popcat::~Popcat()
{
}

void Popcat::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void Popcat::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    // Model.Init();
    View.Create(_root);

    // AttachEvent(View.ui.popcatInfo.imgPopCat);
    AttachEvent(View.ui.popcatInfo.cont);
    AttachEvent(lv_scr_act());
}

void Popcat::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
}

void Popcat::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");

    // lv_indev_wait_release(lv_indev_get_act());  // lv_indev_get_act获取当前活动的输入设备 // lv_indev_wait_release等待当前输入设备的释放
    // lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    // LV_ASSERT_NULL(group);

    // lv_group_add_obj(group, View.ui.popcatInfo.imgPopCat); // lv_group_add_obj用于向组对象中添加控件，这样组对象就可以管理该控件的交互和事件处理
    // lv_group_add_obj(group, View.ui.popcatInfo.cont);      // lv_group_add_obj用于向组对象中添加控件，这样组对象就可以管理该控件的交互和事件处理
    // lv_group_focus_obj(View.ui.popcatInfo.cont);

    // Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK); // transparent 透明的清澈的

    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    View.AppearAnimStart(); // 开始加载进入该page的动画，出场动画 // 填flase是动画正着放，true是反着放
}

void Popcat::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    timer = lv_timer_create(onTimerUpdate, 1000, this); // 这里设置一个1s的定时器，软定时器，用于在onTimerUpdate里update
}

void Popcat::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");
    // lv_group_t *group = lv_group_get_default();
    // LV_ASSERT_NULL(group);
    // lv_group_remove_all_objs(group);
    lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数

    lv_timer_del(timer); // 删除该页面的软定时器
}

void Popcat::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");
}

void Popcat::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    View.Delete(); // 删除该页面的UI指针，释放容器的内存
    // Model.Deinit();
}

void Popcat::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void Popcat::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Popcat::Update()
{
    LV_LOG_USER("Hi!\n");
}

void Popcat::onTimerUpdate(lv_timer_t *timer)
{
    Popcat *instance = (Popcat *)timer->user_data;

    instance->Update();
}

void Popcat::onEvent(lv_event_t *event)
{
    Popcat *instance = (Popcat *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    // if (code == LV_EVENT_LONG_PRESSED)
    // {
    //     if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
    //     {
    //         instance->_Manager->Pop();
    //     }
    // }

    if (code == LV_EVENT_GESTURE)
    {
        switch (lv_indev_get_gesture_dir(lv_indev_get_act()))
        {
        case LV_DIR_LEFT:
            printf("LV_DIR_LEFT!\n");
            instance->_Manager->Pop();

            break;
        case LV_DIR_RIGHT:
            printf("LV_DIR_RIGHT!\n");
            instance->_Manager->Pop();

            break;
        case LV_DIR_TOP:
            printf("LV_DIR_TOP!\n");

            break;
        case LV_DIR_BOTTOM:
            printf("LV_DIR_BOTTOM!\n");

            break;

        default:
            break;
        }
    }

    if (obj == instance->_root)
    {
        if (code == LV_EVENT_LEAVE)
        {
            instance->_Manager->Pop();
        }
    }
}
