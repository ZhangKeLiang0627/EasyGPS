#include "MiniMaxLlm.h"

/* 百度文心一言 */
String MiniMaxLlm::getResponse(String inputText)
{
    HTTPClient http_client;

    http_client.begin(apiUrl);
    http_client.addHeader("Content-Type", "application/json");
    String tokenKey = String("Bearer ") + apiKey;
    http_client.addHeader("Authorization", tokenKey);

    http_client.setTimeout(20000); // 20s的超时时间

    // String payload = "{\"messages\":[{\"role\":\"user\",\"content\":\"请用一句话回答我，严格规定在二十个字以内。\"},{\"role\":\"assistant\",\"content\":\"好的。\"},{\"role\":\"user\",\"content\":\"" + inputText + "\"}]}";
    String payload = "{\"model\":\"abab5.5s-chat\",\"messages\":[{\"role\": \"system\",\"content\": \"你是生活助手机器人贾维斯，要求下面的回答严格控制在64字符以内。\"},{\"role\":\"user\",\"content\":\"" + inputText + "\"}]}";

    int httpResponseCode = http_client.POST(payload);

    if (httpResponseCode == HTTP_CODE_OK)
    {
        String response = http_client.getString();

        http_client.end();

        // _LOG("\r\n%s\r\n", response.c_str());

        // Parse JSON response
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, response);

        // String outPutText = doc["result"];
        String outPutText = doc["choices"][0]["message"]["content"];

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