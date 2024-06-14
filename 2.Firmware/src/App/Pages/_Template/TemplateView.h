#ifndef __TEMPLATE_VIEW_H
#define __TEMPLATE_VIEW_H

#include "../Page.h"

namespace Page
{

    class TemplateView
    {
    public:
    public:
        struct
        {
            struct
            {
                lv_obj_t *cont;
                lv_obj_t *labelHello;
                lv_obj_t *labelUint;
            } topInfo;

            struct
            {
                lv_obj_t *cont;

            } bottomInfo;
            struct
            {
                lv_obj_t *cont;
                lv_obj_t *btnSayHi; // haha有点直白
                lv_obj_t *btnMenu;  // 进入菜单
                lv_obj_t *labelSayHi;
            } btnCont;

            lv_anim_timeline_t *anim_timeline;
            lv_anim_timeline_t *anim_timelineForSayHi;
        } ui;

        void Create(lv_obj_t *root);
        void Delete();
        void AppearAnimStart(bool reverse = false);
        void AppearAnimSayHi(bool reverse = false);

    private:
        void TopInfo_Create(lv_obj_t *par);
        void BottomInfo_Create(lv_obj_t *par);
        void BtnCont_Create(lv_obj_t *par);
        lv_obj_t *Btn_Create(lv_obj_t *par, const void *img_src, lv_coord_t y_ofs);
    };

}

#endif // !__VIEW_H
