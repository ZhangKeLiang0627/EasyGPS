#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

bool HAL::SmartAssistant_Init()
{
    // ...
    return 0;
}

/**
 * @brief  启动智能助手对话
 * @param  en
 * @retval 无
 */
void HAL::SmartAssistant_SetBegin(bool en)
{
    if (!en)
        return;

    Chappie.saa.begin();

}

/**
 * @brief  获取智能助手的信息
 * @param  无
 * @retval 无
 */
void HAL::SmartAssistant_GetInfo(HAL::Saa_Info_t *info)
{
    info->inputText = Chappie.saa.getInputText();
    info->outputText = Chappie.saa.getOutputText();
    info->isRunning = Chappie.saa.isRunning();
    info->isListening = Chappie.saa.isListening();
}