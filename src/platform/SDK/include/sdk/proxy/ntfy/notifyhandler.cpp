/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    notifyhandler.cpp
Author:      Wu yu wei
Created:     2013/08/17 10:00
Description: 通知处理器接口
*********************************************************************/
#include "notifyhandler.h"
#include "proxy/frame/proxyserver.h"
#include "proxy/frame/rspproxy.h"
#include "proxy/frame/connectionmanager.h"
#include "sdk/include/appcontext.h"
#include <map>
#include "logger.h"

namespace ProxyServer
{
typedef std::map<std::string, NotifyHandler *>::iterator  NtfyMapIterator;

NotifyHandler::NotifyHandler()
{
}

std::string NotifyHandler::toNotification(void *  /*para*/, std::string    /*clientID*/)
{
    UBP_PROXY_SERVER_ERROR("BASE other than derived NotifyHandler::toNotification is called and return empty");
    return "";
}

int NotifyHandler::fireNotification(void *para, std::string clientID)
{
    /* 向指定Client端发送通知信息 */
    ClientHandlerRef ref = ConnectionManager::instance()->getRemoteClientHandler(clientID);
    ClientHandler * ch = ref.get();
    if(ch == NULL)
    {
        UBP_PROXY_SERVER_ERROR("fire Notification not found, clientID:"<<clientID);
        return -1;
    }

    ch->getResponseProxy()->sendReply(((std::string*)para)->c_str());

    //UBP_PROXY_SERVER_INFO("fire Notification: "<< ((std::string*)para)->c_str()<<" [successfully]");

    return 0;
}

int NotifyHandler::fireBatchNotification(void *para, std::string clientID)
{
    //TODO
    return fireNotification(para, clientID);
}

/* 注册CommandHanlder*/
void NotifyFactory::registerHandler(std::string& cmdid, NotifyHandler * handler)
{
    ntfyMap.insert(std::make_pair(cmdid, handler));
}

NotifyFactory::NotifyFactory()
{

}

NotifyFactory* NotifyFactory::instance()
{
    if(instance_ == NULL)
    {
        instance_ = new NotifyFactory;
    }

    return instance_;
}

/*
  按cmdid来查找NotifyHandler
*/

NotifyHandler* NotifyFactory::getHandler(std::string& cmdid)
{
    NotifyHandler * ret = NULL;
    NtfyMapIterator aci = ntfyMap.find(cmdid);
    if(aci == ntfyMap.end()){
        return NULL;
    }

    else if(aci->second != NULL){
        ret = aci->second;
    }

    return ret;
}

NotifyFactory * NotifyFactory::instance_;
}

