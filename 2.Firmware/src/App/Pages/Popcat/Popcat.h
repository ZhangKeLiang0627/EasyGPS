#ifndef __POPCAT_PRESENTER_H
#define __POPCAT_PRESENTER_H

#include "PopcatView.h"
#include "PopcatModel.h"

namespace Page
{

    class Popcat : public PageBase
    {
    public:
        Popcat();
        virtual ~Popcat();

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
        PopcatView View;
        PopcatModel Model;
        lv_timer_t *timer;
    };

}

#endif
