/**
 * @file MiniMaxLlm.h
 * @author kkl
 * @brief
 * @version 0.1
 * @date 2024-05-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class MiniMaxLlm
{

public:
    MiniMaxLlm() {}
    ~MiniMaxLlm() {}

    String getResponse(String inputText);

    String getToken(void);

private:
    const String apiKey = "xxxxxxxxxxxxxxxxxxxI1NiIsInR5cCI6IkpXVCJ9.eyJHcm91cE5hbWUiOiJDaGVuc2hhbiIsIlVzZXJOYW1lIjoiQ2hlbnNoYW4iLCJBY2NvdW50IjoiIiwiU3ViamVjdElEIjoiMTc4MjU4ODUwOTcwNjUyMjkxMSIsIlBob25lIjoiMTMyNDYxNjYyMTAiLCJHcm91cElEIjoiMTc4MjU4ODUwOTY5ODEzNDMwMyIsIlBhZ2VOYW1lIjoiIiwiTWFpbCI6IiIsIkNyZWF0ZVRpbWUiOiIyMDI0LTA1LTA2IDIyOjM0OjM4IiwiaXNzIjoibWluaW1heCJ9.D_rzFIdaHRBvQ68XCgYKNWs5DkupWPW8RApkDwjWE9A43StEi04Y_UxdFzYABVVce3EJNxL0mSc3nzDN1-0U_QmkJvjGUFioA81tcoGnDn-EWitv7h70xBmQraqc2-zc8rXW8nKYNA5hLMMA5j35whNLqwEBnMgbCrWUNLfmd1TfY80m1uLCErwzlvhh8Hd2EkXLq0SQLWCdbBlHy2Gw9EYle_osy_kvPJhl7mmRmQej1IfQrZUaP5y7bXznGG3gSluT85NOzSCRHOhFSIUQMTUJOInaoFVH37-Tgd2-mkM4MKF9koseg3SmgVxLmTmsZQK4U57BLUeZo1Qbfla3tw";
    const String apiUrl = "https://api.minimax.chat/v1/text/chatcompletion_v2";
};

/**
 * @brief Define the way to format logout
 *
 */
#define _LOG(format, args...) printf(format, ##args)
