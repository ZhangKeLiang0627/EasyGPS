/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "AppFactory.h"
#include "_Template/Template.h"
#include "Dialplate/Dialplate.h"
#include "SystemInfos/SystemInfos.h"
#include "StartUp/StartUp.h"
#include "Popcat/Popcat.h"
#include "LaunchOut/LaunchOut.h"
#include "WaveTable/WaveTable.h"
#include "Settings/Settings.h"
#include "SmartAssistant/SmartAssistant.h"
#include "Compass/Compass.h"
#include "LiveMap/LiveMap.h"
#include "BLEKeyboard/BLEKeyboard.h"

#define APP_CLASS_MATCH(className)         \
    do                                     \
    {                                      \
        if (strcmp(name, #className) == 0) \
        {                                  \
            return new Page::className;    \
        }                                  \
    } while (0)

PageBase *AppFactory::CreatePage(const char *name)
{
    APP_CLASS_MATCH(Template);
    APP_CLASS_MATCH(Dialplate);
    APP_CLASS_MATCH(SystemInfos);
    // APP_CLASS_MATCH(Startup);
    APP_CLASS_MATCH(Popcat);
    APP_CLASS_MATCH(LaunchOut);
    APP_CLASS_MATCH(WaveTable);
    APP_CLASS_MATCH(Settings);
    APP_CLASS_MATCH(SmartAssistant);
    APP_CLASS_MATCH(Compass);
    APP_CLASS_MATCH(LiveMap);
    APP_CLASS_MATCH(BLEKeyboard);

    return nullptr;
}
