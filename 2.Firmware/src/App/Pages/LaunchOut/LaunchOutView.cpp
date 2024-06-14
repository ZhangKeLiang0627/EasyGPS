#include "LaunchOutView.h"
#include "App/Version.h"

using namespace Page;

void LaunchOutView::Create(lv_obj_t *root)
{
    lv_obj_t *circleBehind = lv_obj_create(root);
    lv_obj_remove_style_all(circleBehind);
    lv_obj_clear_flag(circleBehind, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(circleBehind, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_size(circleBehind, LV_HOR_RES + 30, LV_VER_RES + 30);
    lv_obj_set_style_radius(circleBehind, 42, LV_PART_MAIN);
    
    lv_obj_set_style_bg_color(circleBehind, lv_color_hex(0x2b3d6e), LV_PART_MAIN);
    lv_obj_set_align(circleBehind, LV_ALIGN_CENTER);

    ui.circleBehind = circleBehind;

    lv_obj_t *circleMiddle = lv_obj_create(circleBehind);
    lv_obj_set_size(circleMiddle, LV_HOR_RES - 20, LV_VER_RES - 20);
    lv_obj_set_style_radius(circleMiddle, 42, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(circleMiddle, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_bg_color(circleMiddle, lv_color_hex(0x4263a3), LV_PART_MAIN);
    lv_obj_set_align(circleMiddle, LV_ALIGN_CENTER);

    ui.circleMiddle = circleMiddle;

    lv_obj_t *circleFront = lv_obj_create(circleBehind);
    lv_obj_set_size(circleFront, LV_HOR_RES - 50, LV_VER_RES - 50);
    lv_obj_set_style_radius(circleFront, 42, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(circleFront, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_bg_color(circleFront, lv_color_hex(0x5f7eac), LV_PART_MAIN);
    lv_obj_set_align(circleFront, LV_ALIGN_CENTER);

    ui.circleFront = circleFront;

    lv_obj_t *square = lv_obj_create(circleBehind);
    lv_obj_set_size(square, LV_HOR_RES / 2, LV_VER_RES / 2);
    lv_obj_set_style_radius(square, 42, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(square, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_bg_color(square, lv_color_hex(0xa9bbad), LV_PART_MAIN);
    lv_obj_align_to(square, circleBehind, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_align(square, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_pos(square, LV_HOR_RES / 2, LV_VER_RES / 2);
    // lv_obj_center(square);

    ui.square = square;

    lv_obj_t *label = lv_label_create(circleBehind);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("smiley_36"), 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, VERSION_FIRMWARE_NAME);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    ui.labelLogo = label;

    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 1000, lv_anim_path_ease_in_out, true}

    lv_anim_timeline_wrapper_t wrapper[] =
        {
            ANIM_DEF(0, ui.circleBehind, height, 20, lv_obj_get_height(ui.circleBehind)),
            ANIM_DEF(0, ui.circleBehind, width, 20, lv_obj_get_width(ui.circleBehind)),

            ANIM_DEF(300, ui.circleMiddle, height, 20, lv_obj_get_height(ui.circleMiddle)),
            ANIM_DEF(300, ui.circleMiddle, width, 20, lv_obj_get_width(ui.circleMiddle)),

            ANIM_DEF(400, ui.circleFront, height, 20, lv_obj_get_height(ui.circleFront)),
            ANIM_DEF(400, ui.circleFront, width, 20, lv_obj_get_width(ui.circleFront)),

            ANIM_DEF(450, ui.square, height, 20, lv_obj_get_height(ui.square)),
            ANIM_DEF(450, ui.square, width, 20, lv_obj_get_width(ui.square)),

            ANIM_DEF(600, ui.labelLogo, y, 80, lv_obj_get_y(ui.labelLogo)),
            {700, ui.labelLogo, (lv_anim_exec_xcb_t)lv_label_set_opa_scale, LV_OPA_TRANSP, LV_OPA_COVER, 1000, lv_anim_path_ease_in_out, true},

            LV_ANIM_TIMELINE_WRAPPER_END};

    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void LaunchOutView::Delete()
{
    if (ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}
