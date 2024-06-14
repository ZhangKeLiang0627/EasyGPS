#include "BLEKeyboard.h"

using namespace Page;

BLEKeyboard::BLEKeyboard()
    : timer(nullptr)
{
}

BLEKeyboard::~BLEKeyboard()
{
}

void BLEKeyboard::onCustomAttrConfig()
{
    LV_LOG_USER("onCustomAttrConfig");
    // SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE); // 设置当前的动画样式（？
}

void BLEKeyboard::onViewLoad()
{
    LV_LOG_USER("onViewLoad");
    Model.Init();
    View.Create(_root);

    /* Be sure the fist button is in the middle */
    lv_obj_scroll_to_view(lv_obj_get_child(View.ui.cont, 0), LV_ANIM_ON);

    // 添加事件回调
    AttachEvent(lv_scr_act());
    // AttachEvent(View.ui.mouseMoveCtrlCont.btnMouse);
    // AttachEvent(View.ui.mouseMoveCtrlCont.cont);

    lv_obj_add_event_cb(View.ui.musicCtrlCont.btnNextTrack.btn, (lv_event_cb_t)musicCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.musicCtrlCont.btnPause.btn, (lv_event_cb_t)musicCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.musicCtrlCont.btnPrevTrack.btn, (lv_event_cb_t)musicCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.musicCtrlCont.btnVolumeDown.btn, (lv_event_cb_t)musicCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.musicCtrlCont.btnVolumeUp.btn, (lv_event_cb_t)musicCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.musicCtrlCont.btnMute.btn, (lv_event_cb_t)musicCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);

    lv_obj_add_event_cb(View.ui.directionCtrlCont.btnDirUp.btn, (lv_event_cb_t)directionCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.directionCtrlCont.btnDirDown.btn, (lv_event_cb_t)directionCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.directionCtrlCont.btnCopy.btn, (lv_event_cb_t)directionCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.directionCtrlCont.btnPaste.btn, (lv_event_cb_t)directionCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.directionCtrlCont.btnCut.btn, (lv_event_cb_t)directionCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.directionCtrlCont.btnWithdraw.btn, (lv_event_cb_t)directionCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);

    lv_obj_add_event_cb(View.ui.mouseMoveCtrlCont.cont, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);

    lv_obj_add_event_cb(View.ui.mouseMoveCtrlCont.btnMouse, (lv_event_cb_t)mouseMoveCtrlBtnHandler, LV_EVENT_PRESSING, this);

    lv_obj_add_event_cb(View.ui.mouseClickCtrlCont.btnLeft.btn, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.mouseClickCtrlCont.btnRight.btn, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.mouseClickCtrlCont.btnBack.btn, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.mouseClickCtrlCont.btnForward.btn, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.mouseClickCtrlCont.btnPageUp.btn, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(View.ui.mouseClickCtrlCont.btnPageDown.btn, (lv_event_cb_t)mouseClickCtrlBtnHandler, LV_EVENT_SHORT_CLICKED, this);
}

void BLEKeyboard::onViewDidLoad()
{
    LV_LOG_USER("onViewDidLoad");
}

void BLEKeyboard::onViewWillAppear()
{
    LV_LOG_USER("onViewWillAppear");

    // lv_group_t *group = lv_group_get_default(); // 用于获取LittlevGL中默认的组对象
    // LV_ASSERT_NULL(group);

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP);
    Model.SetStatusBarAppear(true);

    Update(); // 刷新，第一次加载界面，需要刷新一下，不然数值不显示会卡一下才显示

    lv_obj_fade_in(_root, 350, 0);

    // View.AppearAnimStart(); // 开始加载进入该page的动画，出场动画 // 填flase是动画正着放，true是反着放
}

void BLEKeyboard::onViewDidAppear()
{
    LV_LOG_USER("onViewDidAppear");
    timer = lv_timer_create(onTimerUpdate, 2000, this); // 这里设置一个2s的定时器，软定时器，用于在onTimerUpdate里update
}

void BLEKeyboard::onViewWillDisappear()
{
    LV_LOG_USER("onViewWillDisappear");

    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);

    lv_group_remove_all_objs(group);

    lv_timer_del(timer); // 删除该页面的软定时器

    lv_obj_fade_out(_root, 350, 0);
}

