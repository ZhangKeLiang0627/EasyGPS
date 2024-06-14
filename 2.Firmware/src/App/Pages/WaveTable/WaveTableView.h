#ifndef __WAVETABLE_VIEW_H
#define __WAVETABLE_VIEW_H

#include "../Page.h"

namespace Page
{
    class WaveTableView
    {
    public:
        struct btnCont
        {
            lv_obj_t *btn;
            lv_obj_t *img;
            lv_obj_t *label;
        };

    public:
        struct
        {
            lv_obj_t *cont;

            btnCont btnA;
            btnCont btnB;
            btnCont btnC;
            btnCont btnD;
            btnCont btnE;
            btnCont btnF;
            btnCont btnG;

            lv_anim_timeline_t *anim_timeline;
        } ui;

        void Create(lv_obj_t *root);
        void Delete();
        void AppearAnimStart(bool reverse = false);

    private:
        void BtnCont_Create(lv_obj_t *par);
        void Btn_Create(lv_obj_t *par, btnCont *cont, const char *name, const void *img_src);
    };

}

#endif // !__VIEW_H
