/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    observerproxy.h
Author:      Wu yu wei
Created:     2013/08/20 16:00
Description: 事件监听触发代理，用于收集业务Indicator，触发NotifyHandler回调
*********************************************************************/
#ifndef OBSERVERPROXY_H
#define OBSERVERPROXY_H

//#include "mq/callstatusdispatcher.h"
//#include "mq/resconfdispatcher.h"
//#include "mq/guievtnotifier.h"
//#include "mq/resstatusdispatcher.h"

#include "platform/modules/timer/ubp_timer.hpp"
#include "sdk/include/gisreportmanager.h"
#include "sdk/include/displaymanager.h"
#include <list>
#include "util/sync.h"

#include "ace/OS_main.h"
#include "ace/Task_T.h"
#include "ace/Synch.h"

#include "boost/shared_ptr.hpp"


//封装智能指针
typedef GrpDspInfo GroupStatusIndicator ;
typedef boost::shared_ptr<P2pcallStatusIndicator> P2pcallStatusIndicatorRef;
typedef boost::shared_ptr<P2pvideocallStatusIndicator> P2pvideocallStatusIndicatorRef;
typedef boost::shared_ptr<GrpDspInfo> GroupStatusIndicatorRef;
typedef boost::shared_ptr<SdsMessageIndicator> SdsMessageIndicatorRef;
typedef boost::shared_ptr<GISReportIndicator> GISReportIndicatorRef;
typedef boost::shared_ptr<ProvChangeIndicator> ProvChangeIndicatorRef;
typedef boost::shared_ptr<DCModuleStatusIndicator> DCModuleStatusIndicatorRef;
typedef boost::shared_ptr<ResourceStatusIndicator> ResourceStatusIndicatorRef;

namespace ProxyServer
{
class NotifyHandler;
class DispatchWorker_P2PAudio;
class DispatchWorker_Group;
class DispatchWorker_MO;
class DispatchWorker_GIS;
class DispatchWorker_Resource;

class ObserverProxy;

class SDK_IMPORT_EXPORT ObserverProxy //:// public DisplayManager
{
     friend class DispatchWorker_P2PAudio;
     friend class DispatchWorker_Group;
     friend class DispatchWorker_MO;
     friend class DispatchWorker_GIS;
     friend class DispatchWorker_Resource;

public:
    explicit ObserverProxy();
    /* 装载Slots */
    void preprareSlots();

public : /* 监听Slots定义 */
    /* 接收GIS Report */
    void recvGISRpt(GISReportIndicatorRef);
    /* 更新订阅结果 */
    void updateSubRsp(GISReportIndicatorRef);
    /* 配置资源更新通知 */
    void moChanged(ProvChangeIndicatorRef);
    /* 配置准备完毕通知，根据这个通知来查询GIS 订阅列表，更新GUI缓存 */
    void provResynch(ProvChangeIndicatorRef);
    /* 语音业务状态通知 */
    void updateP2PStatus(P2pcallStatusIndicatorRef);
    /* 当前主讲人信息通知 */
    void updateGroupStatus(GroupStatusIndicatorRef);
    /* 临时群组关闭通知 */
    void notifyGroupClose(ResourceStatusIndicatorRef);
    /* 视频浏览SDP协商资源通知 */
    void notifyVideoInfo(P2pvideocallStatusIndicatorRef);
    /* 模块状态更新通知 */
    void moduleStatusUpdate(DCModuleStatusIndicatorRef);
    /* 调度台关闭通知  */
    void onShutDown();
    /* 通知地图上快速定位终端 */
    void notifyQuickLocate(int);
    /* 过滤Mo */
    bool filterMO(ProvChangeIndicator * ind);  
    /* 过滤Resource State */
    bool filterResource(ResourceStatusIndicator * ind);
    /* 过滤tempgroup */
    bool filterGroup(GroupStatusIndicator * ind);

