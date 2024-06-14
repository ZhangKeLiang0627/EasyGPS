#include "PopcatView.h"
#include <stdarg.h>
#include <stdio.h>

using namespace Page;

void PopcatView::Create(lv_obj_t *root)
{
    /* screen layout */
    lv_obj_t *cont = lv_obj_create(root);
    lv_obj_remove_style_all(cont);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xcaaf9b), 0);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0); // 设置cont画布完全不透明，0表示默认状态，当然还有什么按键按下状态，聚焦状态之类的

    ui.popcatInfo.cont = cont;

    lv_obj_t *img = lv_obj_create(cont);

    lv_obj_set_size(img, LV_HOR_RES, LV_VER_RES);

    lv_obj_set_style_bg_img_src(img, ResourcePool::GetImage("popcat_close"), 0);               // lv_obj_set_style_bg_img_src用于设置对象的背景图像源。通过这个函数可以为指定的对象设置背景图像，可以是内存中的图片数据，也可以是文件路径
    lv_obj_set_style_bg_img_src(img, ResourcePool::GetImage("popcat_open"), LV_STATE_PRESSED); // lv_obj_set_style_bg_img_src用于设置对象的背景图像源。通过这个函数可以为指定的对象设置背景图像，可以是内存中的图片数据，也可以是文件路径
    lv_obj_align_to(img, cont, LV_ALIGN_CENTER, 0, 0);

    ui.popcatInfo.imgPopCat = img;

    static lv_style_transition_dsc_t tran;                       // lv_style_transition_dsc_t用于描述样式过渡的属性。这个数据结构用于定义样式过渡的动画效果，包括过渡的时间、延迟、缓动函数等属性
    static const lv_style_prop_t prop[] = {LV_STYLE_BG_IMG_SRC}; // lv_style_prop_t枚举类型，用于表示样式的属性。这个枚举类型定义了一系列的样式属性，例如背景颜色、边框宽度、文本颜色等
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_linear,
        0,
        0,
        nullptr); // 为啥要做这个初始化呢？原因是如果不做这个初始化，你按下的样式也会发生改变，但是就是两个图片的切换，而没有过渡动画
    lv_obj_set_style_transition(img, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(img);

    /* animation create */
    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

#define ANIM_OPA_DEF(start_time, obj) \
    ANIM_DEF(start_time, obj, opa_scale, LV_OPA_TRANSP, LV_OPA_COVER)

    lv_anim_timeline_wrapper_t wrapper[] =
        {

            ANIM_OPA_DEF(200, ui.popcatInfo.cont),

            LV_ANIM_TIMELINE_WRAPPER_END // 这个标志着结构体成员结束，不能省略，在下面函数lv_anim_timeline_add_wrapper的轮询中做判断条件
        };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void PopcatView::Delete()
{
    if (ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void PopcatView::AppearAnimStart(bool reverse) // 开始开场动画
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
