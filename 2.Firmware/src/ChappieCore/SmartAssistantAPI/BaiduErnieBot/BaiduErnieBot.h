/**
 * @file BaiduErnieBot.h
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

class BaiduErnieBot
{

public:
    BaiduErnieBot() {}
    ~BaiduErnieBot() {}

    void init(void)
    {
        // load param 加载变量
        getToken();
    }
    
    String getResponse(String inputText);

    void getToken(void);

    void gainToken(void);

private:
    // String _token = "24.a5491e2c24a7530134953afb40aa0a23.2592000.1718098342.282335-57964024";
    String _token = "";
    String apiErnieBotUrl = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=";
    // https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions // ERNIE-3.5-8K
    // https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions_pro // ERNIE-4.0-8K
};

/**
 * @brief Define the way to format logout
 *
 */
#define _LOG(format, args...) printf(format, ##args)
