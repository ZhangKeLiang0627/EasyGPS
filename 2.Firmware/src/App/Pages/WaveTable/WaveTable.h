#ifndef __WAVETABLE_PRESENTER_H
#define __WAVETABLE_PRESENTER_H

#include "WaveTableView.h"
#include "WaveTableModel.h"

namespace Page
{

    class WaveTable : public PageBase
    {
    public:
        WaveTable();
        virtual ~WaveTable();

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
        static void scrollEventCallback(lv_event_t *e);
        static void onEvent(lv_event_t *event);

    private:
        WaveTableView View;
        WaveTableModel Model;
        lv_timer_t *timer;
    };

}

#endif
