/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    mq_context_itf.hpp
  Author:      Wu Gang
  Created:     2013/01/09 13:42
  Description: MQ消息接口

*********************************************************************/
#ifndef UBP_PLATFORM_MOUDLES_MQ_CONTEXT_ITF_HPP_
#define UBP_PLATFORM_MOUDLES_MQ_CONTEXT_ITF_HPP_

#include "mq_export.hpp"

#include <list>
#include <string>
#include "google/protobuf/message.h"
#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"

#include "platform/platform_msg.pb.h"
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "platform/modules/mq/message_factory.hpp"
#include "platform/modules/mq/mq_error.hpp"

//////////////////////////////////////////////////////////////////////////
//MQ message type define
//////////////////////////////////////////////////////////////////////////
//TODO::定义成枚举，to move to internal
static const ACE_UINT32 UBP_MQ_SYNC_TYPE = 1;
static const ACE_UINT32 UBP_MQ_ASYN_RPN_TYPE = 2;
static const ACE_UINT32 UBP_MQ_ASYN_NORPN_TYPE = 3;
static const ACE_UINT32 UBP_MQ_NOTIFY_TYPE = 4;

//缺省发送消息等待结果的缺省超时时间,单位：秒。（注：此超时时间可在下发消息时设置）
static const ACE_UINT32 DEFAULT_SEND_MQ_TIMEOUT = 5;
//发送消息次数（如果失败重发）
static const ACE_UINT32 DEFAULT_SEND_TRY_TIMES = 1;
//在DealerMsg或者ReponseMsg中回调处理消息超时时间,单位：秒（缺省60秒）
static const ACE_UINT32 DEFAULT_REPONSE_HANDLE_MQ_TIMEOUT = 60;
//异步消息在队列中的等待时间,单位：秒（缺省120秒），超过则从队列删除不能发送异步响应
static const ACE_UINT32 DEFAULT_ASYN_RPN_WAIT_TIMEOUT = 120;

//处理消息回调的任务个数
static const ACE_UINT32 DEFAULT_MSGCALLBACK_WORKER_COUNT = 1;

//request socket最大闲置时间（秒）,超过则会被关闭
static const ACE_UINT32 SOCKET_MAX_IDLE_TIME = 300;

typedef ::google::protobuf::Message IMessage;

namespace boost
{
template <typename R>  class unique_future;
template <typename R>  class promise;
};

using namespace google::protobuf;
using namespace ubp::platform;

