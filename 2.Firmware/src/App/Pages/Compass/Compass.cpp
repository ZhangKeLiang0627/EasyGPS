#include "Compass.h"

using namespace Page;

Compass::Compass()
    : timer(nullptr)
{
}

Compass::~Compass()
{
}

void Compass::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void Compass::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    Model.Init();
    View.Create(_root);

    AttachEvent(lv_scr_act());
    AttachEvent(View.ui.btnCont.btn); // 添加事件回调
}

void Compass::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
}

void Compass::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");

    // lv_indev_wait_release(lv_indev_get_act());  // lv_indev_get_act获取当前活动的输入设备 // lv_indev_wait_release等待当前输入设备的释放
    lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    LV_ASSERT_NULL(group);

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP); // transparent 透明的清澈的

    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    lv_obj_fade_in(_root, 350, 0);

    View.AppearAnimStart(); // 开始加载进入该page的动画，出场动画 // 填flase是动画正着放，true是反着放
}

void Compass::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    timer = lv_timer_create(onTimerUpdate, 100, this); // 这里设置一个1s的定时器，软定时器，用于在onTimerUpdate里update
}

void Compass::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_remove_all_objs(group);

    lv_timer_del(timer); // 删除该页面的软定时器
}

void Compass::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");
}

void Compass::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    View.Delete(); // 删除该页面的UI指针，释放容器的内存
    Model.Deinit();

    lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数
}

void Compass::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void Compass::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Compass::Update()
{
    LV_LOG_USER("Hi!\n");

    /* MAG */
    static int16_t angleLast = 0;
    int angle = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    bool isCalibrated = false;

    Model.GetMAGInfo(&angle, &x, &y, &z, &isCalibrated);

    // 如果校准成功
    if (isCalibrated)
    {
        lv_obj_move_background(View.ui.btnCont.cont);
        if (angleLast < angle)
        {
            int16_t tmp = angle - angleLast;

            if (tmp < 180)
            {
                int16_t cnt = tmp / 10;
                if (tmp == 0)
                    tmp++;
                angleLast += cnt;
            }
            else
            {
                tmp = 180 + 179 - angle + angleLast;
                int16_t cnt = tmp / 10;
                if (tmp == 0)
                    tmp++;
                angleLast -= cnt;
            }
        }
        else
        {
            int16_t tmp = angleLast - angle;
            if (tmp < 180)
            {
                int16_t cnt = tmp / 10;
                angleLast -= cnt;
            }
            else
            {
                tmp = 180 + 179 + angle - angleLast;
                int16_t cnt = tmp / 10;
                if (tmp == 0)
                    tmp++;
                angleLast += cnt;
            }
        }

        if (angleLast > 180)
            angleLast = -179 + (angleLast - 180);
        if (angleLast < -179)
            angleLast = 180 - (-179 - angleLast);

        lv_img_set_angle(View.ui.mainCont.img,  angleLast * 10);

        uint8_t tmp = 0;
        if (angleLast < 157)
            tmp = 0;
        else if (angleLast < 112)
            tmp = 1;
        else if (angleLast < 67)
            tmp = 2;
        else if (angleLast < 22)
            tmp = 3;
        else if (angleLast < -22)
            tmp = 4;
        else if (angleLast < -67)
            tmp = 5;
        else if (angleLast < -112)
            tmp = 6;
        else if (angleLast < -157)
            tmp = 7;
        else
            tmp = 7;

        int16_t angle_t = angleLast;
        if (angle_t < 0)
            angle_t += 360;
        angle_t = -angle_t + 360;

        lv_label_set_text_fmt(View.ui.mainCont.label[label_angle], rotation_ch[tmp], angle_t);
    }
}

void Compass::onTimerUpdate(lv_timer_t *timer)
{
    Compass *instance = (Compass *)timer->user_data;

    instance->Update();
}

void Compass::onEvent(lv_event_t *event)
{
    Compass *instance = (Compass *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.btnCont.btn)
        {
            lv_label_set_text_fmt(instance->View.ui.btnCont.label, "%s", "开始校准");
            instance->View.AppearAnimClick();
            instance->Model.SetMAGCalibration();
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
