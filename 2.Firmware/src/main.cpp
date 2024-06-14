#include <Arduino.h>
#include "lvgl.h"
#include "ChappieCore/ChappieCore.h"
#include "App/App.h"
#include "App/Common/HAL/HAL.h"

ChappieCore Chappie;

void setup()
{
  /* Init Chappie Core */
  Chappie.begin();

  /* Connect HAL */
  HAL::HAL_Init();

  /* UI Create */
  App_Init();

  /* Memory Check */
  _LOG("[PSRAM] free PSRAM: %d\r\n", ESP.getFreePsram());
  // Serial.printf("Deafult free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  // Serial.printf("PSRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

#if !CHAPPIE_CORE_USE_LVGL_WITHOUT_RTOS
  Chappie.lvgl.enable();
#endif
}

void loop()
{
#if CHAPPIE_CORE_USE_LVGL_WITHOUT_RTOS
  lv_timer_handler();
  delay(1);
#endif
  // HAL::HAL_Update();
  delay(10);
}
