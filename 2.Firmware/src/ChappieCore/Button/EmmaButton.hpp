/**
 * @file EmmaButton.hpp
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

/* Enable module Button */
#define EMMA_MODULE_BUTTON              1
#if EMMA_MODULE_BUTTON
    /* Set pin -1 to disable */
    #define EMMA_BTN_A_PIN              -1
    #define EMMA_BTN_B_PIN              0
    #define EMMA_BTN_C_PIN              -1
    /* Add more button in "Button/EmmaButton.hpp" */
#endif

#include "Button/Button.h"
#if EMMA_MODULE_BUTTON

/* Add your button with this */
#define Button_Add(name, pin) __Button name = __Button(pin)

class EmmaButton {
    private:
    public:
    
        #if EMMA_BTN_A_PIN >= 0
        Button_Add(A, EMMA_BTN_A_PIN);
        #endif

        #if EMMA_BTN_B_PIN >= 0
        Button_Add(B, EMMA_BTN_B_PIN);
        #endif

        #if EMMA_BTN_C_PIN >= 0
        Button_Add(C, EMMA_BTN_C_PIN);
        #endif

};

#endif
