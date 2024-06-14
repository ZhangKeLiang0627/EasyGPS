#ifndef __LAUNCHOUT_PRESENTER_H
#define __LAUNCHOUT_PRESENTER_H

#include "LaunchOutView.h"
#include "LaunchOutModel.h"

namespace Page
{

class LaunchOut : public PageBase
{
public:

public:
    LaunchOut();
    virtual ~LaunchOut();

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
    static void onTimer(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);

private:
    LaunchOutView View;
    LaunchOutModel Model;
};

}

#endif
