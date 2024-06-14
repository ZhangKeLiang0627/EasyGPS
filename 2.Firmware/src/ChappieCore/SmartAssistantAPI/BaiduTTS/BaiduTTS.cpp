#include "BaiduTTS.h"

/* 百度在线语音合成 */
bool BaiduTTS::getSynthesis(String inputText, int16_t *rawData, int32_t *len)
{
    HTTPClient http_client;

    _LOG("[TTS] Start Synthesis!\r\n");

    memset(_payLoad, '\0', strlen(_payLoad)); // 将数组清空
    strcat(_payLoad, "tex=");                 // 合成的文本，UTF-8编码格式
    strcat(_payLoad, inputText.c_str());      // 合成的文本，UTF-8编码格式
    strcat(_payLoad, "&lan=zh");              // 语言选择，目前只有中英文混合模式，填写固定值zh
    strcat(_payLoad, "&cuid=hugokkl");        // 识别码，随便几个字符，但最好唯一
    strcat(_payLoad, "&ctp=1");               // 客户端类型选择，web端填写固定值1
    strcat(_payLoad, "&spd=7");               // 语速，取值0-15，默认为5中语速
    strcat(_payLoad, "&pit=6");               // 音调，取值0-15，默认为5中语调
    strcat(_payLoad, "&vol=12");              // 音量，基础音库取值0-9，精品音库取值0-15，默认为5中音量
    strcat(_payLoad, "&per=5118");            // 基础音库，度丫丫=4
    strcat(_payLoad, "&aue=5");               // 3为mp3格式(默认)；4为pcm-16k；5为pcm-8k；6为wav（内容同pcm-16k）
    strcat(_payLoad, "&tok=");                // access_token
    strcat(_payLoad, _token.c_str());         // access_token

    http_client.begin(apiTTSUrl);

    http_client.setTimeout(10000); // 10s超时时间

    int httpResponseCode = http_client.POST(_payLoad);

    if (httpResponseCode == HTTP_CODE_OK)
    {
        WiFiClient response;
        int32_t streamLength = http_client.getSize();

        _LOG("streamSize:%d\r\n", streamLength);

        response = http_client.getStream();

        while (!response.available()) // 等待数据流可获取
        {
            delay(1);
        }

        streamLength = min(streamLength, *len);

        *len = streamLength < 0 ? 0 : streamLength;

        printf("streamLength:%d\r\n", *len);

        response.readBytes((char *)rawData, streamLength);

        _LOG("[TTS] Synthesis done!\r\n");

        return true;
    }
    else
    {
        *len = 0;

        _LOG("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpResponseCode).c_str());
    }

    http_client.end();

    _LOG("[TTS] Synthesis fail!\r\n");

    return false;
}

/* 获取Token */
void BaiduTTS::gainToken(void)
{
    // param
    HTTPClient http_client;
    Preferences prefs;
    String response;

    // 注意，要把下面网址中的your_apikey和your_secretkey替换成自己的API Key和Secret Key
    // http_client.begin("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=your_apikey&client_secret=your_secretkey");
    http_client.begin("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=souCWerKxgs7khvJE3Bxxxxx&client_secret=92b50AWi00ZEZPyAQTZnMwwEPc4xxxxx");

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
        _LOG("[TTS] Token: %s\n", token.c_str());

        // Storage
        prefs.begin("SaAPI");
        prefs.putString("BaiduVoice", token);
        _LOG("[Storage] BaiduVoiceToken: %s\n", prefs.getString("BaiduVoice").c_str());
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
void BaiduTTS::getToken(void)
{
    // param
    Preferences prefs;
    prefs.begin("SaAPI");
    _token = prefs.getString("BaiduVoice");

    _LOG("[Storage] getBaiduVoiceToken: %s\n", _token.c_str());
    prefs.end();
}

