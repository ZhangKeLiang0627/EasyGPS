#include "HAL.h"
#include "ChappieCore/ChappieCore.h"

extern ChappieCore Chappie;

/**
 * @brief  WiFi初始化
 * @param  无
 * @retval 无
 */
void HAL::WiFi_Init()
{
    // ...
}

void HAL::WiFi_Update(void)
{
    // ...
    if (Chappie.Wf.OnAP)
    {
        Chappie.Wf.WiFiloop();
    }
    else if (Chappie.Wf.isConnected())
    {
        // ...todo
        __IntervalExecute(Chappie.Wf.NtpTimeCorr(), 5000);
    }
    else if (Chappie.Wf.wifiEnabled && !Chappie.Wf.isConnected())
    {
        // ...todo
        // __IntervalExecute(Chappie.Wf.enableWiFi(), 500);
        __IntervalExecute(_LOG("[WiFi] is connecting...\n"), 2000);
    }

}
void HAL::WiFi_APSetEnable(bool en)
{
    if (en)
    {
        Chappie.Wf.APMode();
    }
    else
    {
        Chappie.Wf.APClose();
    }
}
void HAL::WiFi_STASetEnable(bool en)
{
    if (en)
    {
        Chappie.Wf.enableWiFi();
    }
    else
    {
        Chappie.Wf.disableWiFi();
    }
}
void HAL::WiFi_GetInfo(HAL::WiFi_Info_t *info)
{
    info->name = Chappie.Wf.WiFiN();
    info->IP = Chappie.Wf.LocalIP();
    info->gateWay = Chappie.Wf.GateWay();
    info->Mac = Chappie.Wf.WiFiMac();
    info->isConnected = Chappie.Wf.isConnected();
    info->isAPEnabled = Chappie.Wf.OnAP;
    info->isWiFiEnabled = Chappie.Wf.isWiFiEnabled();
}