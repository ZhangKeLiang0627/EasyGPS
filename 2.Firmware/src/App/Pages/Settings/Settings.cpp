#include "Settings.h"

using namespace Page;

Settings::Settings()
    : timer(nullptr)
{
}

Settings::~Settings()
{
}

void Settings::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void Settings::onViewLoad() // 该页面page的UI加载初始化
{
    LV_LOG_USER("onViewLoad");
    Model.Init();
    View.Create(_root); // 创建画布 // _root相当于全局画布，所有的控件都基于_root

    /* Update the buttons position manually for first */
    lv_event_send(View.ui.menuCont.cont, LV_EVENT_SCROLL, NULL);
    /* Be sure the fist button is in the middle */
    lv_obj_scroll_to_view(lv_obj_get_child(View.ui.menuCont.cont, 1), LV_ANIM_ON);

    AttachEvent(lv_scr_act());
    lv_obj_add_event_cb(lv_obj_get_child(View.ui.wifiInfo.swAP, -1), (lv_event_cb_t)WiFiSwitchHandler, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(lv_obj_get_child(View.ui.wifiInfo.swSTA, -1), (lv_event_cb_t)WiFiSwitchHandler, LV_EVENT_CLICKED, this);

    lv_obj_add_event_cb(lv_obj_get_child(View.ui.bleInfo.sw, -1), (lv_event_cb_t)BLESwitchHandler, LV_EVENT_CLICKED, this);

    lv_obj_add_event_cb(lv_obj_get_child(View.ui.displayInfo.brig, -1), (lv_event_cb_t)DisplaySliderHandler, LV_EVENT_VALUE_CHANGED, this);

    lv_obj_add_event_cb(lv_obj_get_child(View.ui.powInfo.swAutoPowOff, -1), (lv_event_cb_t)PowerSwitchHandler, LV_EVENT_CLICKED, this);
}

void Settings::onViewDidLoad() // 该页面page的UI结束加载初始化
{
    LV_LOG_USER("onViewDidLoad");
}

void Settings::onViewWillAppear() // 该页面page的UI开始出场动画
{
    LV_LOG_USER("onViewWillAppear");

    // lv_indev_wait_release(lv_indev_get_act());  // lv_indev_get_act获取当前活动的输入设备 // lv_indev_wait_release等待当前输入设备的释放
    lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    LV_ASSERT_NULL(group);

    // lv_group_set_wrap(group, false); // lv_group_set_wrap设置组对象的循环模式，以便在焦点移动时确定是否循环到组的开头，这对于导航控件和处理焦点移动时的行为非常有用

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP); // 设置状态栏的样式（？
    Model.SetStatusBarAppear(true);
    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    lv_obj_fade_in(_root, 350, 0);

    // View.AppearAnimStart(); // 开始加载进入该page的动画，出场动画
}

void Settings::onViewDidAppear() // 该页面page的UI结束出场动画
{
    LV_LOG_USER("onViewDidAppear");

    timer = lv_timer_create(onTimerUpdate, 1000, this); // 这里设置一个1s的定时器，软定时器，用于在onTimerUpdate里update
}

void Settings::onViewWillDisappear() // 该页面page的UI开始退场动画
{
    LV_LOG_USER("onViewWillDisappear");

    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);

    lv_group_remove_all_objs(group);

    lv_timer_del(timer);
}

void Settings::onViewDidDisappear() // 该页面page的UI结束退场动画
{
    LV_LOG_USER("onViewDidDisappear");
}

void Settings::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    Model.Deinit();
    View.Delete();
    lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数
}

void Settings::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void Settings::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Settings::Update()
{
    /* WiFi */
    char WiFiname[20] = {0};
    char WiFiIP[20] = {0};
    char WiFiGatWay[20] = {0};
    char WiFiMac[20] = {0};
    bool isWiFiConnected = false;
    bool isWiFiEnabled = false;
    bool isAPEnabled = false;
    Model.GetWiFiInfo(WiFiname, WiFiIP, WiFiGatWay, WiFiMac, &isWiFiConnected, &isAPEnabled, &isWiFiEnabled);
    // printf("[test] ip:%s\r\n", WiFiIP);
    Content_Update(View.ui.wifiInfo.labelName, WiFiname);
    Content_Update(View.ui.wifiInfo.labelConnect, isWiFiConnected ? "Connected" : "DisConnected");
    Content_Update(View.ui.wifiInfo.labelIP, WiFiIP);
    Switch_Update(View.ui.wifiInfo.swAP, isAPEnabled);
    Switch_Update(View.ui.wifiInfo.swSTA, isWiFiEnabled);

    /* BLEKeyboard */
    char BLEName[20] = {0};
    bool isBLEConnected = false;
    bool isBLEEnabled = false;
    Model.GetBLEInfo(BLEName, &isBLEConnected, &isBLEEnabled);
    Content_Update(View.ui.bleInfo.labelName, BLEName);
    Content_Update(View.ui.bleInfo.labelConnect, isBLEConnected ? "Connected" : "DisConnected");
    Switch_Update(View.ui.bleInfo.sw, isBLEEnabled);

    /* Backlight */
    uint16_t Brightness = 255;
    Model.GetDisplayBrightness(&Brightness);
    Slider_Update(View.ui.displayInfo.brig, Brightness);

    /* Power */
    bool isAutoPowOff = false;
    Model.GetPowAutoOff(&isAutoPowOff);
    Switch_Update(View.ui.powInfo.swAutoPowOff, isAutoPowOff);
}

