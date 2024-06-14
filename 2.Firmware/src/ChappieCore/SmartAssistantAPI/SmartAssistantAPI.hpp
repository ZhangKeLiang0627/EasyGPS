/**
 * @file SmartAssistantAPI.hpp
 * @author kkl
 * @brief
 * @version 0.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "BaiduASR/BaiduASR.h"
#include "BaiduTTS/BaiduTTS.h"
#include "BaiduErnieBot/BaiduErnieBot.h"
#include "MiniMaxLlm/MiniMaxLlm.h"
#include "../Mic/EmmaMicPDM.hpp"
#include "../Speaker/EmmaSpeaker.hpp"

class SmartAssistantAPI
{
private:
    TaskHandle_t _task_handler;
    bool _task_inited;

    /**
     * _task_running
     * false -> idie
     * true -> running
     */
    bool _task_running = false;

    /**
     * _task_listening
     * false -> idie
     * true -> listening
     */
    bool _task_listening = false;

    /**
     * @brief Task to handle API Web service
     *
     * @param param
     */
    static void task_handler(void *param)
    {
        SmartAssistantAPI *saa = (SmartAssistantAPI *)param;

        for (;;)
        {
            if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
            {
                // prepare for task
                int timeRecord = 0;

                saa->_inputText = "none";
                saa->_outputText = "none";

                saa->_task_running = true;
                saa->_task_listening = false;

                saa->_speaker->stop();

                // Do your task
                _LOG("Helloworld and %s!\n", saa->_inputText.c_str());

                /* Talk，对话开始 */
                _LOG("[Talk] Talk Start!\n");

                /* Mic，麦克风采集语音 */
                _LOG("[Mic] Record begin!\n");
                saa->_task_listening = true;
                int recordCount = 0;  // 记录for循环轮次
                int voiceBegin = 0;   // 是否开始说话
                int voiceChunks = 0;  // 检测到说话几次
                int silentChunks = 0; // 检测到安静几次
                for (uint16_t i = 0; i < (saa->_samples / 1024); i++)
                {
                    bool micResult = false; // Mic的录制情况

                    micResult = saa->_mic->record((int16_t *)(saa->_rawData + i * 1024), 1024);  // 录制声音
                    _LOG("[Mic] record: %s count: %d\n", (micResult == true ? "true" : "false"), i); // 打印信息
                    while (saa->_mic->isRecording())
                        vTaskDelay(1); // 等待录制完成

                    // 计算均方根，检测说话
                    float rms = saa->_mic->calculateRMS((int16_t *)(saa->_rawData + i * 1024), 256);
                    _LOG("[Mic] rms:%.2f\n", rms);

                    recordCount = i; // 记录录制的轮数

                    if (rms < 1500)
                    {
                        if (voiceBegin == 1)
                        {
                            silentChunks++;
                        }
                    }
                    else
                    {
                        voiceChunks++;
                        if (voiceChunks >= 4) // 如果检测到说话超过几次，则代表说话开始
                        {
                            voiceBegin = 1;
                        }

                        silentChunks = 0;
                    }
                    if (silentChunks >= 12) // 如果检测到安静超过几次，则代表说话结束
                    {
                        silentChunks = 0;
                        recordCount = i; // 记录录制的轮数
                        break;
                    }
                }
                saa->_task_listening = false;
                _LOG("[Mic] Finish! recordCountx1024x2:%s\n", String(recordCount * 1024 * 2).c_str());

                /* SST，语音转文字 */
                _LOG("[ASR] Begin!\n");
                timeRecord = millis();
                saa->_inputText = saa->asr.getRecognitionResult(saa->_rawData, recordCount * 1024 * 2);
                // 防止语音识别失败
                if (strcmp(saa->_inputText.c_str(), "<error>") == 0)
                {
                    saa->_inputText = "none";
                }
                _LOG("[ASR] Record: %s, timeSpend: %d ms\n", saa->_inputText.c_str(), (millis() - timeRecord));

                /* GPT，内容对话 */
                _LOG("[GPT] Begin!\n");
                timeRecord = millis();
                saa->_outputText = saa->bot.getResponse(saa->_inputText); // Baidu
                // saa->_outputText = saa->mml.getResponse(saa->_inputText); // MiniMax

                // 如果输出失败
                if (strcmp(saa->_outputText.c_str(), "null") == 0)
                {
                    saa->_outputText = "您似乎有些小声呢，请对着麦克风说话吧！";
                }
                _LOG("[GPT] Response: %s, timeSpend: %d ms\n", saa->_outputText.c_str(), (millis() - timeRecord));

                /* TTS，文字转语音 */
                // _LOG("[TTS] Begin!\n");
                // int32_t synthLength = saa->_samples * 2;
                // timeRecord = millis();
                // saa->tts.getSynthesis(saa->_outputText, saa->_rawData, &synthLength);
                // _LOG("[TTS] timeSpend: %d ms\n", (millis() - timeRecord));

                // /* Speaker，扬声器播放语音 */
                // _LOG("[Speaker] Begin!\n");
                // saa->_speaker->playRaw((int16_t *)saa->_rawData, synthLength / 2, 8000);

                vTaskDelay(3000);

                saa->_task_running = false;
                _LOG("[Talk] Talk complete!\n");
            }
            delay(5);
        }
        vTaskDelete(NULL);
    }

