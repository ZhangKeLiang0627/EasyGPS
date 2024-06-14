/**
 * @file EmmaSpeaker.hpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-03-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
// #include "../../EmmaConfig.h"

/* Enable module Speaker */
/* Use speaker class from M5Unified, thanks to @lovyan03 */
/* https://github.com/m5stack/M5Unified/blob/master/src/utility/Speaker_Class.hpp */
/* Only wraped I2S mode so far */
#define EMMA_MODULE_SPEAKER             1
#if EMMA_MODULE_SPEAKER
    #define EMMA_SPEAKER_DATA_PIN       17
    #define EMMA_SPEAKER_BCLK_PIN       16
    #define EMMA_SPEAKER_LRCLK_PIN      15
    #define EMMA_SPEAKER_I2S_PORT       I2S_NUM_1
    /* Pin to shut down the speaker */
    #define EMMA_SPEAKER_ENABLE_PIN     3
    /* More configs in Speaker/EmmaSpeaker.hpp */
#endif

#if EMMA_MODULE_SPEAKER
#include "Speaker/Speaker_Class.hpp"
#include <driver/gpio.h>


class EmmaSpeaker : public m5::Speaker_Class {
    private:
    public:
        EmmaSpeaker()
        {
            /* Setup pins */
            auto cfg = config();
            cfg.pin_data_out = EMMA_SPEAKER_DATA_PIN;
            cfg.pin_bck = EMMA_SPEAKER_BCLK_PIN;
            cfg.pin_ws = EMMA_SPEAKER_LRCLK_PIN;
            cfg.i2s_port = EMMA_SPEAKER_I2S_PORT;
            cfg.task_priority = 10;
            cfg.sample_rate = 16000;
            config(cfg);
            begin();

            enable();
        }
        ~EmmaSpeaker() { end(); }
        
        /* For hardware enable pin if exist */
        inline void enable() { setCtrlPin(1); }
        inline void disable() { setCtrlPin(0); }
        inline void setCtrlPin(uint8_t level)
        {
            gpio_reset_pin((gpio_num_t)EMMA_SPEAKER_ENABLE_PIN);
            gpio_set_direction((gpio_num_t)EMMA_SPEAKER_ENABLE_PIN, GPIO_MODE_OUTPUT_OD);
            gpio_set_level((gpio_num_t)EMMA_SPEAKER_ENABLE_PIN, level);
        }
};

#endif