void Settings::onTimerUpdate(lv_timer_t *timer)
{
    Settings *instance = (Settings *)timer->user_data;

    instance->Update();
}

void Settings::Content_Update(lv_obj_t *obj, const char *text)
{
    lv_label_set_text_fmt(lv_obj_get_child(obj, -1), "%s", text);
}

void Settings::Switch_Update(lv_obj_t *obj, bool isChecked)
{
    if (isChecked)
        lv_obj_add_state(lv_obj_get_child(obj, -1), LV_STATE_CHECKED);
    else
        lv_obj_clear_state(lv_obj_get_child(obj, -1), LV_STATE_CHECKED);
}

void Settings::Slider_Update(lv_obj_t *obj, int32_t val)
{
    lv_slider_set_value(lv_obj_get_child(obj, -1), val, LV_ANIM_OFF);
}

void Settings::onEvent(lv_event_t *event)
{
    Settings *instance = (Settings *)lv_event_get_user_data(event); // 获得this指针
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event); // lv_event_get_current_target用于获取当前正在处理事件的目标对象
    lv_event_code_t code = lv_event_get_code(event);    // lv_event_get_code就是获取短按长按双击之类的枚举

    if (code == LV_EVENT_SHORT_CLICKED)
    {
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

void Settings::WiFiSwitchHandler(lv_event_t *event)
{
    Settings *instance = (Settings *)lv_event_get_user_data(event); // 获得this指针
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event); // lv_event_get_current_target用于获取当前正在处理事件的目标对象
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_VALUE_CHANGED || LV_EVENT_CLICKED)
    {
        if (obj == lv_obj_get_child(instance->View.ui.wifiInfo.swAP, -1))
        {
            if (lv_obj_has_state(obj, LV_STATE_CHECKED))
            {
                instance->Model.SetAPEnable(true);
            }
            else
            {
                instance->Model.SetAPEnable(false);
            }
        }

        if (obj == lv_obj_get_child(instance->View.ui.wifiInfo.swSTA, -1))
        {
            if (lv_obj_has_state(obj, LV_STATE_CHECKED))
            {
                instance->Model.SetWiFiEnable(true);
            }
            else
            {
                instance->Model.SetWiFiEnable(false);
            }
        }
    }
}

void Settings::BLESwitchHandler(lv_event_t *event)
{
    Settings *instance = (Settings *)lv_event_get_user_data(event); // 获得this指针
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event); // lv_event_get_current_target用于获取当前正在处理事件的目标对象
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_VALUE_CHANGED || LV_EVENT_CLICKED)
    {
        if (obj == lv_obj_get_child(instance->View.ui.bleInfo.sw , -1))
        {
            if (lv_obj_has_state(obj, LV_STATE_CHECKED))
            {
                instance->Model.SetBLEEnable(true);
            }
            else
            {
                instance->Model.SetBLEEnable(false);
            }
        }
    }
}

void Settings::DisplaySliderHandler(lv_event_t *event)
{
    Settings *instance = (Settings *)lv_event_get_user_data(event); // 获得this指针
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event); // lv_event_get_current_target用于获取当前正在处理事件的目标对象
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_indev_wait_release(lv_indev_get_act());

        if (obj == lv_obj_get_child(instance->View.ui.displayInfo.brig, -1))
        {
            instance->Model.SetDisplayBrightness(lv_slider_get_value(obj));
        }
    }
}

void Settings::PowerSwitchHandler(lv_event_t *event)
{
    Settings *instance = (Settings *)lv_event_get_user_data(event); // 获得this指针
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event); // lv_event_get_current_target用于获取当前正在处理事件的目标对象
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_VALUE_CHANGED || LV_EVENT_CLICKED)
    {
        if (obj == lv_obj_get_child(instance->View.ui.powInfo.swAutoPowOff, -1))
        {
            if (lv_obj_has_state(obj, LV_STATE_CHECKED))
            {
                instance->Model.SetPowAutoOff(true);
            }
            else
            {
                instance->Model.SetPowAutoOff(false);
            }
        }
    }
}