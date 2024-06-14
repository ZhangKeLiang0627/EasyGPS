#include "TemplateView.h"
#include <stdarg.h>
#include <stdio.h>

using namespace Page;

void TemplateView::Create(lv_obj_t *root)
{
    BottomInfo_Create(root); // 三个大块，分成三个初始化
    TopInfo_Create(root);
    BtnCont_Create(root);
    // 动画的创建
    ui.anim_timeline = lv_anim_timeline_create(); // lv_anim_timeline_create用于创建一个时间轴动画。时间轴动画是一种基于时间的动画，可以通过在一定时间内对对象属性进行逐步修改来实现
    ui.anim_timelineForSayHi = lv_anim_timeline_create();

// 宏定义真的用的出神入化啊，羡慕！这里的意思是定义类似结构体成员，下面wrapper[]里用到，动画相关的哈
#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    lv_anim_timeline_wrapper_t wrapper[] =
        {
            ANIM_DEF(0, ui.bottomInfo.cont, height, 20, lv_obj_get_height(ui.bottomInfo.cont)),                               // 这里动画是从顶上往下滑出来的
            ANIM_DEF(0, ui.bottomInfo.cont, width, 20, lv_obj_get_width(ui.bottomInfo.cont)),                                 // 这里动画是从顶上往下滑出来的
            ANIM_DEF(200, ui.topInfo.cont, y, -lv_obj_get_height(ui.topInfo.cont), lv_obj_get_y(ui.topInfo.cont)),            // 这里动画是从顶上往下滑出来的
            ANIM_DEF(300, ui.btnCont.cont, y, 240 + lv_obj_get_height(ui.btnCont.cont), 240 - lv_obj_get_y(ui.btnCont.cont)), // 这里动画是从顶上往下滑出来的
                                                                                                                              // ANIM_OPA_DEF(200, ui.topInfo.labelHello),

            ANIM_DEF(700, ui.btnCont.btnSayHi, height, 10, lv_obj_get_height(ui.btnCont.btnSayHi)),
            ANIM_DEF(900, ui.btnCont.btnMenu, height, 10, lv_obj_get_height(ui.btnCont.btnMenu)),
            LV_ANIM_TIMELINE_WRAPPER_END // 这个标志着结构体成员结束，不能省略，在下面函数lv_anim_timeline_add_wrapper的轮询中做判断条件
        };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);

    lv_anim_timeline_wrapper_t wrapperForSayHi[] =
        {
            ANIM_DEF(0, ui.btnCont.labelSayHi, y, -50, lv_obj_get_y(ui.btnCont.labelSayHi) - 110),
            {0, ui.btnCont.labelSayHi, (lv_anim_exec_xcb_t)lv_label_set_opa_scale, LV_OPA_TRANSP, LV_OPA_COVER, 300, lv_anim_path_ease_out, true},
            {600, ui.btnCont.labelSayHi, (lv_anim_exec_xcb_t)lv_label_set_opa_scale, LV_OPA_COVER, LV_OPA_TRANSP, 300, lv_anim_path_ease_out, true},
            LV_ANIM_TIMELINE_WRAPPER_END // 这个标志着结构体成员结束，不能省略，在下面函数lv_anim_timeline_add_wrapper的轮询中做判断条件
        };
    lv_anim_timeline_add_wrapper(ui.anim_timelineForSayHi, wrapperForSayHi);
}

void TemplateView::Delete()
{
    if (ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        lv_anim_timeline_del(ui.anim_timelineForSayHi);

        ui.anim_timeline = nullptr;
        ui.anim_timelineForSayHi = nullptr;
    }
}

void TemplateView::AppearAnimStart(bool reverse) // 开始开场动画
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}

void TemplateView::AppearAnimSayHi(bool reverse) // 开始开场动画
{
    lv_anim_timeline_set_reverse(ui.anim_timelineForSayHi, reverse);
    lv_anim_timeline_start(ui.anim_timelineForSayHi);
}

