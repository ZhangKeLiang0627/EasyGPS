/**
 * @file ChappieBLE.hpp
 * @author kkl
 * @brief Reference: https://github.com
 * @version 0.1
 * @date 2024-04-18
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <Arduino.h>
#include <BleCombo.h>
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <BleGamepad.h>

static String _name = "HugoklblDevice";

class ChappieBLE
{
private:
    bool _inited = false;
    bool _isEnabled = false;

public:
    ChappieBLE() {}
    ~ChappieBLE() {}

    inline void init(void)
    {
        if (!_inited)
        {
            _inited = true;
            _isEnabled = false;
            bleDevice.setName(_name.c_str());
            bleDevice.setDelay(10);

            printf("[BLE] Ble Device init successed!\n");
        }
        printf("[BLE] Ble Device init repeatedly!\n");
    }

    inline void deinit(void)
    {
        // 如果未初始化
        if (!_inited)
            return;

        _inited = false;
        _isEnabled = false;
    }

    /**
     * @brief Let BLE bebin
     *
     * @param none
     * @return none
     */
    inline void begin(void)
    {
        // 如果未初始化
        if (!_inited)
            return;

        _isEnabled = true;

        Keyboard.begin();
        Mouse.begin();
        Gamepad.begin();
    }

    /**
     * @brief Set BLE to sleep
     *
     * @param none
     * @return none
     */
    inline void sleep(void)
    {
        // 如果未初始化
        if (!_inited)
            return;

        _isEnabled = false;

        Keyboard.end();
        Mouse.end();
        Gamepad.end();
    }

    /**
     * @brief Check BLE isConnected
     *
     * @param none
     * @return none
     */
    inline bool isConnected(void)
    {
        if (!_inited)
            return false;

        return bleDevice.isConnected();
    }

    /**
     * @brief Check BLE isEnabled
     *
     * @param none
     * @return none
     */
    inline bool isEnabled(void)
    {
        if (!_inited)
            return false;

        return _isEnabled;
    }

    /**
     * @brief Write BLEKeyboard to device
     *
     * @param none
     * @return none
     */
    inline size_t write(const uint8_t *c)
    {
        if (!_inited && !bleDevice.isConnected())
            return false;

        if (c[1] == 0x00)
        {
            printf("[BLE] MediaCtrl\n");
            return Keyboard.write(c);
        }
        else if (c[1] == 0xff)
        {
            printf("[BLE] KeyCtrl\n");
            return Keyboard.write(c[0]);
        }
        else
        {
            printf("[BLE] ComboCtrl\n");
            Keyboard.press(c[0]);
            Keyboard.press(c[1]);
            delay(5);
            Keyboard.releaseAll();
            return true;
        }
    }

    /**
     * @brief move BLEMouse to device
     *
     * @param none
     * @return none
     */
    inline void move(const uint8_t *c)
    {
        Mouse.move(c[0], c[1], c[2], c[3]);
    }

    /**
     * @brief click BLEMouse to device
     *
     * @param none
     * @return none
     */
    inline void click(const uint16_t b)
    {
        Mouse.click(b);
    }

    /**
     * @brief Return BLE Device's name
     *
     * @param none
     * @return char *
     */
    inline char *GetName(void)
    {
        return (char *)_name.c_str();
    }

private:
};