void BLEKeyboard::onViewDidDisappear()
{
    LV_LOG_USER("onViewDidDisappear");
}

void BLEKeyboard::onViewUnload()
{
    LV_LOG_USER("onViewUnload");

    // View.Delete(); // 删除该页面的UI指针，释放容器的内存
    Model.Deinit();

    lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数
}

void BLEKeyboard::onViewDidUnload()
{
    LV_LOG_USER("onViewDidUnload");
}

void BLEKeyboard::AttachEvent(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void BLEKeyboard::Update()
{
    // printf("[BLE] Hi!\n");
    if (lv_obj_get_state(View.ui.mouseMoveCtrlCont.btnMouse) != LV_STATE_PRESSED)
    {
        lv_obj_add_flag(View.ui.cont, LV_OBJ_FLAG_SCROLLABLE);
        AttachEvent(lv_scr_act());
    }
}

void BLEKeyboard::onTimerUpdate(lv_timer_t *timer)
{
    BLEKeyboard *instance = (BLEKeyboard *)timer->user_data;

    instance->Update();
}

void BLEKeyboard::onEvent(lv_event_t *event)
{
    BLEKeyboard *instance = (BLEKeyboard *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    // if (code == LV_EVENT_SHORT_CLICKED)
    // {
    //     if (obj == instance->View.ui.mouseMoveCtrlCont.btnMouse)
    //     {
    //         instance->Model.SendBLEMouseClickValue(1);
    //     }
    // }

    // if (code == LV_EVENT_PRESSING)
    // {
    //     static lv_point_t pointLast = {140, 120};
    //     lv_point_t pointNow = {140, 120};
    //     uint8_t Val[4] = {0};
    //     lv_coord_t x = 0;
    //     lv_coord_t y = 0;
    //     if (obj == instance->View.ui.mouseMoveCtrlCont.cont)
    //     {
    //         if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
    //         {
    //             lv_obj_clear_flag(instance->View.ui.cont, LV_OBJ_FLAG_SCROLLABLE);
    //             lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数
    //             lv_indev_get_point(lv_indev_get_act(), &pointNow);

    //             // lv_obj_set_style_translate_x(obj, (pointNow.x - 140), 0);
    //             // lv_obj_set_style_translate_y(obj, (pointNow.y - 140), 0);

    //             x = pointNow.x - pointLast.x;
    //             y = pointNow.y - pointLast.y;

    //             if (LV_ABS(x) > 80 || LV_ABS(y) > 80)
    //             {
    //                 // record
    //                 pointLast.x = pointNow.x;
    //                 pointLast.y = pointNow.y;
    //                 return;
    //             }

    //             Val[0] = x > 0 ? x * 2 : x * 2;
    //             Val[1] = y < 0 ? y * 2 : y * 2;

    //             instance->Model.SendBLEMouseMoveValue(Val);
    //             // printf("[LVGL] x: %d / y: %d \n", pointNow.x, pointNow.y);

    //             // record
    //             pointLast.x = pointNow.x;
    //             pointLast.y = pointNow.y;
    //         }
    //     }
    // }

    if (code == LV_EVENT_GESTURE)
    {
        lv_indev_wait_release(lv_indev_get_act());

        switch (lv_indev_get_gesture_dir(lv_indev_get_act()))
        {
        case LV_DIR_LEFT:
            instance->_Manager->Pop();
            break;
        case LV_DIR_RIGHT:
            instance->_Manager->Pop();
            break;
        case LV_DIR_TOP:

            break;
        case LV_DIR_BOTTOM:

            break;
        default:
            break;
        }
    }
}

void BLEKeyboard::musicCtrlBtnHandler(lv_event_t *event)
{
    BLEKeyboard *instance = (BLEKeyboard *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    uint8_t Val[2] = {0, 0};

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.musicCtrlCont.btnVolumeDown.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 64;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.musicCtrlCont.btnVolumeUp.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 32;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.musicCtrlCont.btnMute.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 16;

                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.musicCtrlCont.btnNextTrack.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 1;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.musicCtrlCont.btnPause.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 8;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.musicCtrlCont.btnPrevTrack.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 2;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
    }
}

void BLEKeyboard::directionCtrlBtnHandler(lv_event_t *event)
{
    BLEKeyboard *instance = (BLEKeyboard *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    uint8_t Val[2] = {0xff, 0xff};

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.directionCtrlCont.btnDirUp.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 0xda;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.directionCtrlCont.btnDirDown.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 0xd9;
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.directionCtrlCont.btnCut.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 0x80; // Ctrl
                Val[1] = 0x78; // x
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.directionCtrlCont.btnWithdraw.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 0x80; // Ctrl
                Val[1] = 0x7A; // z
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.directionCtrlCont.btnPaste.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 0x80; // Ctrl
                Val[1] = 0x76; // v
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
        else if (obj == instance->View.ui.directionCtrlCont.btnCopy.btn)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                Val[0] = 0x80; // Ctrl
                Val[1] = 0x63; // c
                instance->Model.SendBLEKeyboardValue(Val);
            }
        }
    }
}