public:
    SmartAssistantAPI() : _task_inited(false) {}
    ~SmartAssistantAPI() {}

public:
    /**
     * @brief Init Mic and Speaker, create a payLoad buffer and create a task to handle API Web service
     *
     * @param mic
     * @param speaker
     * @param payLoad
     */
    inline void init(EmmaMicPDM *mic, EmmaSpeaker *speaker, char *payLoad)
    {
        _mic = mic;
        _speaker = speaker;
        _payLoad = payLoad;

        _rawData = (int16_t *)ps_malloc(_samples * sizeof(int16_t));

        tts.init(payLoad);
        asr.init(payLoad);
        bot.init();

        /* Create a task to handle API Web service */
        if (!_task_inited)
        {
            _task_inited = true;
            xTaskCreate(task_handler, "SaAPI", 1024 * 5, this, 10, &_task_handler);
        }
    }

    inline void init(EmmaMicPDM *mic, EmmaSpeaker *speaker)
    {
        _mic = mic;
        _speaker = speaker;
        _payLoad = (char *)ps_malloc(100000 * sizeof(char));
        delay(100);
        _rawData = (int16_t *)ps_malloc(_samples * sizeof(int16_t));
        delay(100);

        /* If something wrong */
        if ((_payLoad == NULL) || (_rawData == NULL))
        {
            _LOG("[Saa] malloc buffer from PSRAM error\r\n");
            while (1)
                delay(1000);
        }
        else
        {
            _LOG("[Saa] malloc buffer from PSRAM successful\r\n");
            _LOG("[Saa] free PSRAM: %d\r\n", ESP.getFreePsram());
        }

        asr.init(_payLoad);
        tts.init(_payLoad);
        bot.init();

        /* Create a task to handle API Web service */
        if (!_task_inited)
        {
            _task_inited = true;
            xTaskCreate(task_handler, "SaAPI", 1024 * 5, this, 10, &_task_handler);
        }
    }

    inline void begin(void)
    {
        if (_task_running == false)
        {
            xTaskNotifyGive(_task_handler);
        }
    }

    inline bool isRunning(void) { return _task_running; }

    inline bool isListening(void)
    {
        if (_task_running == false)
            return _task_running;
        else
            return _task_listening;
    }

    inline char *getInputText(void) { return (char *)_inputText.c_str(); }

    inline char *getOutputText(void) { return (char *)_outputText.c_str(); }

private:
    /* buff */
    char *_payLoad = nullptr;
    int16_t *_rawData = nullptr;
    const uint32_t _samples = 131072; // This value MUST ALWAYS be a power of 2 // 这个值必须总是2的幂

    /* hal */
    EmmaMicPDM *_mic = nullptr;
    EmmaSpeaker *_speaker = nullptr;

    /* web api */
    BaiduASR asr;
    BaiduTTS tts;
    BaiduErnieBot bot;
    MiniMaxLlm mml;

private:
    String _inputText = "none";
    String _outputText = "none";
};

/**
 * @brief Define the way to format logout
 *
 */
#define _LOG(format, args...) printf(format, ##args)