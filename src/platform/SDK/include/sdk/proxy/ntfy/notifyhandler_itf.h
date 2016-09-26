/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    notifyhandler.h
Author:      Wu yu wei
Created:     2013/08/19 10:00
Description: 通知处理器接口
*********************************************************************/
#ifndef NOTIFYHANDLER_ITF_H
#define NOTIFYHANDLER_ITF_H

#include <string>
#include "sdk_export.h"
#include <map>


class SDK_IMPORT_EXPORT NotifyHandlerItf
{
public:
    
    //发送通知接口
        virtual int fireNotification(void *  /*para*/, std::string clientID = "default"){return 0;}
        virtual std::string toNotification(void *  /*para*/, std::string clientID = "default"){return NULL;}

};


class SDK_IMPORT_EXPORT NotifyFactoryItf
{
 public:
    
         virtual NotifyHandlerItf* getHandler(std::string&  /*cmdid*/){return NULL;}

};


#endif // NOTIFYHANDLER_H
