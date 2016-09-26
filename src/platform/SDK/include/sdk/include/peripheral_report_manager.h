/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    peripheral_report_manager.h
Author:      Zhangjiansheng
Created:     2013/02/23 10:00
Description: 外围设备接口状态上报，子类继承实现

*********************************************************************/

#ifndef PERIPHERAL_REPORT_MANAGER_H
#define PERIPHERAL_REPORT_MANAGER_H

#include "sdk_export.h"

class Peripheralstatus_indicator;

class SDK_IMPORT_EXPORT Peripheral_report_manager
{
public:
    //virtual ~Peripheral_report_manager(){};

    //外围设备数据上报
    virtual void notifyPeripheralReport(Peripheralstatus_indicator *) = 0;

private:
};


#endif // REPORTMANAGER_H
