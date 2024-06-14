/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "App/Config/Config.h"
#include "App/Common/DataProc/DataProc.h"
#include "App/Resource/ResourcePool.h"
#include "App/Pages/AppFactory.h"
#include "App/Pages/StatusBar/StatusBar.h"
#include "App/Utils/PageManager/PageManager.h"

using namespace Page;

#define ACCOUNT_SEND_CMD(ACT, CMD)                                         \
    do                                                                     \
    {                                                                      \
        DataProc::ACT##_Info_t info;                                       \
        DATA_PROC_INIT_STRUCT(info);                                       \
        info.cmd = DataProc::CMD;                                          \
        DataProc::Center()->AccountMain.Notify(#ACT, &info, sizeof(info)); \
    } while (0)

extern lv_indev_t *indev_touchpad;

void App_Init()
{
    static AppFactory factory;
    static PageManager manager(&factory);

    /* Make sure the default group exists */
    if (!lv_group_get_default())
    {
        lv_group_t *group = lv_group_create();
        lv_group_set_default(group);
        lv_indev_set_group(indev_touchpad, group);
    }

    /* Initialize the data processing node */
    DataProc_Init();
    ACCOUNT_SEND_CMD(Storage, STORAGE_CMD_LOAD);
    ACCOUNT_SEND_CMD(SysConfig, SYSCONFIG_CMD_LOAD);

    /* Set screen style */
    lv_obj_t *scr = lv_scr_act();
    lv_obj_remove_style_all(scr);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

    // /* Set root default style */
    static lv_style_t rootStyle;
    lv_style_init(&rootStyle);
    lv_style_set_width(&rootStyle, LV_HOR_RES);
    lv_style_set_height(&rootStyle, LV_VER_RES);
    lv_style_set_bg_opa(&rootStyle, LV_OPA_COVER);
    lv_style_set_bg_color(&rootStyle, lv_color_black());
    lv_style_set_border_width(&rootStyle, 0);
    manager.SetRootDefaultStyle(&rootStyle);

    // /* Initialize resource pool */
    ResourcePool::Init();

    // /* Initialize status bar */
    Page::StatusBar_Create(lv_layer_top());

    // /* Initialize pages */
    manager.Install("Popcat", "Pages/Popcat");
    manager.Install("Template", "Pages/_Template");
    manager.Install("Dialplate", "Pages/Dialplate");
    manager.Install("SystemInfos", "Pages/SystemInfos");
    // manager.Install("Startup", "Pages/Startup");
    manager.Install("LaunchOut", "Pages/LaunchOut");
    manager.Install("WaveTable", "Pages/WaveTable");
    manager.Install("Settings", "Pages/Settings");
    manager.Install("SmartAssistant", "Pages/SmartAssistant");
    manager.Install("Compass", "Pages/Compass");
    manager.Install("LiveMap", "Pages/LiveMap");
    manager.Install("BLEKeyboard", "Pages/BLEKeyboard");

    manager.SetGlobalLoadAnimType(PageManager::LOAD_ANIM_OVER_TOP);

    manager.Push("Pages/LaunchOut");
}

void App_Uninit()
{
       ACCOUNT_SEND_CMD(SysConfig, SYSCONFIG_CMD_SAVE);
       ACCOUNT_SEND_CMD(Storage,   STORAGE_CMD_SAVE);
       ACCOUNT_SEND_CMD(Recorder,  RECORDER_CMD_STOP);
}
