#ifndef __SETTINGS_PRESENTER_H
#define __SETTINGS_PRESENTER_H

#include "SettingsView.h"
#include "SettingsModel.h"

namespace Page
{
    class Settings : public PageBase // public继承
    {
    public:
        Settings();          // 构造函数
        virtual ~Settings(); // 析构函数

        virtual void onCustomAttrConfig();
        virtual void onViewLoad();
        virtual void onViewDidLoad();
        virtual void onViewWillAppear();
        virtual void onViewDidAppear();
        virtual void onViewWillDisappear();
        virtual void onViewDidDisappear();
        virtual void onViewUnload();
        virtual void onViewDidUnload();

    private:
        void Update();
        void AttachEvent(lv_obj_t *obj);
        static void onEvent(lv_event_t *event);
        static void onTimerUpdate(lv_timer_t *timer);

        void Content_Update(lv_obj_t *obj, const char *text);
        void Switch_Update(lv_obj_t *obj, bool isChecked);
        void Slider_Update(lv_obj_t *obj, int32_t val);

    public:
        static void WiFiSwitchHandler(lv_event_t *event);
        static void BLESwitchHandler(lv_event_t *event);
        static void DisplaySliderHandler(lv_event_t *event);
        static void PowerSwitchHandler(lv_event_t *event);

    private:
        SettingsView View;
        SettingsModel Model;
        lv_timer_t *timer;
    };

}

#endif
