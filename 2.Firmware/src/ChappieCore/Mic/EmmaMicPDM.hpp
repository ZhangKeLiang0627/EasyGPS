/**
 * @file EmmaMicPDM.hpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-02-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
// #include "../../EmmaConfig.h"

/* Enable module PDM Mic */
/* Use speaker class from M5Unified, thanks to @lovyan03 */
/* https://github.com/m5stack/M5Unified/blob/master/src/utility/Mic_Class.hpp */
#define EMMA_MODULE_MIC_PDM             1
#if EMMA_MODULE_MIC_PDM
    #define EMMA_MICPDM_CLK_PIN         6
    #define EMMA_MICPDM_DATA_PIN        7
    #define EMMA_MICPDM_I2S_PORT        I2S_NUM_0
    /* More configs in Mic/EmmaMicPDM.hpp */
#endif

#if EMMA_MODULE_MIC_PDM
#include "Mic/Mic_Class.hpp"


class EmmaMicPDM : public m5::Mic_Class {
    private:
    public:
        EmmaMicPDM()
        {
            auto cfg = config();
            cfg.pin_data_in = EMMA_MICPDM_DATA_PIN;
            cfg.pin_ws = EMMA_MICPDM_CLK_PIN;
            cfg.i2s_port = EMMA_MICPDM_I2S_PORT;
            cfg.magnification = 1;
            cfg.task_priority = 15;
            cfg.sample_rate = 16000; // 48000
            config(cfg);
            begin();
        }
        ~EmmaMicPDM() { end(); }

        float calculateRMS(int16_t *buffer, int samples)
        {
            float sum = 0;

            for (int i = 0; i < samples; i++)
            {
                int16_t diff = buffer[i];
                sum += diff * diff;
            }

            return sqrt(sum / (float)samples);
        }
        
        void sleep(void)
        {
            gpio_reset_pin((gpio_num_t)EMMA_MICPDM_CLK_PIN);
            gpio_reset_pin((gpio_num_t)EMMA_MICPDM_DATA_PIN);
        }
};

#endif
