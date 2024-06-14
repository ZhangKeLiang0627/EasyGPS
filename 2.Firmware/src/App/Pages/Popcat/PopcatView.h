#ifndef __POPCAT_VIEW_H
#define __POPCAT_VIEW_H

#include "../Page.h"

namespace Page
{

    class PopcatView
    {
    public:
    public:
        struct
        {

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *imgPopCat;
            } popcatInfo;

            lv_anim_timeline_t *anim_timeline;

        } ui;

        void Create(lv_obj_t *root);
        void Delete();
        void AppearAnimStart(bool reverse = false);

    private:
    };

}

#endif // !__VIEW_H
