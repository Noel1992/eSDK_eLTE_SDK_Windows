/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    mq_dispatch.hpp
  Author:      Wu Gang
  Created:     2014/03/10 13:42
  Description:

*********************************************************************/
#ifndef UBP_PLATFORM_MOUDLES_MQ_DISPATCH_HPP_
#define UBP_PLATFORM_MOUDLES_MQ_DISPATCH_HPP_

#include "ace/Basic_Types.h"
#include <memory>
#include <list>
#include <string>
#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
//TODO::to be removed
#include "platform/modules/thread/ubp_threadpool.hpp"
#include "platform/modules/mq/mq_poll_task.hpp"
#include "platform/modules/mq/mq_context_itf.hpp"


namespace ubp
{
namespace platform
{
namespace mq
{
class IMessageSocket;
class MsgDealWorker;
class SocketPollCtl;

typedef MsgDealWorker *WorkerPtr;
typedef std::map<std::string, WorkerPtr>  WorkerMap;
typedef std::map<std::string, WorkerPtr>::iterator WorkerIter;
typedef std::map<std::string, WorkerPtr>::const_iterator WorkerConstIter;
typedef std::pair<std::string, WorkerPtr> WorkerMapPair;
typedef std::pair<WorkerMap::const_iterator, bool> WorkerMapPairRet;
typedef WorkerMap::value_type WorkerValueType;

class MQ_IMPORT_EXPORT BaseDispatchMsg
{
  public:
    BaseDispatchMsg()
    {
    }
    virtual ~BaseDispatchMsg()
    {
    }
};

class MQ_IMPORT_EXPORT MsgDispatcher
{
  public:
    MsgDispatcher(IMessageContext *pContext, const std::string &endpoint);
    virtual ~MsgDispatcher();

    ACE_INT32 Init();

    ACE_INT32 Fini();

    ACE_INT32 Start();

    ACE_INT32 AddWorker(const std::string &workerId, MsgDealWorker *pWorker);

    ACE_INT32 PushMsg(const std::string &workerId, BaseDispatchMsg *pMsg);

    //轮询Socket获取消息
    //timeout-轮询阻塞时间，单位微秒;0:不阻塞直接返回;-1:阻塞直到有消息返回
    BaseDispatchMsg *OnMessage(long timeout = 0);

  private:
    void RecvPair(IMessageSocket *);

  private:
    WorkerMap taskMap_;
    std::string endpoint_;
    std::string pairEndpoint_;
    ACE_Recursive_Thread_Mutex rwPushSocketMutex_;  //rwPushSocketMutex_读写锁
    ACE_Recursive_Thread_Mutex rwTaskMapMutex_;  //rwPushSocketMutex_读写锁
    bool init_;
    IMessageContext *pContext_;
    void *zmqContext_;   //ZMQ context
    std::auto_ptr<IMessageSocket> pPushSocket_;
    std::auto_ptr<IMessageSocket> clientPairSocket_;
    std::auto_ptr<IMessageSocket> serverPairSocket_;
    bool isStop_;
};

//////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT MsgDealWorker: public MQPollTask
{
    friend MsgDispatcher;
  public:
    MsgDealWorker();
    virtual ~MsgDealWorker();

    /********************************************************************
     Method:      OnMessage
     FullName:    MsgDealWorker::OnMessage
     Access:      public
     Returns:     void
     Description: 接收MsgDispatcher派发的消息
     Parameter:   void *pMsg 使用后需要release
    ********************************************************************/
    virtual ACE_INT32 OnMessage(BaseDispatchMsg *pMsg) = 0;

    /********************************************************************
     Method:      SendMsg2Dispatcher
     FullName:    MsgDealWorker::SendMsg2Dispatcher
     Access:      public
     Returns:     ACE_INT32：发送消息的长度，如果返回UBP_FAIL发送失败
     Description: 向MsgDispatcher发送消息
     Parameter:   void *pMsg：MsgDispatcher接收到需要release
    ********************************************************************/
    ACE_INT32 SendMsg2Dispatcher(BaseDispatchMsg *pMsg);

	void SetHwm(ACE_INT32 hwm);
    ACE_INT32 GetHwm() const;

	std::string GetWorkerID() const;
    ACE_INT32 Init(void);
    ACE_INT32 Fini(void);
    ACE_INT32 Run(void);
  protected:
    IMessageSocket *GetSocket() const;
    void SetWorkerID(const std::string &workId);
    void SetEndPoint(const std::string &endpoint);
  private:
    IMessageSocket *pDealSocket_;
    bool run_;
    std::string workerId_;
    ACE_Recursive_Thread_Mutex rwSendSocketMutex_;  //rwSendSocketMutex_读写锁
    std::string endpoint_;
	ACE_INT32 hwm_;
};

}//mq
}//platform
}//ubp
#endif //UBP_PLATFORM_MOUDLES_MQ_DISPATCH_HPP_