	//virtual void notifyGroupStatus(GrpDspInfo*){}
	//virtual void notifyResourceStatus(ResourceStatusIndicator*){}
	//virtual void notifyP2pcallStatus(P2pcallStatusIndicator*){}
	//virtual void notifyP2pvideocallStatus(P2pvideocallStatusIndicator*){}
	//virtual void notifyUserStatus(ResourceStatusIndicator*){}
	//virtual void notifyUserSdsStatus(SdsMessageIndicator*){}
	//virtual void notifyModuleStatus(DCModuleStatusIndicator*){}
	//virtual void notifySMS(SdsMessageIndicator*){}
	//virtual void notifyProvisionChange(ProvChangeIndicator*){}
	//virtual void notifyProvisionAllResync(ProvChangeIndicator*){}
	//virtual void notifyStatusAllResync(ResourceStatusIndicator*){}
	


private:
    /* Schedual 一个operation, 异步执行Handler */
    ACE_INT32 schedualOperation(NotifyHandler *, void *);

    /* 退出开关 */
    bool b_shutdown;
    DispatchWorker_P2PAudio * dsp_audio;
    DispatchWorker_Group * dsp_group;
    DispatchWorker_MO * dsp_mo;
    DispatchWorker_GIS * dsp_gis;
    DispatchWorker_Resource * dsp_res;
};

class SDK_IMPORT_EXPORT ObserverProxy_T
{
public:
	static ObserverProxy *instance();

private:
	static ObserverProxy* m_instance;
	static utils::RW_Thread_Mutex _mutex;

};


class OperationHandler :public ubp::platform::timer::UbpTimerHandler
{
public:
    OperationHandler(NotifyHandler *handler, void * para);

    virtual ~OperationHandler(void);

    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

private:
    NotifyHandler * _handler;
    //保存执行参数， 便于释放
    void * _para;
};

/* Gis Task, 向Gis Client 推送GIS消息 */
class DispatchWorker_GIS : public ACE_Task<ACE_MT_SYNCH>
{
public:
    DispatchWorker_GIS(ObserverProxy* obs);    
    void pushGisReport(GISReportIndicatorRef);
    virtual int svc(void);

private:
    ObserverProxy * _ref;
    /* Gis mutex */
    utils::Mutex _gismutex;
    /* Gis发送队列 */
    std::list<GISReportIndicatorRef>* gisrptList;
};

/* P2PAudio Status Task, 向Gis Client 推送点呼消息 */
class DispatchWorker_P2PAudio: public ACE_Task<ACE_MT_SYNCH>
{
public:
    DispatchWorker_P2PAudio(ObserverProxy* obs);
    virtual int svc(void);
    void pushAudioStatus(P2pcallStatusIndicatorRef);

private:
    ObserverProxy * _ref;
    /* 语音点呼状态 mutex */
    utils::Mutex _p2paudiomutex;
    /* 语音点呼发送队列 */
    std::list<P2pcallStatusIndicatorRef>* p2paudioStatusList;
};


/* Group Status Task, 向Gis Client 推送组呼消息 */
class DispatchWorker_Group: public ACE_Task<ACE_MT_SYNCH>
{
public:
    DispatchWorker_Group(ObserverProxy* obs);
    virtual int svc(void);
    void pushGroupStatus(GroupStatusIndicatorRef);

private:
    ObserverProxy * _ref;
    /* 组呼状态 mutex */
    utils::Mutex _groupmutex;
    std::list<GroupStatusIndicatorRef>* groupStatusList;
};

/* Mo 状态变更通知Task, 向Gis Client 推送Mo Change消息 */
class DispatchWorker_MO: public ACE_Task<ACE_MT_SYNCH>
{
public:
    DispatchWorker_MO(ObserverProxy* obs);
    virtual int svc(void);
    void pushMOStatus(ProvChangeIndicatorRef);

private:
    ObserverProxy * _ref;
    /* Mo mutex */
    utils::Mutex _momutex;
    std::list<ProvChangeIndicatorRef>* moIndList;
};

/* Resource 状态变更通知Task, 向Gis Client 推送Resource Change消息 */
class DispatchWorker_Resource: public ACE_Task<ACE_MT_SYNCH>
{
public:
    DispatchWorker_Resource(ObserverProxy* obs);
    virtual int svc(void);
    void pushResourceStatus(ResourceStatusIndicatorRef);

private:
    ObserverProxy * _ref;
    /* 组呼状态 mutex */
    utils::Mutex _resmutex;
    std::list<ResourceStatusIndicatorRef>* grpcloseList;
};

}

#endif // OBSERVERPROXY_H
