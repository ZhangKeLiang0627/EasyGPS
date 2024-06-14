#ifndef _CHAPPIECORE_CONFIG_H_
#define _CHAPPIECORE_CONFIG_H_

/* Config */
#define FIRMWARE_NAME "[EasyGPS]"
#define VERSION_SOFTWARE "v1.2"
#define VERSION_HARDWARE "v1.4"
#define VERSION_AUTHOR_NAME "kkl"

#define CHAPPIE_CORE_USE_LVGL_WITHOUT_RTOS 0 // 宏：是否使用RTOS执行LVGL
#define CHAPPIE_CORE_LVGL_USE_PSRAM 1 // 宏：是否使用片外RAM作为LVGL的缓存Buffer
#define CHAPPIE_CORE_USE_BLE 1 // 宏：是否使用蓝牙

// #define CHAPPIE_CORE_LVGL_DIS_PORTRAIT  0

#endif