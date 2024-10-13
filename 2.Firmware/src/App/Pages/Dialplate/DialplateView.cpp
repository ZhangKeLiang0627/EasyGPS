#include "DialplateView.h"
#include <stdarg.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) // 获取数组大小

using namespace Page;

void DialplateView::Create(lv_obj_t *root) // 初始化画布
{
    BottomInfo_Create(root); // 三个大块，分成三个初始化
    TopInfo_Create(root);
    BtnCont_Create(root);

    ui.anim_timeline = lv_anim_timeline_create(); // lv_anim_timeline_create用于创建一个时间轴动画。时间轴动画是一种基于时间的动画，可以通过在一定时间内对对象属性进行逐步修改来实现
// 宏定义真的用的出神入化啊，羡慕！这里的意思是定义类似结构体成员，下面wrapper[]里用到，动画相关的哈
#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}
// 这里也是定义结构体成员，和透明度变化相关的
#define ANIM_OPA_DEF(start_time, obj) \
    ANIM_DEF(start_time, obj, opa_scale, LV_OPA_TRANSP, LV_OPA_COVER)

    lv_coord_t y_tar_top = lv_obj_get_y(ui.topInfo.cont); // emm cont这里理解成容器吧，其实就是包含TopInfo的小画布，root是大画布嘛，这样应该能懂吧
    lv_coord_t y_tar_bottom = lv_obj_get_y(ui.bottomInfo.cont);
    lv_coord_t h_tar_btn = lv_obj_get_height(ui.btnCont.btnRec);

    lv_anim_timeline_wrapper_t wrapper[] =
        {
            ANIM_DEF(0, ui.topInfo.cont, y, -lv_obj_get_height(ui.topInfo.cont), y_tar_top), // 这里动画是从顶上往下滑出来的

            ANIM_DEF(200, ui.bottomInfo.cont, y, -lv_obj_get_height(ui.bottomInfo.cont), y_tar_bottom), // 这里动画是从顶上往下滑出来的
            ANIM_OPA_DEF(200, ui.bottomInfo.cont),

            ANIM_DEF(500, ui.btnCont.btnMap, height, 0, h_tar_btn),
            ANIM_DEF(600, ui.btnCont.btnRec, height, 0, h_tar_btn),
            ANIM_DEF(700, ui.btnCont.btnMenu, height, 0, h_tar_btn),
            LV_ANIM_TIMELINE_WRAPPER_END // 这个标志着结构体成员结束，不能省略，在下面函数lv_anim_timeline_add_wrapper的轮询中做判断条件
        };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void DialplateView::Delete()
{
    if (ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void DialplateView::TopInfo_Create(lv_obj_t *par)
{
    lv_obj_t *cont = lv_obj_create(par);    // par传进来的是大画布，所以cont这里以大画布作为父对象
    lv_obj_remove_style_all(cont);          // lv_obj_remove_style_all用于从对象中删除所有样式。这个函数可以用于清除对象上已经应用的所有样式，使对象恢复到默认的外观。
    lv_obj_set_size(cont, LV_HOR_RES, 142); // 设置cont画布的大小，水平宽度是默认的，高度设为142（240*240）

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0);             // 设置cont画布完全不透明，0表示默认状态，当然还有什么按键按下状态，聚焦状态之类的
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x333333), 0); // 设置cont画布的背景色

    lv_obj_set_style_radius(cont, 27, 0); // lv_obj_set_style_radius用于设置对象的圆角半径。它允许用户为特定的对象（如按钮、容器等）设置圆角，使其拥有圆润的外观。
    lv_obj_set_y(cont, -36);              // 设置cont画布的y坐标，藏住上面的圆角，只露出下面的圆角，good！
    ui.topInfo.cont = cont;               // 把cont画布的指针传给结构体里的cont

    lv_obj_t *label = lv_label_create(cont);                                       // 基于cont画布创建label，显示码速
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_65"), 0); // 设置字体样式
    lv_obj_set_style_text_color(label, lv_color_white(), 0);                       // 设置字体颜色
    lv_label_set_text(label, "00");                                                // 设标签为‘00’字样
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 63);                                  // 设标签顶居中，y向下偏移63
    ui.topInfo.labelSpeed = label;                                                 // 将label指针传给了labelSpeed，所以下面就可以接着修改label

    label = lv_label_create(cont); // 基于cont画布再次创建label，显示单位
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "km/h");
    lv_obj_align_to(label, ui.topInfo.labelSpeed, LV_ALIGN_OUT_BOTTOM_MID, 0, 8); // LV_ALIGN_OUT_BOTTOM_MID表示对象的底部中心点与其父级容器的底部对齐，并且对象在水平方向上居中对齐。
    ui.topInfo.labelUint = label;
}

