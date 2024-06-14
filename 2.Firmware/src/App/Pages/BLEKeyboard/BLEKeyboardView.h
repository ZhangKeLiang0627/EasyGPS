#ifndef __BLEKEYBOARD_VIEW_H
#define __BLEKEYBOARD_VIEW_H

#include "../Page.h"

namespace Page
{

    class BLEKeyboardView
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

            struct
            {
                lv_obj_t *cont;

                btnCont btnNextTrack;
                btnCont btnPrevTrack;
                btnCont btnPause;
                btnCont btnMute;
                btnCont btnVolumeUp;
                btnCont btnVolumeDown;

            } musicCtrlCont;

            struct
            {
                lv_obj_t *cont;

                btnCont btnDirUp;
                btnCont btnDirDown;
                btnCont btnPaste;
                btnCont btnCopy;
                btnCont btnCut;
                btnCont btnWithdraw;

            } directionCtrlCont;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *btnMouse;

            } mouseMoveCtrlCont;

            struct
            {
                lv_obj_t *cont;

                btnCont btnLeft;
                btnCont btnRight;
                btnCont btnBack;
                btnCont btnForward;
                btnCont btnPageUp;
                btnCont btnPageDown;

            } mouseClickCtrlCont;

            lv_anim_timeline_t *anim_timeline;
        } ui;

        void Create(lv_obj_t *root);
        void Delete();
        void AppearAnimStart(bool reverse = false);

    private:
        lv_obj_t *BtnCont_Create(lv_obj_t *par, uint32_t c = 0xffffff);
        void BtnKeyboard_Create(lv_obj_t *par, btnCont *cont, const char *name, const void *img_src);
        lv_obj_t *MouseCircle_Create(lv_obj_t *par);
    };

}

#endif // !__VIEW_H
