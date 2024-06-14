#ifndef __COMPASS_PRESENTER_H
#define __COMPASS_PRESENTER_H

#include "CompassView.h"
#include "CompassModel.h"

namespace Page
{

    class Compass : public PageBase
    {
    public:
        Compass();
        virtual ~Compass();

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
        CompassView View;
        CompassModel Model;
        lv_timer_t *timer;
        char text[16] = "THINKING...";
    };

}

#endif
