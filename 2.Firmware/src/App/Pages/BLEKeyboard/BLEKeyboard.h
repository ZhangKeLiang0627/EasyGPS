#ifndef __BLEKEYBOARD_PRESENTER_H
#define __BLEKEYBOARD_PRESENTER_H

#include "BLEKeyboardView.h"
#include "BLEKeyboardModel.h"

namespace Page
{

    class BLEKeyboard : public PageBase
    {

    public:
        BLEKeyboard();
        virtual ~BLEKeyboard();

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

    public:
        static void musicCtrlBtnHandler(lv_event_t *event);
        static void directionCtrlBtnHandler(lv_event_t *event);
        static void mouseMoveCtrlBtnHandler(lv_event_t *event);
        static void mouseClickCtrlBtnHandler(lv_event_t *event);

    private:
        BLEKeyboardView View;
        BLEKeyboardModel Model;
        lv_timer_t *timer;
    };

}

#endif
