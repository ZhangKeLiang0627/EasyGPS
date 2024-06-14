/**
 * @file BaiduTTS.h
 * @author kkl
 * @brief
 * @version 0.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h> // 用于参数数据的

class BaiduTTS
{

public:
    BaiduTTS() {}
    BaiduTTS(char *payLoad) : _payLoad(payLoad) {}
    ~BaiduTTS() { _payLoad = nullptr; }

    void init(char *payLoad)
    {
        // load param 加载变量
        _payLoad = payLoad;
        getToken();
    }

    bool getSynthesis(String inputText, int16_t *rawData, int32_t *len);

    void getToken(void);

    void gainToken(void);

private:
    char *_payLoad = nullptr;
    String _token = "";
    String apiTTSUrl = "http://tsn.baidu.com/text2audio";
};

/**
 * @brief Define the way to format logout
 *
 */
#define _LOG(format, args...) printf(format, ##args)