void DialplateView::BottomInfo_Create(lv_obj_t *par)
{
    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_style_bg_color(cont, lv_color_black(), 0); // 设置cont画布背景颜色为黑色
    lv_obj_set_size(cont, LV_HOR_RES, 90);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 106);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP); // lv_obj_set_flex_flow用于设置对象的 Flex 布局流方向。Flex 布局是一种用于创建灵活的、自适应的用户界面布局的技术，它可以帮助你轻松地实现弹性和响应式设计。
                                                       // LV_FLEX_FLOW_ROW_WRAP用于设置对象的 Flex 布局流方向为水平方向布局并且换行。
    lv_obj_set_flex_align(                             // lv_obj_set_flex_align用于设置 Flex 布局中的对齐方式。它可以帮助你轻松地实现弹性和响应式设计。
        cont,
        LV_FLEX_ALIGN_SPACE_EVENLY, // 在主轴或交叉轴上平均分配空间，包括首尾
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER);

    ui.bottomInfo.cont = cont;

    const char *unitText[4] =
        {
            "AVG",
            "Time",
            "Trip",
            "Calorie"};

    for (int i = 0; i < ARRAY_SIZE(ui.bottomInfo.labelInfoGrp); i++) // 轮询添加信息标签，它好细节啊！就连小标签也搞个容器存着
    {
        SubInfoGrp_Create(
            cont,
            &(ui.bottomInfo.labelInfoGrp[i]),
            unitText[i]);
    }
}

void DialplateView::SubInfoGrp_Create(lv_obj_t *par, SubInfo_t *info, const char *unitText) // 创建信息标签（gps、time、卡路里）等
{
    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 93, 39);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(
        cont,
        LV_FLEX_ALIGN_SPACE_AROUND, // 在主轴或交叉轴上平均分配空间
        LV_FLEX_ALIGN_CENTER,       // 在主轴或交叉轴的中心位置对齐
        LV_FLEX_ALIGN_CENTER);

    lv_obj_t *label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    info->lableValue = label;

    label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_13"), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xb3b3b3), 0);
    lv_label_set_text(label, unitText);
    info->lableUnit = label;

    info->cont = cont;
}

void DialplateView::BtnCont_Create(lv_obj_t *par) // 按钮容器画布的创建
{
    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, 40);
    lv_obj_align_to(cont, ui.bottomInfo.cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); // lv_obj_align_to相对父类容器进行对齐

    /*lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_place(
        cont,
        LV_FLEX_PLACE_SPACE_AROUND,
        LV_FLEX_PLACE_CENTER,
        LV_FLEX_PLACE_CENTER
    );*/

    ui.btnCont.cont = cont;

    ui.btnCont.btnMap = Btn_Create(cont, ResourcePool::GetImage("locate"), -80); // 创建三个button
    ui.btnCont.btnRec = Btn_Create(cont, ResourcePool::GetImage("start"), 0);
    ui.btnCont.btnMenu = Btn_Create(cont, ResourcePool::GetImage("menu"), 80);
}

lv_obj_t *DialplateView::Btn_Create(lv_obj_t *par, const void *img_src, lv_coord_t x_ofs)
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

void DialplateView::AppearAnimStart(bool reverse) // 开始开场动画
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
