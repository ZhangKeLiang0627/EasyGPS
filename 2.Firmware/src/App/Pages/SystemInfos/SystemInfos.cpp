#include "SystemInfos.h"
#include "App/Version.h"
#include "HardwareSerial.h"

using namespace Page;

SystemInfos::SystemInfos()
{
}

SystemInfos::~SystemInfos()
{
}

void SystemInfos::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
}

void SystemInfos::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    Serial.println("[SystemInfos] onViewLoad!");

    Model.Init();
    View.Create(_root);
    AttachEvent(_root);

    SystemInfosView::item_t *item_grp = ((SystemInfosView::item_t *)&View.ui);

    for (int i = 0; i < sizeof(View.ui) / sizeof(SystemInfosView::item_t); i++)
    {
        AttachEvent(item_grp[i].icon);
    }
}

void SystemInfos::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
}

void SystemInfos::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);
    Model.SetStatusBarAppear(true);
    
    timer = lv_timer_create(onTimerUpdate, 1000, this);
    lv_timer_ready(timer); // 就是无视倒计时，直接给一个触发标志位，就是马上跑一遍回调函数

    View.SetScrollToY(_root, -LV_VER_RES, LV_ANIM_OFF);
    lv_obj_set_style_opa(_root, LV_OPA_TRANSP, 0);
    lv_obj_fade_in(_root, 250, 0);
}

void SystemInfos::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    View.onFocus(group);
}

void SystemInfos::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_focus_obj(nullptr);
    lv_group_remove_all_objs(group);
    lv_group_set_focus_cb(lv_group_get_default(), nullptr);
    lv_obj_fade_out(_root, 250, 0);
}

void SystemInfos::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");

    lv_timer_del(timer);
}

void SystemInfos::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    View.Delete();
    Model.Deinit();
}

void SystemInfos::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void SystemInfos::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void SystemInfos::Update()
{
    char buf[64] = {0};

    /* Sport */
    float trip = 0;
    float maxSpd = 0;
    // Model.GetSportInfo(&trip, buf, sizeof(buf), &maxSpd);
    View.SetSport(trip, buf, maxSpd);

    /* GPS */
    float lat = 0;
    float lng = 0;
    float alt = 0;
    float course = 0;
    float speed = 0;
    Model.GetGPSInfo(&lat, &lng, &alt, buf, sizeof(buf), &course, &speed);
    View.SetGPS(lat, lng, alt, buf, course, speed);

    /* MAG */
    float dir = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    Model.GetMAGInfo(&dir, &x, &y, &z);
    View.SetMAG(dir, x, y, z);

    /* IMU */
    int steps = 0;
    Model.GetIMUInfo(&steps, buf, sizeof(buf));
    View.SetIMU(steps, buf);

    /* RTC */
    Model.GetRTCInfo(buf, sizeof(buf));
    View.SetRTC(buf);

    /* Power */
    int usage = 0;
    float voltage = 0;
    Model.GetBatteryInfo(&usage, &voltage, buf, sizeof(buf));
    View.SetBattery(usage, voltage, buf);

    /* Storage */
    bool detect = 0;
    const char *type = "-";
    Model.GetStorageInfo(&detect, &type, buf, sizeof(buf));
    View.SetStorage(
        detect ? "OK" : "ERROR",
        buf,
        type,
        VERSION_FILESYSTEM);

    /* System */
    DataProc::MakeTimeString(lv_tick_get(), buf, sizeof(buf));
    View.SetSystem(
        VERSION_FIRMWARE_NAME " " VERSION_SOFTWARE,
        VERSION_AUTHOR_NAME,
        VERSION_LVGL,
        buf,
        VERSION_COMPILER,
        VERSION_BUILD_TIME);
}

void SystemInfos::onTimerUpdate(lv_timer_t *timer)
{
    SystemInfos *instance = (SystemInfos *)timer->user_data;

    instance->Update();
}

void SystemInfos::onEvent(lv_event_t *event)
{
    SystemInfos *instance = (SystemInfos *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED)
    {
        if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
        {
            instance->_Manager->Pop();
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
