/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    notifyhandler.h
Author:      Wu yu wei
Created:     2013/08/19 10:00
Description: 通知处理器接口
*********************************************************************/
#pragma warning(disable: 4251)
#ifndef NOTIFYHANDLER_H
#define NOTIFYHANDLER_H

#include <string>
#include "sdk_export.h"
#include <map>

namespace ProxyServer
{
class SDK_IMPORT_EXPORT NotifyHandler
{
public:
    NotifyHandler();

    //发送通知接口
    virtual int fireNotification(void *para, std::string clientID = "default");

    //发送批通知接口
    virtual int fireBatchNotification(void *para, std::string clientID = "default");
	
    virtual std::string toNotification(void *para, std::string clientID = "default");

protected:
    std::string _cmdid;

};


class SDK_IMPORT_EXPORT NotifyFactory
{
 public:
    void registerHandler(std::string& cmdid, NotifyHandler *);

    virtual NotifyHandler* getHandler(std::string& cmdid);

    static NotifyFactory* instance();

    template<typename T>
        struct AutoReg {
            AutoReg(std::string cmdid) {
            static T* s_ins = new T;
            NotifyFactory::instance()->registerHandler(cmdid, s_ins);
        }
     };

private:
    NotifyFactory();

    static NotifyFactory * instance_;

    std::map<std::string, NotifyHandler *> ntfyMap;

};

}

#endif // NOTIFYHANDLER_H
