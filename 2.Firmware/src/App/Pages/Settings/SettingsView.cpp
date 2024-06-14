#include "SettingsView.h"
#include <stdarg.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) // 获取数组大小

using namespace Page;

void SettingsView::Create(lv_obj_t *root) // 初始化画布
{
    lv_obj_t *cont = lv_obj_create(root);
    ui.menuCont.cont = cont;
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xe7e1cb), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_center(cont);

    lv_obj_set_style_pad_all(cont, 10, LV_PART_MAIN);      // 设置每一个item的宽度
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);      // 设置每一个item的间距
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);       // 设置弹性布局，item竖着排
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);               // 设置画布滚动方向：垂直滚动
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER); // 设置在垂直滚动结束时捕捉子元素的位置：人话：打开菜单第一个item的位置，现在是居中
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_ON); // 设置滚动条是否显示：是
    // lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    ui.wifiInfo.labelName = ContentBox_Create(cont, "WiFi Name", "none");
    ui.wifiInfo.labelConnect = ContentBox_Create(cont, "WiFi State", "Disconneccted");
    ui.wifiInfo.labelIP = ContentBox_Create(cont, "WiFi IP", "none");
    ui.wifiInfo.swSTA = SwitchBox_Create(cont, "WiFi Mode");
    ui.wifiInfo.swAP = SwitchBox_Create(cont, "AP Mode");

    ui.bleInfo.labelName = ContentBox_Create(cont, "BLE Name", "none");
    ui.bleInfo.labelConnect = ContentBox_Create(cont, "BLE State", "Disconneccted");
    ui.bleInfo.sw = SwitchBox_Create(cont, "BLE Enable");

    ui.displayInfo.brig = SliderBox_Create(cont, "Bright");

    ui.powInfo.swAutoPowOff = SwitchBox_Create(cont, "AutoPowOff");
}

void SettingsView::Delete()
{
    // if (ui.anim_timeline)
    // {
    //     lv_anim_timeline_del(ui.anim_timeline);
    //     ui.anim_timeline = nullptr;
    // }
}

lv_obj_t *SettingsView::SwitchBox_Create(lv_obj_t *par, const char *title)
{
    lv_obj_t *obj = lv_obj_create(par);
    lv_obj_create(obj);
    lv_obj_set_size(obj, lv_pct(100), lv_pct(25));
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(obj, LV_OPA_70, 0); // 设置透明度，不透明
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6d9fdc), 0);
    lv_obj_set_style_radius(obj, 12, 0); // 按钮画圆角
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tt = lv_label_create(obj);
    lv_label_set_text(tt, title);
    lv_obj_set_style_text_font(tt, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_align(tt, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t *sw = Switch_Create(obj, false, 70, 0);

    return obj;
}

lv_obj_t *SettingsView::SliderBox_Create(lv_obj_t *par, const char *title, int32_t min, int32_t max, int32_t val)
{
    lv_obj_t *obj = lv_obj_create(par);
    lv_obj_create(obj);
    lv_obj_set_size(obj, lv_pct(100), lv_pct(25));
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(obj, LV_OPA_70, 0); // 设置透明度，不透明
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x32a8ce), 0);
    lv_obj_set_style_radius(obj, 12, 0); // 按钮画圆角
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tt = lv_label_create(obj);
    lv_label_set_text(tt, title);
    lv_obj_set_style_text_font(tt, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_align(tt, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t *slider = Slider_Create(obj, LV_SYMBOL_SETTINGS, 40, 0);

    return obj;
}

lv_obj_t *SettingsView::ContentBox_Create(lv_obj_t *par, const char *title, const char *content)
{
    lv_obj_t *obj = lv_obj_create(par);

    lv_obj_create(obj);
    lv_obj_set_size(obj, lv_pct(100), lv_pct(25));
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(obj, LV_OPA_70, 0); // 设置透明度，不透明
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x52a1b2), 0);
    lv_obj_set_style_radius(obj, 12, 0); // 按钮画圆角
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tt = lv_label_create(obj);
    lv_obj_t *ct = lv_label_create(obj);

    lv_label_set_text(tt, title);

    lv_obj_set_style_text_font(tt, ResourcePool::GetFont("bahnschrift_17"), 0);

    lv_obj_align(tt, LV_ALIGN_LEFT_MID, 10, 0);

    lv_label_set_text(ct, content);

    lv_obj_set_width(ct, 100);

    lv_label_set_long_mode(ct, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(ct, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_set_style_text_color(ct, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(ct, LV_ALIGN_RIGHT_MID, -20, 0);

    return obj;
}

lv_obj_t *SettingsView::Switch_Create(lv_obj_t *par, bool state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *obj = lv_switch_create(par);

    lv_obj_set_size(obj, LV_HOR_RES / 5, LV_VER_RES / 8);

    lv_obj_set_style_anim_time(obj, 200, 0);

    lv_obj_set_style_pad_all(obj, -3, LV_PART_KNOB);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_KNOB);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2694bf), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x5f8b75), LV_PART_KNOB | LV_STATE_CHECKED);

    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xd2d6f1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa4d9b2), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_center(obj);

    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, y_ofs);

    lv_obj_add_state(obj, state ? LV_STATE_CHECKED : 0);

    return obj;
}

