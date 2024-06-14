#ifndef __SETTINGS_VIEW_H
#define __SETTINGS_VIEW_H

#include "../Page.h"

namespace Page
{

    class SettingsView
    {

    public:
        typedef struct
        {
            lv_obj_t *cont;
            lv_obj_t *lableValue;
            lv_obj_t *lableUnit;
        } SubInfo_t;

        typedef uint8_t lv_menu_builder_variant_t;

        enum
        {
            LV_MENU_ITEM_BUILDER_VARIANT_1,
            LV_MENU_ITEM_BUILDER_VARIANT_2
        };

    public:
        struct
        {
            struct
            {
                lv_obj_t *cont;
                lv_obj_t *page;
                lv_obj_t *section;
            } menuCont;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *AXPTemp;
                lv_obj_t *BatVolt;
                lv_obj_t *BatCurr;
                lv_obj_t *BatPower;
                lv_obj_t *swAutoPowOff;
                lv_obj_t *USBCharing;
            } powInfo;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *labelConnect;
                lv_obj_t *labelName;
                lv_obj_t *labelIP;
                lv_obj_t *swAP;
                lv_obj_t *swSTA;
            } wifiInfo;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *labelConnect;
                lv_obj_t *labelName;
                lv_obj_t *sw;
            } bleInfo;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *brig;
            } displayInfo;

            struct
            {
                lv_obj_t *cont;
                SubInfo_t labelInfoGrp[4];
            } bottomInfo;

            struct
            {
                lv_obj_t *cont;
                lv_obj_t *btnMap;
                lv_obj_t *btnRec;
                lv_obj_t *btnMenu;
            } btnCont;

            lv_anim_timeline_t *anim_timeline;
        } ui;

        void Create(lv_obj_t *root);
        void Delete();
        void AppearAnimStart(bool reverse = false);

    private:
        lv_obj_t *SwitchBox_Create(lv_obj_t *par, const char *title);
        lv_obj_t *SliderBox_Create(lv_obj_t *par, const char *title, int32_t min = 0, int32_t max = 255, int32_t val = 128);
        lv_obj_t *ContentBox_Create(lv_obj_t *par, const char *title, const char *content = nullptr);
        lv_obj_t *Switch_Create(lv_obj_t *par, bool state = false, lv_coord_t x_ofs = 0, lv_coord_t y_ofs = 0);
        lv_obj_t *Slider_Create(lv_obj_t *par, const void *img_src, lv_coord_t x_ofs = 0, lv_coord_t y_ofs = 0, int32_t min = 0, int32_t max = 255, int32_t val = 127);
        lv_obj_t *Btn_Create(lv_obj_t *par, const void *img_src = nullptr, lv_coord_t x_ofs = 0);
    };

}

#endif // !__VIEW_H