void TemplateView::TopInfo_Create(lv_obj_t *par)
{
    lv_obj_t *cont = lv_obj_create(par);    // par传进来的是大画布，所以cont这里以大画布作为父对象
    lv_obj_remove_style_all(cont);          // lv_obj_remove_style_all用于从对象中删除所有样式。这个函数可以用于清除对象上已经应用的所有样式，使对象恢复到默认的外观。
    lv_obj_set_size(cont, LV_HOR_RES, 150); // 设置cont画布的大小，水平宽度是默认的，高度设为150（240*240）

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0);             // 设置cont画布完全不透明，0表示默认状态，当然还有什么按键按下状态，聚焦状态之类的
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x32cd32), 0); // 设置cont画布的背景色 0x32cd32酸橙绿，科科！

    lv_obj_set_style_radius(cont, 42, 0); // lv_obj_set_style_radius用于设置对象的圆角半径。它允许用户为特定的对象（如按钮、容器等）设置圆角，使其拥有圆润的外观。
                                          //    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_y(cont, -36);              // 设置cont画布的y坐标，藏住上面的圆角，只露出下面的圆角，good！
    ui.topInfo.cont = cont;               // 把cont画布的指针传给结构体里的cont

    lv_obj_t *label = lv_label_create(cont);                                       // 基于cont画布创建label，显示码速
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_65"), 0); // 设置字体样式
    lv_obj_set_style_text_color(label, lv_color_hex(0x323332), 0);                 // 设置字体颜色
    lv_label_set_text(label, "00");                                               // 设标签为‘00’字样
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 63);                                  // 设标签顶居中，y向下偏移63
    ui.topInfo.labelHello = label;                                                 // 将label指针传给了labelSpeed，所以下面就可以接着修改label

    label = lv_label_create(cont); // 基于cont画布再次创建label，显示单位
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "/YourClick");
    lv_obj_align_to(label, ui.topInfo.labelHello, LV_ALIGN_OUT_BOTTOM_MID, 0, 8); // LV_ALIGN_OUT_BOTTOM_MID表示对象的底部中心点与其父级容器的底部对齐，并且对象在水平方向上居中对齐。
    ui.topInfo.labelUint = label;
}

void TemplateView::BottomInfo_Create(lv_obj_t *par)
{
    lv_obj_t *cont = lv_obj_create(par); // 继承父画布
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 300, 300);

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0);             // 设置cont画布完全不透明，0表示默认状态，当然还有什么按键按下状态，聚焦状态之类的
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x8a2be2), 0); // 设置cont画布背景颜色为0x8a2be2，湖紫色

    lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0); // 将画布改为圆形
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 30);         // 放到中心位置

    ui.bottomInfo.cont = cont; // 把cont画布的指针传给结构体里的cont
}

void TemplateView::BtnCont_Create(lv_obj_t *par) // 按钮容器画布的创建
{
    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, 150);

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0); // 设置cont画布完全不透明，0表示默认状态，当然还有什么按键按下状态，聚焦状态之类的
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xf44e21), 0);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 100);
    lv_obj_set_style_radius(cont, 42, 0); // lv_obj_set_style_radius用于设置对象的圆角半径。它允许用户为特定的对象（如按钮、容器等）设置圆角，使其拥有圆润的外观。

    ui.btnCont.btnSayHi = Btn_Create(cont, ResourcePool::GetImage("start"), -45); // 创建三个button
    ui.btnCont.btnMenu = Btn_Create(cont, ResourcePool::GetImage("menu"), -5);

    lv_obj_t *label = lv_label_create(par);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_set_style_text_opa(label, LV_OPA_TRANSP, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "YourClick + 1");
    // lv_obj_align_to(label, par, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 35, -90);
    ui.btnCont.labelSayHi = label;

    ui.btnCont.cont = cont;
}

lv_obj_t *TemplateView::Btn_Create(lv_obj_t *par, const void *img_src, lv_coord_t y_ofs)
{
    lv_obj_t *obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 88, 31);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); // lv_obj_clear_flag用于清除指定对象的标志位。标志位是用来表示对象状态或特性的一个标记，通过设置或清除标志位可以改变对象的行为或外观。
                                                    // 就是说这里删去原本默认的按钮样式，为下面自定义添加图片做准备
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, y_ofs);
    lv_obj_set_style_bg_img_src(obj, img_src, 0); // lv_obj_set_style_bg_img_src用于设置对象的背景图像源。通过这个函数可以为指定的对象设置背景图像，可以是内存中的图片数据，也可以是文件路径

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);     // 设置透明度，不透明
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED); // 设置button按下时的长宽
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x356b8c), 0);                // 设置按钮默认的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x242947), LV_STATE_PRESSED); // 设置按钮在被按下时的颜色
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf2daaa), LV_STATE_FOCUSED); // 设置按钮在被聚焦时的颜色
    lv_obj_set_style_radius(obj, 9, 0);                                       // 按钮画圆角

    static lv_style_transition_dsc_t tran;                                                      // lv_style_transition_dsc_t用于描述样式过渡的属性。这个数据结构用于定义样式过渡的动画效果，包括过渡的时间、延迟、缓动函数等属性
    static const lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV}; // lv_style_prop_t枚举类型，用于表示样式的属性。这个枚举类型定义了一系列的样式属性，例如背景颜色、边框宽度、文本颜色等
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        150,
        0,
        nullptr); // 为啥要做这个初始化呢？原因是如果不做这个初始化，你按下的样式也会发生改变，但是就是两个图片的切换，而没有过渡动画
    lv_obj_set_style_transition(obj, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(obj); // 真伟大，但是看不懂，没有循环刷新，怎么改变layout？

    return obj;
}
