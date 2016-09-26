/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    gw_displayManager.h
Author:      Wang Kai
Created:     2014/03/12 10:00

Description: DisplayManager implementation class for SDK_GW,
             convert from DisplayManager interface to ServiceObserver interface
*********************************************************************/

#ifndef GW_DISPLAYMANAGER_H
#define GW_DISPLAYMANAGER_H

#include "sdk/include/displaymanager.h"
#include "sdk/gwapi/service_observer.h"
#include "sdk/include/sdk_export.h"
using namespace LTE::TRUNK::SDK;


class SDK_IMPORT_EXPORT Gw_DisplayManager : public DisplayManager
{
public:
    Gw_DisplayManager();
    //virtual ~Gw_DisplayManager();

    //通知组呼的状态变更信息
    virtual void notifyGroupStatus(GrpDspInfo * grpInfo);
    //通知用户和群组资源的状态变更信息
    virtual void notifyResourceStatus(ResourceStatusIndicator * resourceStatus);
    //通知点对点呼叫的状态变更信息
    virtual void notifyP2pcallStatus(P2pcallStatusIndicator * callStatus);
    //通知视频呼叫的状态变更信息（包括视频回传）
    virtual void notifyP2pvideocallStatus(P2pvideocallStatusIndicator * callStatus);

    virtual void notifyUserStatus(ResourceStatusIndicator * resourceStatus);
    virtual void notifyUserSdsStatus(SdsMessageIndicator * sdsmsg);
    virtual void notifyModuleStatus(DCModuleStatusIndicator * moduleStatus);

    //短数据信息上报给GUI
    virtual void notifySMS(SdsMessageIndicator * sdsmsg);

    //通知配置对象的变更信息， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
    virtual void notifyProvisionChange(ProvChangeIndicator* ){;}

    //通知配置对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
    virtual void notifyProvisionAllResync(ProvChangeIndicator* );

    //通知状态对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
    virtual void notifyStatusAllResync(ResourceStatusIndicator* ) {;}

    virtual void setObserver(AudioServiceObserver* it){audioObserver = it;}
    virtual void setObserver(VideoServiceObserver* it){videoObserver = it;}
    virtual void setObserver(MessageServiceObserver* it){MessageObserver = it;}
    virtual void setObserver(GroupServiceObserver* it){groupObserver = it;}
    virtual void setObserver(QueryServiceObserver* it){queryObserver = it;}

    virtual void setObserver(ModuleStatusObserver* it){moduleObserver = it;}
    virtual void setObserver(ResourceStatusObserver* it){resourceObserver = it;}
private:
    AudioServiceObserver* audioObserver;
    VideoServiceObserver* videoObserver;
    MessageServiceObserver* MessageObserver;
    GroupServiceObserver* groupObserver;
    QueryServiceObserver* queryObserver;

    ModuleStatusObserver* moduleObserver;
    ResourceStatusObserver* resourceObserver;
};

#endif // GW_DISPLAYMANAGER_H
