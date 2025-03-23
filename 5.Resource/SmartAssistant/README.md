## 关于智能语音助手SmartAssistant的使用方法

![ssa流程](/4.Pics/ssa.png)

### 1. 获得密钥

- 百度在线语音识别 & 百度在线语音合成（共用一套密钥）

> 请您[戳这里:)](https://zhangkeliang0627.github.io/2024/03/24/ESP32S3%E6%8E%A5%E5%85%A5%E7%99%BE%E5%BA%A6%E5%9C%A8%E7%BA%BF%E8%AF%AD%E9%9F%B3%E8%AF%86%E5%88%AB/README/)了解密钥的详细获取方法！

- 文心一言API

> 请您[戳这里:)](https://zhangkeliang0627.github.io/2024/03/25/ESP32S3%E6%8E%A5%E5%85%A5%E6%96%87%E5%BF%83%E4%B8%80%E8%A8%80/README/)了解密钥的详细获取方法！

### 2. 填入密钥

请分别前往源码:
- `src/ChappieCore/SmartAssistantAPI/BaiduASR`
- `src/ChappieCore/SmartAssistantAPI/BaiduTTS`
- `src/ChappieCore/SmartAssistantAPI/BaiduErnieBot`

找到对应的`gainToken`函数，替换您自己的密钥。

### 3. 上机调试
基本上按步骤到这里就可以成功啦，如果没有成功使用智能语音助手，请考虑是否连上WiFi、密钥是否输入正确、账户是否有领取免费次数等原因。祝您复刻成功！