/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    portmanager.h
Author:      Wu yu wei
Created:     2012/08/14 10:00
Description: 端口分配器
*********************************************************************/
#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"

#include <memory>
#include "sdk_export.h"

class PortUtil;
class PortManager;
typedef ACE_Singleton<PortManager, ACE_Null_Mutex> PortManager_T;


//定义端口类型
typedef enum
{
    PORT_RTP //RTP端口

}PORT_t;

class SDK_IMPORT_EXPORT PortManager
{
public:
    PortManager();
    virtual ~PortManager();

    unsigned int allocatePort(PORT_t port_type, int call_type);
    void freePort(PORT_t port_type, unsigned int port);
    void updatePortRange(unsigned int start, unsigned int end);
    //重置
    void reset();

private:
    ACE_Recursive_Thread_Mutex lock_;

    PortUtil* util_;
};


#endif // PORTMANAGER_H
