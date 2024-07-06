#include "BaiduErnieBot.h"

/* 百度文心一言 */
String BaiduErnieBot::getResponse(String inputText)
{
    HTTPClient http_client;

    String URL = apiErnieBotUrl + _token;

    http_client.begin(URL);
    http_client.addHeader("Content-Type", "application/json");

    http_client.setTimeout(20000); // 20s的超时时间

    // String payload = "{\"messages\":[{\"role\":\"user\",\"content\":\"请用一句话回答我，严格规定在二十个字以内。\"},{\"role\":\"assistant\",\"content\":\"好的。\"},{\"role\":\"user\",\"content\":\"" + inputText + "\"}]}";
    String payload = "{\"messages\":[{\"role\":\"user\",\"content\":\"" + inputText + "\"}],\"system\":\"你是生活助手机器人，要求回答严格控制在64字内！\"}";

    int httpResponseCode = http_client.POST(payload);

    if (httpResponseCode == HTTP_CODE_OK)
    {
        String response = http_client.getString();
        http_client.end();

        // _LOG("\r\n%s\r\n", response.c_str());

        // Parse JSON response
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, response);

        // authentication 鉴权
        String responseErr = doc["error_code"];
        printf("[ASR] Err: %s\n", responseErr);
        if (strcmp(responseErr.c_str(), "110") == 0 || strcmp(responseErr.c_str(), "111") == 0)
        {
            gainToken();
            return "token失效了，已尝试重新获取，请试试看吧！";
        }

        String outPutText = doc["result"];

        _LOG("\r\n%s\r\n", outPutText.c_str());

        return outPutText;
    }
    else
    {
        _LOG("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpResponseCode).c_str());
        http_client.end();

        return "<error>";
    }
}

/* 获取Token */
void BaiduErnieBot::gainToken(void)
{
    // param
    HTTPClient http_client;
    Preferences prefs;
    String response;

    // 注意，要把下面网址中的your_apikey和your_secretkey替换成自己的API Key和Secret Key
    // http_client.begin("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=your_apikey&client_secret=your_secretkey");
    http_client.begin("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=Psp49YQ4FFrzrqbiHK7xxxxx&client_secret=BPx8GurbpL640iq4O9xj1soySTzxxxxx");
    http_client.addHeader("Content-Type", "application/json");

    // GET
    int httpResponseCode;
    httpResponseCode = http_client.GET();

    if (httpResponseCode == HTTP_CODE_OK)
    {
        response = http_client.getString();
        http_client.end();

        // Parse JSON response
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, response);

        // access_token
        String token = doc["access_token"];
        _LOG("[GPT] Token: %s\n", token.c_str());

        // Storage
        prefs.begin("SaAPI");
        prefs.putString("BaiduBot", token);
        _LOG("[Storage] BaiduBotToken: %s\n", prefs.getString("BaiduBot").c_str());
        prefs.end();

        // refresh local token 更新本地token
        _token = token;
    }
    else
    {
        _LOG("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpResponseCode).c_str());
        http_client.end();
    }
}

/* 从flash中获取token到本地 */
void BaiduErnieBot::getToken(void)
{
    // param
    Preferences prefs;
    prefs.begin("SaAPI");
    _token = prefs.getString("BaiduBot");

    _LOG("[Storage] getBaiduBotToken: %s\n", _token.c_str());
    prefs.end();
}