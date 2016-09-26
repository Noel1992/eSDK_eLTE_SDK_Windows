/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    peripheralstatus_indicator.h
Author:      zhangjiansheng
Created:     2013/02/22 16:36
Description: 本对象负责将外围设备检测的信息上报给GUI
*********************************************************************/

#ifndef PERIPHERALSTATUS_INDICATOR_H
#define PERIPHERALSTATUS_INDICATOR_H

#include "sdk_export.h"
#include "ace/Basic_Types.h"
#include <string>

typedef enum {
    PTT_MIC = 1,   //台咪
    SND_CARD = 2,    //声卡
    SERIAL_PORT =3   //串口
} Peripheral_t;

class SDK_IMPORT_EXPORT Peripheralstatus_indicator
{
public:
    Peripheralstatus_indicator(Peripheral_t Pdevice);
    virtual ~Peripheralstatus_indicator();
    std::string datalog_str();

    Peripheral_t getDeviceTp();

    void setDevStatus(bool DevStatus); //PTT PRESS:TRUE, PTT RELEASE:FALSE;

    bool getDevStatus();

private:
    bool m_DevStatus; //PTT_MIC:"PTT PRESS:TRUE, PTT RELEASE:FALSE";SERIAL_PORT\SND_CARD:"ONLINE:TRUE, OFFLINE:FALSE"
    Peripheral_t _device; //device tp
};


#endif // PERIPHERALSTATUS_INDICATOR_H
