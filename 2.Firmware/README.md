#### 编译该源码需要注意 lib/lv_conf.h

- 需要将 `lv_conf.h` 放在lvgl库文件夹的同一目录：  .pio/libdeps/esp32-s3-devkitc-1

#### 请手动在lib文件夹中拉取需要的库文件

- 进入lib文件夹`cd EasyGPS/2.Firmware/lib/`

- 使用`git`工具拉取相关的库

```bash
git clone https://github.com/juanmcasillas/ESP32-BLE-Combo

git clone https://github.com/FastLED/FastLED

git clone https://github.com/h2zero/NimBLE-Arduino

git clone https://github.com/mprograms/QMC5883LCompass

git clone https://github.com/mikalhart/TinyGPSPlus
``` 