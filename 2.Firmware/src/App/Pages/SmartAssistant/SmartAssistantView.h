#ifndef __SMARTASSISTANT_VIEW_H
#define __SMARTASSISTANT_VIEW_H

#include "../Page.h"

namespace Page
{

    class SmartAssistantView
    {
    public:
    public:
        struct
        {
            struct
            {
                lv_obj_t *cont;
                lv_obj_t *label;
                lv_obj_t *textCont;
                lv_obj_t *inputText;
                lv_obj_t *outputText;
            } labelCont;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *btn;
            } btnCont;
            lv_anim_timeline_t *anim_timeline;
            lv_anim_timeline_t *anim_timelineClick;
        } ui;

        void Create(lv_obj_t *root);
        void Delete();
        void AppearAnimStart(bool reverse = false);
        void AppearAnimClick(bool reverse = false);

    private:
        void LabelCont_Create(lv_obj_t *par);
        void BtnCont_Create(lv_obj_t *par);
        lv_obj_t *Btn_Create(lv_obj_t *par, const void *img_src, lv_coord_t y_ofs);
        lv_obj_t *RoundRect_Create(lv_obj_t *par, lv_coord_t x_ofs, lv_coord_t y_ofs);
    };

}

#endif // !__VIEW_H