void BLEKeyboard::mouseMoveCtrlBtnHandler(lv_event_t *event)
{
    BLEKeyboard *instance = (BLEKeyboard *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSING)
    {
        static lv_point_t pointLast = {140, 120};
        lv_point_t pointNow = {140, 120};
        uint8_t Val[4] = {0};
        lv_coord_t x = 0;
        lv_coord_t y = 0;
        if (obj == instance->View.ui.mouseMoveCtrlCont.btnMouse)
        {
            if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
            {
                lv_obj_clear_flag(instance->View.ui.cont, LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_remove_event_cb(lv_scr_act(), onEvent); // 移除屏幕手势回调函数
                lv_indev_get_point(lv_indev_get_act(), &pointNow);

                x = pointNow.x - pointLast.x;
                y = pointNow.y - pointLast.y;

                if (LV_ABS(x) > 30 || LV_ABS(y) > 30)
                {
                    // record
                    pointLast.x = pointNow.x;
                    pointLast.y = pointNow.y;
                    return;
                }

                Val[0] = x > 0 ? x * 2 : x * 2;
                Val[1] = y < 0 ? y * 2 : y * 2;

                instance->Model.SendBLEMouseMoveValue(Val);
                // printf("[LVGL] x: %d / y: %d \n", pointNow.x, pointNow.y);

                // record
                pointLast.x = pointNow.x;
                pointLast.y = pointNow.y;
            }
        }
    }
}

void BLEKeyboard::mouseClickCtrlBtnHandler(lv_event_t *event)
{
    BLEKeyboard *instance = (BLEKeyboard *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    uint8_t Val[4] = {0};

    /** Click Value
     *  MOUSE_LEFT 1
     *  MOUSE_RIGHT 2
     *  MOUSE_MIDDLE 4
     *  MOUSE_BACK 8
     *  MOUSE_FORWARD 16
     */
    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if (obj == instance->View.ui.mouseMoveCtrlCont.cont)
        {
            instance->Model.SendBLEMouseClickValue(1);
        }
        else if (obj == instance->View.ui.mouseClickCtrlCont.btnLeft.btn)
        {
            instance->Model.SendBLEMouseClickValue(1);
        }
        else if (obj == instance->View.ui.mouseClickCtrlCont.btnRight.btn)
        {
            instance->Model.SendBLEMouseClickValue(2);
        }
        else if (obj == instance->View.ui.mouseClickCtrlCont.btnBack.btn)
        {
            instance->Model.SendBLEMouseClickValue(8);
        }
        else if (obj == instance->View.ui.mouseClickCtrlCont.btnForward.btn)
        {
            instance->Model.SendBLEMouseClickValue(16);
        }

        else if (obj == instance->View.ui.mouseClickCtrlCont.btnPageUp.btn)
        {
            Val[2] = 8;
            instance->Model.SendBLEMouseMoveValue(Val);
        }
        else if (obj == instance->View.ui.mouseClickCtrlCont.btnPageDown.btn)
        {
            Val[2] = -8;
            instance->Model.SendBLEMouseMoveValue(Val);
        }
    }
}