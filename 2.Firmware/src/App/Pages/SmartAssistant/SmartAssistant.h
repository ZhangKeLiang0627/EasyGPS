#ifndef __SMARTASSISTANT_PRESENTER_H
#define __SMARTASSISTANT_PRESENTER_H

#include "SmartAssistantView.h"
#include "SmartAssistantModel.h"

namespace Page
{

    class SmartAssistant : public PageBase
    {
    public:
        SmartAssistant();
        virtual ~SmartAssistant();

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
        static void onTimerUpdate(lv_timer_t *timer);
        static void onEvent(lv_event_t *event);

    private:
        SmartAssistantView View;
        SmartAssistantModel Model;
        lv_timer_t *timer;
        char text[16] = "THINKING...";
    };

}

#endif