lv_obj_t *SettingsView::Slider_Create(lv_obj_t *par, const void *img_src, lv_coord_t x_ofs, lv_coord_t y_ofs, int32_t min, int32_t max, int32_t val)
{
    lv_obj_t *obj = lv_slider_create(par);

    lv_slider_set_mode(obj, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_range(obj, min, max);
    lv_slider_set_value(obj, val, LV_ANIM_OFF);

    lv_obj_set_size(obj, lv_pct(61), LV_VER_RES / 7);

    // lv_obj_set_style_border_width(obj, 4, LV_PART_KNOB);
    // lv_obj_set_style_pad_all(obj, 6, LV_PART_KNOB);
    // lv_obj_set_style_radius(obj, 20, LV_PART_KNOB);
    // lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_KNOB);
    // lv_obj_set_style_bg_color(obj, lv_color_hex(0x115588), LV_PART_KNOB);

    lv_obj_set_style_radius(obj, 8, LV_PART_MAIN);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3c9ba6), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_radius(obj, 8, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa4d9b2), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, y_ofs);

    if (img_src != nullptr)
    {
        lv_obj_t *img = lv_img_create(obj);
        lv_obj_align(img, LV_ALIGN_LEFT_MID, 10, 0);
        lv_img_set_src(img, img_src);
        // lv_obj_set_style_img_opa(img, LV_OPA_50, LV_PART_MAIN);
        // lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
        // lv_obj_set_style_img_recolor(img, lv_color_white(), LV_PART_MAIN);
    }

    return obj;
}

lv_obj_t *SettingsView::Btn_Create(lv_obj_t *par, const void *img_src, lv_coord_t x_ofs)
{
    lv_obj_t *obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 40, 31);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); // lv_obj_clear_flag用于清除指定对象的标志位。标志位是用来表示对象状态或特性的一个标记，通过设置或清除标志位可以改变对象的行为或外观。
                                                    // 就是说这里删去原本默认的按钮样式，为下面自定义添加图片做准备
    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, 0);
    lv_obj_set_style_bg_img_src(obj, img_src, 0); // lv_obj_set_style_bg_img_src用于设置对象的背景图像源。通过这个函数可以为指定的对象设置背景图像，可以是内存中的图片数据，也可以是文件路径

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);     // 设置透明度，不透明
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED); // 设置button按下时的长宽
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x666666), 0);                // 设置按钮默认的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xbbbbbb), LV_STATE_PRESSED); // 设置按钮在被按下时的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff931e), LV_STATE_FOCUSED); // 设置按钮在被聚焦时的颜色
    lv_obj_set_style_radius(obj, 9, 0);                                       // 按钮画圆角

    static lv_style_transition_dsc_t tran;                                                      // lv_style_transition_dsc_t用于描述样式过渡的属性。这个数据结构用于定义样式过渡的动画效果，包括过渡的时间、延迟、缓动函数等属性
    static const lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV}; // lv_style_prop_t枚举类型，用于表示样式的属性。这个枚举类型定义了一系列的样式属性，例如背景颜色、边框宽度、文本颜色等
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        200,
        0,
        nullptr); // 为啥要做这个初始化呢？原因是如果不做这个初始化，你按下的样式也会发生改变，但是就是两个图片的切换，而没有过渡动画
    lv_obj_set_style_transition(obj, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(obj); // 真伟大，但是看不懂，没有循环刷新，怎么改变layout？

    return obj;
}

void SettingsView::AppearAnimStart(bool reverse) // 开始开场动画
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