namespace ubp
{
namespace platform
{
namespace mq
{

class IMessageReponseHandler;
class IMessageDealerHandler;
class IMessageConsumerHandler;
class IMessageSocket;
class IMessageAppFrame;
class ZmqLoginInfo;
class ZmqParams;

typedef boost::unique_future<IMessageAppFrame *> IMessageFuture;
typedef boost::promise <IMessageAppFrame *> IMessagePromise;

//////////////////////////////////////////////////////////////////////////////
//AsynMsgCallbackItf
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT AsynMsgCallbackItf
{
  public :
    /********************************************************************
    Method:      OnRecvResponse
    FullName:    AsynMsgCallbackItf::OnRecvResponse
    Access:      public
    Returns:     UBP_SUCCESS:处理成功；其他:处理失败
    Description: 接收异步消息的结果回调函数
    Parameter:   IMessageHeader* header- 消息头
    Parameter:   IMessage* msgbody- 消息体
    ********************************************************************/
    virtual void OnRecvResponse(const IMessageHeader *header,
                                const IMessage *msgbody) = 0;

    virtual void OnTimeout(const IMessageHeader *header,
                           const IMessage *msgbody) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//IMessageContext
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageContext
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    virtual ACE_INT32 Init() = 0;

    virtual ACE_INT32 Fini() = 0;

    IMessageContext *CreateInstance();

    //设置本地的svckey,只在做客户端设置，服务端由MQ默认设置
    virtual ACE_INT32 SetLocalSvcKey(const std::string &name) = 0;

    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     响应消息IMessage*，失败返回NULL。IMessage*需要调用者释放
     Description: 发送同步消息(带缺省的消息头)
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &message - 待发送消息
     Parameter:   ACE_UINT32 timeout - 消息超时时间（秒）
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessage &message,
                                      ACE_UINT32 timeout,
                                      ACE_UINT32 retry = DEFAULT_SEND_TRY_TIMES) = 0;

    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     响应消息IMessage*，失败返回NULL。IMessage*需要调用者释放
     Description: 发送同步消息(带缺省的消息头)
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &message - 待发送消息
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessage &message) = 0;

    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     响应消息IMessage*，失败返回NULL。IMessage*需要调用者释放
     Description: 发送同步消息
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &message - 待发送消息的消息头
     Parameter:   const REQ &message - 待发送消息
     Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessageHeader &header,
                                      const IMessage &message,
                                      ACE_UINT32 timeout,
                                      ACE_UINT32 retry = DEFAULT_SEND_TRY_TIMES) = 0;
    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     响应消息IMessage*，失败返回NULL。IMessage*需要调用者释放
     Description: 发送同步消息
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &message - 待发送消息的消息头
     Parameter:   const REQ &message - 待发送消息
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessageHeader &header,
                                      const IMessage &message) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息,带缺省的消息头
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &msgbody - 待发送消息
      Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息,带缺省的消息头
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &msgbody - 待发送消息
      Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息,带缺省的消息头
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &msgbody - 待发送消息
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息,带缺省的消息头
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &msgbody - 待发送消息
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:失败；>0:成功
     Description: 发送异步消息
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &header - 待发送消息的消息头
     Parameter:   const REQ &msgbody - 待发送消息
     Parameter:   AsynMsgCallbackItf - 处理回应消息的回调函数
     Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:失败；>0:成功
     Description: 发送异步消息
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &header - 待发送消息的消息头
     Parameter:   const REQ &msgbody - 待发送消息
     Parameter:   AsynMsgCallbackItf - 处理回应消息的回调函数
     Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:失败；>0:成功
     Description: 发送异步消息
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &header - 待发送消息的消息头
     Parameter:   const REQ &msgbody - 待发送消息
     Parameter:   AsynMsgCallbackItf - 处理回应消息的回调函数
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:失败；>0:成功
     Description: 发送异步消息
     Parameter:   const std::string &destSvcKey - 目标服务key
     Parameter:   const REQ &header - 待发送消息的消息头
     Parameter:   const REQ &msgbody - 待发送消息
     Parameter:   AsynMsgCallbackItf - 处理回应消息的回调函数
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息，等待返回结果
      Parameter:   IMessagePromise &promise - 用于接收异步响应消息的对象
                   注意promise中获取的IMessageAppFrame* 需要释放
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &header - 待发送消息的消息头
      Parameter:   const REQ &msgbody - 待发送消息
      Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
      ///////////////////////////////////////////////////////////////////
      Example:
      {
          IMessagePromise promise;
          pMqContext_->SendMessage(promise, toService_, *pHeader, *pmsg, 5);

          //等到结果前做其他事情
          //......

          //获取异步消息的返回结果
          std::auto_ptr<IMessageAppFrame> pResult(promise.get_future().get());
          if(NULL == pResult.get())
          {
              UBP_MQ_INFO("!!!!!Get future result timeout!!!!");
          }
          else
          {
              UBP_MQ_INFO("Get future result success!");
              UBP_MQ_DEBUG(pResult->GetMsgHeader()->ShortDebugString());
              UBP_MQ_DEBUG(pResult->GetMsgbody()->ShortDebugString());
          }
      }
     ********************************************************************/
    virtual ACE_INT32 SendMessage(IMessagePromise &promise,
                                  const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &message,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息，等待返回结果
      Parameter:   IMessagePromise &promise - 用于接收异步响应消息的对象
                   注意promise中获取的IMessageAppFrame* 需要释放
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &header - 待发送消息的消息头
      Parameter:   const REQ &msgbody - 待发送消息
      ///////////////////////////////////////////////////////////////////
      Example:
      {
          IMessagePromise promise;
          pMqContext_->SendMessage(promise, toService_, *pHeader, *pmsg);

          //等到结果前做其他事情
          //......

          //获取异步消息的返回结果
          std::auto_ptr<IMessageAppFrame> pResult(promise.get_future().get());
          if(NULL == pResult.get())
          {
              UBP_MQ_INFO("!!!!!Get future result timeout!!!!");
          }
          else
          {
              UBP_MQ_INFO("Get future result success!");
              UBP_MQ_DEBUG(pResult->GetMsgHeader()->ShortDebugString());
              UBP_MQ_DEBUG(pResult->GetMsgbody()->ShortDebugString());
          }
      }
     ********************************************************************/
    virtual ACE_INT32 SendMessage(IMessagePromise &promise,
                                  const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &message) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息，等待返回结果
      Parameter:   IMessagePromise &promise - 用于接收异步响应消息的对象
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &header - 待发送消息的消息头
      Parameter:   const REQ &msgbody - 待发送消息
      Parameter:   ACE_UINT32 timeout -  消息超时时间（秒）
    ********************************************************************/
    virtual ACE_INT32 SendMessage(IMessagePromise &promise,
                                  const std::string &destSvcKey,
                                  const IMessage &message,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 发送异步消息，等待返回结果
      Parameter:   IMessagePromise &promise - 用于接收异步响应消息的对象
      Parameter:   const std::string &destSvcKey - 目标服务key
      Parameter:   const REQ &header - 待发送消息的消息头
      Parameter:   const REQ &msgbody - 待发送消息
    ********************************************************************/
    virtual ACE_INT32 SendMessage(IMessagePromise &promise,
                                  const std::string &destSvcKey,
                                  const IMessage &message) = 0;

    /********************************************************************
      Method:      SendAsynResponse
      FullName:    IMessageContext::SendAsynResponse
      Access:      public
      Returns:     UBP_FAIL:失败；>0:成功
      Description: 根据异步请求的消息序列号，回应异步响应消息
      Parameter:   ACE_UINT64 sequence - 异步消息请求的消息序列号
      Parameter:   const REQ &msgbody - 待发送的结果消息
    ********************************************************************/
    virtual ACE_INT32 SendAsynResponse(ACE_UINT64 sequence,
                                       const IMessage &msgbody) = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:注册成功；其他：失败
     Description: 注册消息处理类,特别注意：同一个服务进程内的要处理的msgId不能重复
     Parameter:   IMessageReponseHandler *handler - 消息处理类的指针,调用者释放
     Parameter:   msgPackageName  - 需要响应消息的package名
    ********************************************************************/
    virtual ACE_INT32 RegMsgReponser(IMessageReponseHandler *handler,
                                     const std::string &msgPackageName) = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:注册成功；其他：失败
     Description: 注册消息处理类,特别注意：同一个服务进程内的要处理的msgId不能重复
     Parameter:   IMessageReponseHandler *handler - 消息处理类的指针,调用者释放
     Parameter:   char** msgIds  - 需要响应消息的消息ID list，例如：
                  char *msgIds[] =
                  {"ubp.platform.t1",
                   "ubp.platform.t2",
                   NULL
                  };
    ********************************************************************/
    virtual ACE_INT32 RegMsgReponser(IMessageReponseHandler *handler,
                                     const char **msgIds) = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:注册成功；其他：失败
     Description: 注册消息处理类,特别注意：同一个服务进程内的要处理的msgId不能重复
     Parameter:   IMessageReponseHandler *handler - 消息处理类的指针,调用者释放
     Parameter:   std::list<string>:msgIds  - 需要响应消息的消息ID list
    ********************************************************************/
    virtual ACE_INT32 RegMsgReponser(IMessageReponseHandler *handler,
                                     std::list<std::string> msgIds) = 0;

    /********************************************************************
    Method:      RegMsgDealer
    FullName:    IMessageContext::RegMsgDealer
    Access:      public
    Returns:     UBP_SUCCESS:注册成功；其他：失败
    Description: 注册消息处理类,特别注意：同一个服务进程内的要处理的msgId不能重复
    Parameter:   IMessageDealerHandler *handler - 异步消息处理类的指针,调用者释放
    Parameter:   msgPackageName  - 需要响应消息的package名,以此区分不同的reponse
    ********************************************************************/
    virtual ACE_INT32 RegMsgDealer(IMessageDealerHandler *handler,
                                   const std::string &msgPackageName) = 0;

    /********************************************************************
     Method:      RegMsgDealer
     FullName:    IMessageContext::RegMsgDealer
     Access:      public
     Returns:     UBP_SUCCESS:注册成功；其他：失败
     Description: 注册消息处理类,特别注意：同一个服务进程内的要处理的msgId不能重复
     Parameter:   IMessageDealerHandler *handler - 消息处理类的指针,调用者释放
     Parameter:   char** msgIds  - 需要响应消息的消息ID list，例如：
                  char *msgIds[] =
                  {"ubp.platform.t1",
                   "ubp.platform.t2",
                    NULL
                  };
    ********************************************************************/
    virtual ACE_INT32 RegMsgDealer(IMessageDealerHandler *handler,
                                   char **msgIds) = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:注册成功；其他：失败
     Description: 注册消息处理类,特别注意：同一个服务进程内的要处理的msgId不能重复
     Parameter:   IMessageReponseHandler *handler - 消息处理类的指针,调用者释放
     Parameter:   std::list<string>:msgIds  - 需要响应消息的消息ID list
    ********************************************************************/
    virtual ACE_INT32 RegMsgDealer(IMessageDealerHandler *handler,
                                   std::list<std::string> msgIds) = 0;

    /********************************************************************
     Method:      Notify
     FullName:    IMessageContext::Notify
     Access:      public
     Returns:     UBP_SUCCESS-成功，其他：失败
     Description: 发送通知消息（带缺省的消息头）
     Parameter:   const REQ &msgBody - 待通知的消息
     Parameter:   const std::string &topic - 消息的topic
    ********************************************************************/
    virtual ACE_INT32 Notify(const IMessage &msgBody,
                             const std::string &topic,
                             const std::string &filter = "") = 0;

    /********************************************************************
     Method:      Notify
     FullName:    IMessageContext::Notify
     Access:      public
     Returns:     UBP_SUCCESS-成功，其他：失败
     Description: 发送通知消息（自带消息头）
     Parameter:   const REQ &header - 通知消息头
     Parameter:   const REQ &msgBody - 通知消息体
     Parameter:   const std::string &topic - 消息的topic
    ********************************************************************/
    virtual ACE_INT32 Notify(const IMessageHeader &header,
                             const IMessage &msgBody,
                             const std::string &topic,
                             const std::string &filter = "") = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:注册成功；其他：失败
     Description: 注册消息处理类
     Parameter:   const std::string &destSvcKey - 目标服务destSvcKey
     Parameter:   IMessageConsumerHandler *handler - 消息处理类的指针(调用者管理)
    ********************************************************************/
    virtual ACE_INT32 RegMsgConsumer(const std::string &destSvcKey,
                                     const std::string &topic,
                                     IMessageConsumerHandler *cb,
                                     const std::string &filter = "") = 0;

    /********************************************************************
     Method:      UnRegMsgConsumer
     FullName:    IMessageContext::UnRegMsgConsumer
     Access:      public
     Returns:     UBP_SUCCESS:注销成功；其他：失败
     Description: 注销消息处理类
     Parameter:   const std::string &destSvcKey - 目标服务destSvcKey
     Parameter:   IMessageConsumerHandler *handler - 消息处理类的指针(调用者管理)
    ********************************************************************/
    virtual ACE_INT32 UnRegMsgConsumer(const std::string &destSvcKey,
                                       const std::string &topic,
                                       IMessageConsumerHandler *cb) = 0;

    //////////////////////////////////////////////////////////////////////////
    //本机的传输层加密模式：true/false
    virtual bool GetTSLMode() = 0;

    //本机的传输层加密模式：true/false
    virtual void SetTSLMode(bool isTSL) =  0;

    //远端服务IP的传输层加密模式
    virtual ZmqLoginInfo *GetTSLMode(const std::string &remoteIp) =  0;

    //远端服务IP的传输层加密模式
    virtual void SetTSLMode(const std::string &remoteIp, const ZmqLoginInfo &lgnInfo) =  0;

    //登陆接口：LoginResult*需要释放
    virtual LoginResult *Login(const std::string &ip, const ReqLogin &login) = 0;

    //for NAT 登陆接口：LoginResult*需要释放
    virtual LoginResult* Login(const std::string& ip,const ReqLogin& login, 
                               int dealport,int pubport) = 0;
	

    //ZMQ鉴权用户名
    virtual std::string GetLoginName() = 0;

    //ZMQ鉴权密码
    virtual std::string GetLoginPwd() = 0;

    virtual char *GetClientPublic() = 0;
    virtual char *GetClientSecret() = 0;
    virtual char *GetServerPublic() = 0;
    virtual char *GetServerSecret() = 0;
    virtual const std::string& GetDomain() const= 0;
    virtual void *GetZMQContext() = 0;
    virtual ZmqParams *GetZmqParams() = 0;
    virtual void SetClientSrcIp(const std::string &srcIp) = 0;
    virtual std::string GetClientSrcIp() = 0;
};

//////////////////////////////////////////////////////////////////////////////
//ZmqLoginInfo
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT ZmqLoginInfo
{
  public:
    ZmqLoginInfo(bool tlsmode, const std::string &loginname, 
        const std::string &loginpwd,const std::string domain);

    virtual ~ZmqLoginInfo();

    void SetDomainByType(ACE_INT32 type);

  public:
    bool tlsMode_;
    std::string loginName_;
    std::string loginPwd_;
    std::string domain_;
};

//////////////////////////////////////////////////////////////////////////////
//ZmqParams
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT ZmqParams
{
  public:

    virtual ~ZmqParams();

  public:
    //重连时间，单位：毫秒
    ACE_UINT32 RECONNECT_IVL;

    //#最多socket数量
    ACE_UINT32 MAX_SOCKETS;

    //#发布者和消费者心跳的时间，单位：秒
    ACE_UINT32 PUB_SUB_HEARTBEAT_INTERVAL;

    //#消费者心跳超时次数
    ACE_UINT64 SUBSCRIBE_TIMEOUT_TIMES;

    //#消费者心跳超时时间，单位：秒
    ACE_UINT64 SUBSCRIBE_TIMEOUT_TIME;

    //#服务端消息在队列中的等待时间,单位：秒
    ACE_UINT64 ASYN_RPN_WAIT_TIMEOUT;

    //#服务端在回调处理消息超时时间,单位：秒
    ACE_UINT32 REPONSE_HANDLE_MQ_TIMEOUT;

    //#处理消息回调的任务个数
    ACE_UINT32 MSGCALLBACK_WORKER_COUNT;

    //#socket最大闲置时间（秒）,超过则会被关闭
    ACE_UINT64 MAX_IDLE_TIME;

	//#login超时时间
    ACE_UINT32 LOGIN_TIMEOUT;
    
	//#是否支持2.1ZMQ
    bool OLD_SUPPORT;

	//#客户方发送消息默认超时时间
	ACE_UINT32 DEFAULT_REQUEST_TIMEOUT;

	//#客户方默认发送次数
	ACE_UINT32 DEFAULT_RETRY_TIMES;

    //max timeout times
	ACE_UINT32 MAX_CLOSE_SOCKET_TIMES;
};



//////////////////////////////////////////////////////////////////////////////
//IMessageFilter
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageFilter
{
  public :
    /********************************************************************
    Method:      OnFilter
    FullName:    IMessageFilter::OnFilter
    Access:      public
    Returns:     true:符合条件；false:不符合条件
    Description: 符合过滤条件的消息才会被处理
    Parameter:   IMessage* msg- 消息
    ********************************************************************/
    virtual bool OnFilter(IMessage *msg) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//BaseMsgHandlerItf
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT BaseMsgHandlerItf
{
  public:
    BaseMsgHandlerItf();
    virtual ~BaseMsgHandlerItf();

    /********************************************************************
    Method:      OnFilter
    FullName:    BaseMsgHandlerItf::OnFilter
    Access:      public
    Returns:     true:符合条件；false:不符合条件
    Description: 符合过滤条件的消息才会被处理
    ********************************************************************/
    bool OnFilter(IMessageHeader *header, IMessage *body);

    ACE_INLINE void SetMsgHeaderFilter(IMessageFilter *filter)
    {
      this->pHeaderFilter_ = filter ;
    }

    ACE_INLINE void SetMsgBodyFilter(IMessageFilter *filter)
    {
      this->pBodyFilter_ = filter ;
    }

    ACE_INLINE IMessageFilter *GetMsgHeaderFilter()
    {
      return this->pHeaderFilter_;
    }

    ACE_INLINE IMessageFilter *GetMsgBodyFilter()
    {
      return this->pBodyFilter_;
    }

    ACE_INLINE void SetCheckServiceRunning(bool isCheck)
    {
        checkServiceRunning_ = isCheck;
    }

    ACE_INLINE bool GetCheckServiceRunning() const
    {
        return checkServiceRunning_;
    }
  private:
    IMessageFilter *pHeaderFilter_;
    IMessageFilter *pBodyFilter_;
    bool checkServiceRunning_; //check service is running or not before handle msg
};

//////////////////////////////////////////////////////////////////////////////
//IMessageIdFilter
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageIdFilter : public IMessageFilter
{
  public:
    IMessageIdFilter(const std::string &msgid);

    virtual bool OnFilter(IMessage *header);

  private:
    std::string msgId_;
};


//////////////////////////////////////////////////////////////////////////////
//IMessageReponseHandler
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageReponseHandler : public BaseMsgHandlerItf
{
  public:

    /********************************************************************
       Method:      OnReponseMessage
       FullName:    IMessageReponseHandler::OnReponseMessage
       Access:      public
       Returns:     响应消息RPN*，失败返回NULL。MQ负责销毁
       Description: reponse消息接收处理函数
       Parameter:   IMessageHeader* header- 消息头，MQ负责销毁
       Parameter:   IMessage* msgbody - 消息体，，MQ负责销毁
      ********************************************************************/
    virtual IMessage *OnReponseMessage(IMessageHeader *header,
                                       IMessage *msgbody) = 0;

};

//////////////////////////////////////////////////////////////////////////////
//IMessageDealerHandler
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageDealerHandler: public BaseMsgHandlerItf
{
  public:
    /********************************************************************
       Method:      OnDealMessage
       FullName:    IMessageDealerHandler::OnDealMessage
       Access:      public
       Returns:     ACE_INT32 0:成功,其他：失败
       Description: 异步消息接收处理函数
       Parameter:   IMessageHeader* header- 消息头，MQ负责销毁
       Parameter:   IMessage* msgbody - 消息体，，MQ负责销毁
      ********************************************************************/
    virtual ACE_INT32 OnDealMessage(IMessageHeader *header,
                                    IMessage *msgbody) = 0;

    virtual ~IMessageDealerHandler()
    {
    }
};

//////////////////////////////////////////////////////////////////////////////
//IMessageConsumerHandler
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageConsumerHandler : public BaseMsgHandlerItf
{
  public:
    IMessageConsumerHandler()
      : identity_(""), reconnectInterval_(1000)
    {
    }

    /********************************************************************
       Method:      OnMessage
       FullName:    IMessageConsumerHandler::OnMessage
       Access:      public
       Returns:     响应消息RPN*，失败返回NULL。MQ负责销毁
       Description: 消息接收处理函数
       Parameter:   IMessageHeader* header- 消息头，MQ负责销毁
       Parameter:   IMessage* msgbody - 消息体，，MQ负责销毁
      ********************************************************************/
    virtual ACE_INT32 OnMessage(IMessageHeader *header, IMessage *msgbody) = 0;

    void SetIndentity(const std::string &indentity)
    {
      identity_ = indentity;
    }

    std::string GetIndentity() const
    {
      return identity_;
    }

    //socket重连间隔时间，单位ms
    void SetReconnectInterval(int inverval)
    {
      reconnectInterval_ = inverval;
    }

    int GetReconnectInterval() const
    {
      return reconnectInterval_;
    }

  private:
    std::string identity_;
    int reconnectInterval_;
};




//////////////////////////////////////////////////////////////////////////////
//IMessageQueue
//////////////////////////////////////////////////////////////////////////////
class MQ_IMPORT_EXPORT IMessageAppFrame
{
  public:
    IMessageAppFrame();
    IMessageAppFrame(const IMessageHeader *header, const IMessage *body);
    virtual ~IMessageAppFrame();

    IMessageHeader *GetMsgHeader();
    IMessage *GetMsgbody();
    void ReleaseHeader();
    void ReleaseBody();
    void SetMsgHeader(IMessageHeader *);
    void SetMsgbody(IMessage *);
  private:
    IMessageHeader *pMsgHeader_;
    IMessage *pMsgBody_;
    bool isReleaseHeader_;
    bool isReleaseBody_;
};

class MQ_IMPORT_EXPORT IMessageQueue
{
  public:
    IMessageQueue(IMessageContext *mqContext, const std::string &endpoint);
    virtual ~IMessageQueue();
    //轮询Socket获取消息
    //timeout-轮询阻塞时间，单位微秒;0:不阻塞直接返回;-1:阻塞直到有消息返回
    IMessageAppFrame *OnMessage(long timeout = 0);

  private:
    IMessageSocket *pSocketPtr_;
};

class MQ_IMPORT_EXPORT IMessageSender
{
  public:
    IMessageSender(IMessageContext *mqContext, const std::string &endpoint);
    virtual ~IMessageSender();
    void SendMessage(IMessageAppFrame &msg);
  private:
    IMessageSocket *pSocketPtr_;
    ACE_Recursive_Thread_Mutex rwSocketmutex_;
};


}//mq
}//platform
}//ubp
#endif // UBP_PLATFORM_MOUDLES_MQ_CONTEXT_ITF_HPP_
