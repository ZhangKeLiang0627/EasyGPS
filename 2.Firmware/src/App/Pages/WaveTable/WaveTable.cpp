#include "WaveTable.h"

using namespace Page;

WaveTable::WaveTable()
    : timer(nullptr)
{
}

WaveTable::~WaveTable()
{
}

void WaveTable::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    // SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void WaveTable::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    Model.Init();
    View.Create(_root);

    lv_obj_add_event_cb(View.ui.cont, scrollEventCallback, LV_EVENT_SCROLL, NULL); // 滚动界面时使用回调函数
    /* Update the buttons position manually for first */
    lv_event_send(View.ui.cont, LV_EVENT_SCROLL, NULL);
    /* Be sure the fist button is in the middle */
    lv_obj_scroll_to_view(lv_obj_get_child(View.ui.cont, 1), LV_ANIM_ON);

    AttachEvent(View.ui.btnA.btn); // 添加事件回调
    AttachEvent(View.ui.btnB.btn); // 添加事件回调 
    AttachEvent(View.ui.btnC.btn); // 添加事件回调
    AttachEvent(View.ui.btnD.btn); // 添加事件回调
    AttachEvent(View.ui.btnE.btn); // 添加事件回调
    AttachEvent(View.ui.btnF.btn); // 添加事件回调
    AttachEvent(View.ui.btnG.btn); // 添加事件回调
}

void WaveTable::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
}

void WaveTable::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");

    lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    LV_ASSERT_NULL(group);

    lv_group_focus_obj(View.ui.cont);
    // lv_group_set_wrap(group, false); // lv_group_set_wrap设置组对象的循环模式，以便在焦点移动时确定是否循环到组的开头，这对于导航控件和处理焦点移动时的行为非常有用

    Model.SetStatusBarAppear(false);

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP); // transparent 透明的清澈的
    
    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    lv_obj_fade_in(_root, 350, 0);

    // View.AppearAnimStart(); // 开始加载进入该page的动画，出场动画 // 填flase是动画正着放，true是反着放
}

void WaveTable::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    timer = lv_timer_create(onTimerUpdate, 1000, this); // 这里设置一个1s的定时器，软定时器，用于在onTimerUpdate里update
}

void WaveTable::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_remove_all_objs(group);

    lv_timer_del(timer); // 删除该页面的软定时器

    lv_obj_fade_out(_root, 350, 0);
}

void WaveTable::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");
}

void WaveTable::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    View.Delete(); // 删除该页面的UI指针，释放容器的内存
    Model.Deinit();
}

void WaveTable::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void WaveTable::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void WaveTable::Update()
{
    LV_LOG_USER("Hi!\n");
}

void WaveTable::onTimerUpdate(lv_timer_t *timer)
{
    WaveTable *instance = (WaveTable *)timer->user_data;

    instance->Update();
}

void WaveTable::scrollEventCallback(lv_event_t *e)
{
    lv_obj_t *cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a); // 获得画布坐标
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10; // 计算半径
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont); // 获得画布子对象的个数
    for (i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i); // 获得画布子对象
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a); // 获得子对象坐标

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y); // 求绝对值

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if (diff_y >= r)
        {
            x = r;
        }
        else
        {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000); /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0); // 移动子对象

        /*Use some opacity with larger translations*/
        // lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER); // 根据移动的程度修改子对象的透明度
        // lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

void WaveTable::onEvent(lv_event_t *event)
{
    WaveTable *instance = (WaveTable *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.btnA.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/SystemInfos"); //"Pages/Dialplate"
            }
        }
        else if (obj == instance->View.ui.btnB.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/Dialplate"); //"Pages/Dialplate"
            }
        }
        else if (obj == instance->View.ui.btnC.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/Settings"); //"Pages/Dialplate"
            }
        }
        else if (obj == instance->View.ui.btnD.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/Popcat"); 
            }
        }
        else if (obj == instance->View.ui.btnE.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/SmartAssistant");
            }
        }
        else if (obj == instance->View.ui.btnF.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/Compass");
            }
        }
        else if (obj == instance->View.ui.btnG.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                instance->_Manager->Push("Pages/BLEKeyboard");
            }
        }
    }
}
