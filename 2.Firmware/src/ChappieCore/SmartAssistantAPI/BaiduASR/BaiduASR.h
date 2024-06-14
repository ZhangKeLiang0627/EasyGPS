/**
 * @file BaiduSST.h
 * @author kkl
 * @brief
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h> // 用于参数数据的
#include "base64.h"

class BaiduASR
{

public:
    BaiduASR() {}
    BaiduASR(char *payLoad) : _payLoad(payLoad) {}
    ~BaiduASR() { _payLoad = nullptr; }

    void init(char *payLoad)
    {
        // load param 加载变量
        _payLoad = payLoad;
        getToken();
    }

    String getRecognitionResult(int16_t *rawdata, uint32_t len);

    void getToken(void);

    void gainToken(void);

private:
    char *_payLoad = nullptr;
    String _token = "";
    String apiASRUrl = "https://vop.baidu.com/pro_api";
    // http://vop.baidu.com/server_api # 标准版
    // https://vop.baidu.com/pro_api # 极速版
};

/**
 * @brief Define the way to format logout
 *
 */
#define _LOG(format, args...) printf(format, ##args)
