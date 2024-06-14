#ifndef __LAUNCHOUT_VIEW_H
#define __LAUNCHOUT_VIEW_H

#include "../Page.h"

namespace Page
{

class LaunchOutView
{
public:
    void Create(lv_obj_t* root);
    void Delete();

public:
    struct
    {
        lv_obj_t *cont;
        lv_obj_t *circleFront;
        lv_obj_t *circleMiddle;
        lv_obj_t *circleBehind;
        lv_obj_t *square;
        lv_obj_t* labelLogo;

        lv_anim_timeline_t* anim_timeline;
    } ui;

private:
};

}

#endif // !__VIEW_H
