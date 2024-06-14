#include "DialplateView.h"
#include <stdarg.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0])) // ��ȡ�����С

using namespace Page;

void DialplateView::Create(lv_obj_t* root) // ��ʼ������
{
    BottomInfo_Create(root); // ������飬�ֳ�������ʼ��
    TopInfo_Create(root);
    BtnCont_Create(root);

    ui.anim_timeline = lv_anim_timeline_create(); // lv_anim_timeline_create���ڴ���һ��ʱ���ᶯ����ʱ���ᶯ����һ�ֻ���ʱ��Ķ���������ͨ����һ��ʱ���ڶԶ������Խ������޸���ʵ��
// �궨������õĳ����뻯������Ľ���������˼�Ƕ������ƽṹ���Ա������wrapper[]���õ���������صĹ�
#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}
//����Ҳ�Ƕ���ṹ���Ա����͸���ȱ仯��ص�
#define ANIM_OPA_DEF(start_time, obj) \
    ANIM_DEF(start_time, obj, opa_scale, LV_OPA_TRANSP, LV_OPA_COVER)

    lv_coord_t y_tar_top = lv_obj_get_y(ui.topInfo.cont); // emm cont�������������ɣ���ʵ���ǰ���TopInfo��С������root�Ǵ󻭲������Ӧ���ܶ���
    lv_coord_t y_tar_bottom = lv_obj_get_y(ui.bottomInfo.cont);
    lv_coord_t h_tar_btn = lv_obj_get_height(ui.btnCont.btnRec);

    lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, ui.topInfo.cont, y, -lv_obj_get_height(ui.topInfo.cont), y_tar_top), // ���ﶯ���ǴӶ������»�������

        ANIM_DEF(200, ui.bottomInfo.cont, y, -lv_obj_get_height(ui.bottomInfo.cont), y_tar_bottom),// ���ﶯ���ǴӶ������»�������
        ANIM_OPA_DEF(200, ui.bottomInfo.cont),

        ANIM_DEF(500, ui.btnCont.btnMap, height, 0, h_tar_btn),
        ANIM_DEF(600, ui.btnCont.btnRec, height, 0, h_tar_btn),
        ANIM_DEF(700, ui.btnCont.btnMenu, height, 0, h_tar_btn),
        LV_ANIM_TIMELINE_WRAPPER_END // �����־�Žṹ���Ա����������ʡ�ԣ������溯��lv_anim_timeline_add_wrapper����ѯ�����ж�����
    };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void DialplateView::Delete()
{
    if(ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void DialplateView::TopInfo_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par); // par���������Ǵ󻭲�������cont�����Դ󻭲���Ϊ������
    lv_obj_remove_style_all(cont);       // lv_obj_remove_style_all���ڴӶ�����ɾ��������ʽ���������������������������Ѿ�Ӧ�õ�������ʽ��ʹ����ָ���Ĭ�ϵ���ۡ�
    lv_obj_set_size(cont, LV_HOR_RES, 142); // ����cont�����Ĵ�С��ˮƽ�����Ĭ�ϵģ��߶���Ϊ142��240*240��

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0); // ����cont������ȫ��͸����0��ʾĬ��״̬����Ȼ����ʲô��������״̬���۽�״̬֮���
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x333333), 0); // ����cont�����ı���ɫ

    lv_obj_set_style_radius(cont, 27, 0); // lv_obj_set_style_radius�������ö����Բ�ǰ뾶���������û�Ϊ�ض��Ķ����簴ť�������ȣ�����Բ�ǣ�ʹ��ӵ��Բ�����ۡ�
    lv_obj_set_y(cont, -36); // ����cont������y���꣬��ס�����Բ�ǣ�ֻ¶�������Բ�ǣ�good��
    ui.topInfo.cont = cont; // ��cont������ָ�봫���ṹ�����cont

    lv_obj_t* label = lv_label_create(cont); // ����cont��������label����ʾ����
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_65"), 0); // ����������ʽ
    lv_obj_set_style_text_color(label, lv_color_white(), 0); // ����������ɫ
    lv_label_set_text(label, "00"); // ���ǩΪ��00������
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 63); // ���ǩ�����У�y����ƫ��63
    ui.topInfo.labelSpeed = label;                // ��labelָ�봫����labelSpeed����������Ϳ��Խ����޸�label

    label = lv_label_create(cont); // ����cont�����ٴδ���label����ʾ��λ
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("bahnschrift_17"), 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "km/h");
    lv_obj_align_to(label, ui.topInfo.labelSpeed, LV_ALIGN_OUT_BOTTOM_MID, 0, 8); // LV_ALIGN_OUT_BOTTOM_MID��ʾ����ĵײ����ĵ����丸�������ĵײ����룬���Ҷ�����ˮƽ�����Ͼ��ж��롣
    ui.topInfo.labelUint = label;
}

