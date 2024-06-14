# EASY-GPS

### 本项目基于：X-TRACK & CHAPPIE

### Author: kkl

---

视频介绍：[戳这儿:)]()

硬件开源：[戳这里:P](https://oshwhub.com/hugego/easygps-ji-yu-esp32s3-de-gps-zhui-zong-qi)

![Pic](/4.Pics/frontCover.jpg)
![Pic](/4.Pics/pic1.jpg)
![Pic](/4.Pics/pic2.jpg)

## 源码文件树
```bash
EasyGPS/
├─lib
│  ├─ESP32-BLE-Combo
│  ├─FastLED
│  ├─NimBLE-Arduino
│  ├─QMC5883LCompass
│  └─TinyGPSPlus
├─src
│  ├─App  
│  └─ChappieCore
├── platformio.ini
```

## TF卡文件树
```bash
MicroSDcard/
├─FONT
│  └─font_smileysd_16.bin
├─MAP
├─SystemSave.json
└─.SystemSaveBackup.json
```

## 鸣谢
- 感谢[@FASTSHIFT](https://github.com/FASTSHIFT)的[X-TRACK](https://github.com/FASTSHIFT/X-TRACK)项目：提供了本项目的大致框架。
- 感谢[@Forairaaaaa](https://github.com/Forairaaaaa)的[Chappie-Core](https://github.com/Forairaaaaa/Chappie-Core)项目：提供了ESP32S3在Arduino开发框架的BSP，同时提供硬件的参考设计。
- 感谢[@启凡科创](https://oshwhub.com/dhx233/works)的[QF-HEYE](https://oshwhub.com/dhx233/gai-ban-re-cheng-xiang)项目：提供了部分硬件的参考设计。
- 感谢[@Yeely0162](https://github.com/Yeely0162)开源了ESP32进行WiFi连接时的Web端设计。