#ifndef __COMPASS_VIEW_H
#define __COMPASS_VIEW_H

#include "../Page.h"

namespace Page
{
    static const char *rotation_ch[] = {
        "西南%d°",
        "西%d°",
        "西北%d°",
        "北%d°",
        "东北%d°",
        "东%d°",
        "东南%d°",
        "南%d°"};

    enum
    {
        label_start_calibration,
        label_x_min,
        label_y_min,
        label_z_min,
        label_x_max,
        label_y_max,
        label_z_max,
        label_tip,
        label_angle,
        label_cnt
    };

    class CompassView
    {
    public:

    public:
        struct
        {
            struct
            {
                lv_obj_t *cont;
                lv_obj_t *img;
                lv_obj_t *label[label_cnt];

            } mainCont;

            struct 
            {
                lv_obj_t *cont;
            } calibrationCont;
            
            struct
            {
                lv_obj_t *cont;
                lv_obj_t *label;
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
        void MainCont_Create(lv_obj_t *par);
        void CalibrationCont_Create(lv_obj_t *par);
        void BtnCont_Create(lv_obj_t *par);
        lv_obj_t *Btn_Create(lv_obj_t *par, const void *img_src, lv_coord_t y_ofs);
        lv_obj_t *RoundRect_Create(lv_obj_t *par, lv_coord_t x_ofs, lv_coord_t y_ofs);
    };

}

#endif // !__VIEW_H