void DialplateView::BottomInfo_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_style_bg_color(cont, lv_color_black(), 0); // ����cont����������ɫΪ��ɫ
    lv_obj_set_size(cont, LV_HOR_RES, 90);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 106);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP); // lv_obj_set_flex_flow�������ö���� Flex ����������Flex ������һ�����ڴ������ġ�����Ӧ���û����沼�ֵļ����������԰��������ɵ�ʵ�ֵ��Ժ���Ӧʽ��ơ�
                                                       // LV_FLEX_FLOW_ROW_WRAP�������ö���� Flex ����������Ϊˮƽ���򲼾ֲ��һ��С�
    lv_obj_set_flex_align(                             // lv_obj_set_flex_align�������� Flex �����еĶ��뷽ʽ�������԰��������ɵ�ʵ�ֵ��Ժ���Ӧʽ��ơ�
        cont,
        LV_FLEX_ALIGN_SPACE_EVENLY, // ������򽻲�����ƽ������ռ䣬������β
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
        );

    ui.bottomInfo.cont = cont;

    const char* unitText[4] =
    {
        "AVG",
        "Time",
        "Trip",
        "Calorie"
    };

    for (int i = 0; i < ARRAY_SIZE(ui.bottomInfo.labelInfoGrp); i++) // ��ѯ�����Ϣ��ǩ������ϸ�ڰ�������С��ǩҲ�����������
    {
        SubInfoGrp_Create(
            cont,
            &(ui.bottomInfo.labelInfoGrp[i]),
            unitText[i]
        );
    }
}

void DialplateView::SubInfoGrp_Create(lv_obj_t* par, SubInfo_t* info, const char* unitText) // ������Ϣ��ǩ��gps��time����·���
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 93, 39);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(
        cont,
        LV_FLEX_ALIGN_SPACE_AROUND, // ������򽻲�����ƽ������ռ�
        LV_FLEX_ALIGN_CENTER, // ������򽻲��������λ�ö���
        LV_FLEX_ALIGN_CENTER
    );

    lv_obj_t* label = lv_label_create(cont);
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

void DialplateView::BtnCont_Create(lv_obj_t* par) // ��ť���������Ĵ���
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, 40);
    lv_obj_align_to(cont, ui.bottomInfo.cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); // lv_obj_align_to��Ը����������ж���

    /*lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_place(
        cont,
        LV_FLEX_PLACE_SPACE_AROUND,
        LV_FLEX_PLACE_CENTER,
        LV_FLEX_PLACE_CENTER
    );*/

    ui.btnCont.cont = cont;

    ui.btnCont.btnMap = Btn_Create(cont, ResourcePool::GetImage("locate"), -80); // ��������button
    ui.btnCont.btnRec = Btn_Create(cont, ResourcePool::GetImage("start"), 0);
    ui.btnCont.btnMenu = Btn_Create(cont, ResourcePool::GetImage("menu"), 80);
}

lv_obj_t* DialplateView::Btn_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs)
{
    lv_obj_t* obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 40, 31);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); // lv_obj_clear_flag�������ָ������ı�־λ����־λ��������ʾ����״̬�����Ե�һ����ǣ�ͨ�����û������־λ���Ըı�������Ϊ����ۡ�
                                                    // ����˵����ɾȥԭ��Ĭ�ϵİ�ť��ʽ��Ϊ�����Զ������ͼƬ��׼��
    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, 0);
    lv_obj_set_style_bg_img_src(obj, img_src, 0); // lv_obj_set_style_bg_img_src�������ö���ı���ͼ��Դ��ͨ�������������Ϊָ���Ķ������ñ���ͼ�񣬿������ڴ��е�ͼƬ���ݣ�Ҳ�������ļ�·��

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0); //����͸���ȣ���͸��
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED); // ����button����ʱ�ĳ���
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x666666), 0); //���ð�ťĬ�ϵ���ɫ
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xbbbbbb), LV_STATE_PRESSED); // ���ð�ť�ڱ�����ʱ����ɫ
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff931e), LV_STATE_FOCUSED); // ���ð�ť�ڱ��۽�ʱ����ɫ
    lv_obj_set_style_radius(obj, 9, 0); // ��ť��Բ��

    static lv_style_transition_dsc_t tran; // lv_style_transition_dsc_t����������ʽ���ɵ����ԡ�������ݽṹ���ڶ�����ʽ���ɵĶ���Ч�����������ɵ�ʱ�䡢�ӳ١���������������
    static const lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV}; // lv_style_prop_tö�����ͣ����ڱ�ʾ��ʽ�����ԡ����ö�����Ͷ�����һϵ�е���ʽ���ԣ����米����ɫ���߿��ȡ��ı���ɫ��
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        200,
        0,
        nullptr
    ); // ΪɶҪ�������ʼ���أ�ԭ����������������ʼ�����㰴�µ���ʽҲ�ᷢ���ı䣬���Ǿ�������ͼƬ���л�����û�й��ɶ���
    lv_obj_set_style_transition(obj, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(obj); // ��ΰ�󣬵��ǿ�������û��ѭ��ˢ�£���ô�ı�layout��

    return obj;
}

void DialplateView::AppearAnimStart(bool reverse) // ��ʼ��������
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
