/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    mq_context_itf.hpp
  Author:      Wu Gang
  Created:     2013/01/09 13:42
  Description: MQ��Ϣ�ӿ�

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
//TODO::�����ö�٣�to move to internal
static const ACE_UINT32 UBP_MQ_SYNC_TYPE = 1;
static const ACE_UINT32 UBP_MQ_ASYN_RPN_TYPE = 2;
static const ACE_UINT32 UBP_MQ_ASYN_NORPN_TYPE = 3;
static const ACE_UINT32 UBP_MQ_NOTIFY_TYPE = 4;

//ȱʡ������Ϣ�ȴ������ȱʡ��ʱʱ��,��λ���롣��ע���˳�ʱʱ������·���Ϣʱ���ã�
static const ACE_UINT32 DEFAULT_SEND_MQ_TIMEOUT = 5;
//������Ϣ���������ʧ���ط���
static const ACE_UINT32 DEFAULT_SEND_TRY_TIMES = 1;
//��DealerMsg����ReponseMsg�лص�������Ϣ��ʱʱ��,��λ���루ȱʡ60�룩
static const ACE_UINT32 DEFAULT_REPONSE_HANDLE_MQ_TIMEOUT = 60;
//�첽��Ϣ�ڶ����еĵȴ�ʱ��,��λ���루ȱʡ120�룩��������Ӷ���ɾ�����ܷ����첽��Ӧ
static const ACE_UINT32 DEFAULT_ASYN_RPN_WAIT_TIMEOUT = 120;

//������Ϣ�ص����������
static const ACE_UINT32 DEFAULT_MSGCALLBACK_WORKER_COUNT = 1;

//request socket�������ʱ�䣨�룩,������ᱻ�ر�
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
    Returns:     UBP_SUCCESS:����ɹ�������:����ʧ��
    Description: �����첽��Ϣ�Ľ���ص�����
    Parameter:   IMessageHeader* header- ��Ϣͷ
    Parameter:   IMessage* msgbody- ��Ϣ��
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

    //���ñ��ص�svckey,ֻ�����ͻ������ã��������MQĬ������
    virtual ACE_INT32 SetLocalSvcKey(const std::string &name) = 0;

    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     ��Ӧ��ϢIMessage*��ʧ�ܷ���NULL��IMessage*��Ҫ�������ͷ�
     Description: ����ͬ����Ϣ(��ȱʡ����Ϣͷ)
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &message - ��������Ϣ
     Parameter:   ACE_UINT32 timeout - ��Ϣ��ʱʱ�䣨�룩
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessage &message,
                                      ACE_UINT32 timeout,
                                      ACE_UINT32 retry = DEFAULT_SEND_TRY_TIMES) = 0;

    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     ��Ӧ��ϢIMessage*��ʧ�ܷ���NULL��IMessage*��Ҫ�������ͷ�
     Description: ����ͬ����Ϣ(��ȱʡ����Ϣͷ)
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &message - ��������Ϣ
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessage &message) = 0;

    /********************************************************************
     Method:      SendSyncMessage
     FullName:    IMessageContext::SendSyncMessage
     Access:      public
     Returns:     ��Ӧ��ϢIMessage*��ʧ�ܷ���NULL��IMessage*��Ҫ�������ͷ�
     Description: ����ͬ����Ϣ
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &message - ��������Ϣ����Ϣͷ
     Parameter:   const REQ &message - ��������Ϣ
     Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
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
     Returns:     ��Ӧ��ϢIMessage*��ʧ�ܷ���NULL��IMessage*��Ҫ�������ͷ�
     Description: ����ͬ����Ϣ
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &message - ��������Ϣ����Ϣͷ
     Parameter:   const REQ &message - ��������Ϣ
    ********************************************************************/
    virtual IMessage *SendSyncMessage(const std::string &destSvcKey,
                                      const IMessageHeader &header,
                                      const IMessage &message) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ,��ȱʡ����Ϣͷ
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &msgbody - ��������Ϣ
      Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ,��ȱʡ����Ϣͷ
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &msgbody - ��������Ϣ
      Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ,��ȱʡ����Ϣͷ
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &msgbody - ��������Ϣ
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ,��ȱʡ����Ϣͷ
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &msgbody - ��������Ϣ
     ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessage &msgbody,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
     Description: �����첽��Ϣ
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
     Parameter:   const REQ &msgbody - ��������Ϣ
     Parameter:   AsynMsgCallbackItf - �����Ӧ��Ϣ�Ļص�����
     Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
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
     Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
     Description: �����첽��Ϣ
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
     Parameter:   const REQ &msgbody - ��������Ϣ
     Parameter:   AsynMsgCallbackItf - �����Ӧ��Ϣ�Ļص�����
     Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
     Description: �����첽��Ϣ
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
     Parameter:   const REQ &msgbody - ��������Ϣ
     Parameter:   AsynMsgCallbackItf - �����Ӧ��Ϣ�Ļص�����
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody,
                                  AsynMsgCallbackItf *cb) = 0;

    /********************************************************************
     Method:      SendMessage
     FullName:    IMessageContext::SendMessage
     Access:      public
     Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
     Description: �����첽��Ϣ
     Parameter:   const std::string &destSvcKey - Ŀ�����key
     Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
     Parameter:   const REQ &msgbody - ��������Ϣ
     Parameter:   AsynMsgCallbackItf - �����Ӧ��Ϣ�Ļص�����
    ********************************************************************/
    virtual ACE_INT32 SendMessage(const std::string &destSvcKey,
                                  const IMessageHeader &header,
                                  const IMessage &msgbody,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ���ȴ����ؽ��
      Parameter:   IMessagePromise &promise - ���ڽ����첽��Ӧ��Ϣ�Ķ���
                   ע��promise�л�ȡ��IMessageAppFrame* ��Ҫ�ͷ�
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
      Parameter:   const REQ &msgbody - ��������Ϣ
      Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
      ///////////////////////////////////////////////////////////////////
      Example:
      {
          IMessagePromise promise;
          pMqContext_->SendMessage(promise, toService_, *pHeader, *pmsg, 5);

          //�ȵ����ǰ����������
          //......

          //��ȡ�첽��Ϣ�ķ��ؽ��
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
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ���ȴ����ؽ��
      Parameter:   IMessagePromise &promise - ���ڽ����첽��Ӧ��Ϣ�Ķ���
                   ע��promise�л�ȡ��IMessageAppFrame* ��Ҫ�ͷ�
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
      Parameter:   const REQ &msgbody - ��������Ϣ
      ///////////////////////////////////////////////////////////////////
      Example:
      {
          IMessagePromise promise;
          pMqContext_->SendMessage(promise, toService_, *pHeader, *pmsg);

          //�ȵ����ǰ����������
          //......

          //��ȡ�첽��Ϣ�ķ��ؽ��
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
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ���ȴ����ؽ��
      Parameter:   IMessagePromise &promise - ���ڽ����첽��Ӧ��Ϣ�Ķ���
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
      Parameter:   const REQ &msgbody - ��������Ϣ
      Parameter:   ACE_UINT32 timeout -  ��Ϣ��ʱʱ�䣨�룩
    ********************************************************************/
    virtual ACE_INT32 SendMessage(IMessagePromise &promise,
                                  const std::string &destSvcKey,
                                  const IMessage &message,
                                  ACE_UINT32 timeout) = 0;

    /********************************************************************
      Method:      SendMessage
      FullName:    IMessageContext::SendMessage
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽��Ϣ���ȴ����ؽ��
      Parameter:   IMessagePromise &promise - ���ڽ����첽��Ӧ��Ϣ�Ķ���
      Parameter:   const std::string &destSvcKey - Ŀ�����key
      Parameter:   const REQ &header - ��������Ϣ����Ϣͷ
      Parameter:   const REQ &msgbody - ��������Ϣ
    ********************************************************************/
    virtual ACE_INT32 SendMessage(IMessagePromise &promise,
                                  const std::string &destSvcKey,
                                  const IMessage &message) = 0;

    /********************************************************************
      Method:      SendAsynResponse
      FullName:    IMessageContext::SendAsynResponse
      Access:      public
      Returns:     UBP_FAIL:ʧ�ܣ�>0:�ɹ�
      Description: �����첽�������Ϣ���кţ���Ӧ�첽��Ӧ��Ϣ
      Parameter:   ACE_UINT64 sequence - �첽��Ϣ�������Ϣ���к�
      Parameter:   const REQ &msgbody - �����͵Ľ����Ϣ
    ********************************************************************/
    virtual ACE_INT32 SendAsynResponse(ACE_UINT64 sequence,
                                       const IMessage &msgbody) = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
     Description: ע����Ϣ������,�ر�ע�⣺ͬһ����������ڵ�Ҫ�����msgId�����ظ�
     Parameter:   IMessageReponseHandler *handler - ��Ϣ�������ָ��,�������ͷ�
     Parameter:   msgPackageName  - ��Ҫ��Ӧ��Ϣ��package��
    ********************************************************************/
    virtual ACE_INT32 RegMsgReponser(IMessageReponseHandler *handler,
                                     const std::string &msgPackageName) = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
     Description: ע����Ϣ������,�ر�ע�⣺ͬһ����������ڵ�Ҫ�����msgId�����ظ�
     Parameter:   IMessageReponseHandler *handler - ��Ϣ�������ָ��,�������ͷ�
     Parameter:   char** msgIds  - ��Ҫ��Ӧ��Ϣ����ϢID list�����磺
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
     Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
     Description: ע����Ϣ������,�ر�ע�⣺ͬһ����������ڵ�Ҫ�����msgId�����ظ�
     Parameter:   IMessageReponseHandler *handler - ��Ϣ�������ָ��,�������ͷ�
     Parameter:   std::list<string>:msgIds  - ��Ҫ��Ӧ��Ϣ����ϢID list
    ********************************************************************/
    virtual ACE_INT32 RegMsgReponser(IMessageReponseHandler *handler,
                                     std::list<std::string> msgIds) = 0;

    /********************************************************************
    Method:      RegMsgDealer
    FullName:    IMessageContext::RegMsgDealer
    Access:      public
    Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
    Description: ע����Ϣ������,�ر�ע�⣺ͬһ����������ڵ�Ҫ�����msgId�����ظ�
    Parameter:   IMessageDealerHandler *handler - �첽��Ϣ�������ָ��,�������ͷ�
    Parameter:   msgPackageName  - ��Ҫ��Ӧ��Ϣ��package��,�Դ����ֲ�ͬ��reponse
    ********************************************************************/
    virtual ACE_INT32 RegMsgDealer(IMessageDealerHandler *handler,
                                   const std::string &msgPackageName) = 0;

    /********************************************************************
     Method:      RegMsgDealer
     FullName:    IMessageContext::RegMsgDealer
     Access:      public
     Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
     Description: ע����Ϣ������,�ر�ע�⣺ͬһ����������ڵ�Ҫ�����msgId�����ظ�
     Parameter:   IMessageDealerHandler *handler - ��Ϣ�������ָ��,�������ͷ�
     Parameter:   char** msgIds  - ��Ҫ��Ӧ��Ϣ����ϢID list�����磺
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
     Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
     Description: ע����Ϣ������,�ر�ע�⣺ͬһ����������ڵ�Ҫ�����msgId�����ظ�
     Parameter:   IMessageReponseHandler *handler - ��Ϣ�������ָ��,�������ͷ�
     Parameter:   std::list<string>:msgIds  - ��Ҫ��Ӧ��Ϣ����ϢID list
    ********************************************************************/
    virtual ACE_INT32 RegMsgDealer(IMessageDealerHandler *handler,
                                   std::list<std::string> msgIds) = 0;

    /********************************************************************
     Method:      Notify
     FullName:    IMessageContext::Notify
     Access:      public
     Returns:     UBP_SUCCESS-�ɹ���������ʧ��
     Description: ����֪ͨ��Ϣ����ȱʡ����Ϣͷ��
     Parameter:   const REQ &msgBody - ��֪ͨ����Ϣ
     Parameter:   const std::string &topic - ��Ϣ��topic
    ********************************************************************/
    virtual ACE_INT32 Notify(const IMessage &msgBody,
                             const std::string &topic,
                             const std::string &filter = "") = 0;

    /********************************************************************
     Method:      Notify
     FullName:    IMessageContext::Notify
     Access:      public
     Returns:     UBP_SUCCESS-�ɹ���������ʧ��
     Description: ����֪ͨ��Ϣ���Դ���Ϣͷ��
     Parameter:   const REQ &header - ֪ͨ��Ϣͷ
     Parameter:   const REQ &msgBody - ֪ͨ��Ϣ��
     Parameter:   const std::string &topic - ��Ϣ��topic
    ********************************************************************/
    virtual ACE_INT32 Notify(const IMessageHeader &header,
                             const IMessage &msgBody,
                             const std::string &topic,
                             const std::string &filter = "") = 0;

    /********************************************************************
     Method:      RegMsgReponser
     FullName:    IMessageContext::RegMsgReponser
     Access:      public
     Returns:     UBP_SUCCESS:ע��ɹ���������ʧ��
     Description: ע����Ϣ������
     Parameter:   const std::string &destSvcKey - Ŀ�����destSvcKey
     Parameter:   IMessageConsumerHandler *handler - ��Ϣ�������ָ��(�����߹���)
    ********************************************************************/
    virtual ACE_INT32 RegMsgConsumer(const std::string &destSvcKey,
                                     const std::string &topic,
                                     IMessageConsumerHandler *cb,
                                     const std::string &filter = "") = 0;

    /********************************************************************
     Method:      UnRegMsgConsumer
     FullName:    IMessageContext::UnRegMsgConsumer
     Access:      public
     Returns:     UBP_SUCCESS:ע���ɹ���������ʧ��
     Description: ע����Ϣ������
     Parameter:   const std::string &destSvcKey - Ŀ�����destSvcKey
     Parameter:   IMessageConsumerHandler *handler - ��Ϣ�������ָ��(�����߹���)
    ********************************************************************/
    virtual ACE_INT32 UnRegMsgConsumer(const std::string &destSvcKey,
                                       const std::string &topic,
                                       IMessageConsumerHandler *cb) = 0;

    //////////////////////////////////////////////////////////////////////////
    //�����Ĵ�������ģʽ��true/false
    virtual bool GetTSLMode() = 0;

    //�����Ĵ�������ģʽ��true/false
    virtual void SetTSLMode(bool isTSL) =  0;

    //Զ�˷���IP�Ĵ�������ģʽ
    virtual ZmqLoginInfo *GetTSLMode(const std::string &remoteIp) =  0;

    //Զ�˷���IP�Ĵ�������ģʽ
    virtual void SetTSLMode(const std::string &remoteIp, const ZmqLoginInfo &lgnInfo) =  0;

    //��½�ӿڣ�LoginResult*��Ҫ�ͷ�
    virtual LoginResult *Login(const std::string &ip, const ReqLogin &login) = 0;

    //for NAT ��½�ӿڣ�LoginResult*��Ҫ�ͷ�
    virtual LoginResult* Login(const std::string& ip,const ReqLogin& login, 
                               int dealport,int pubport) = 0;
	

    //ZMQ��Ȩ�û���
    virtual std::string GetLoginName() = 0;

    //ZMQ��Ȩ����
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
    //����ʱ�䣬��λ������
    ACE_UINT32 RECONNECT_IVL;

    //#���socket����
    ACE_UINT32 MAX_SOCKETS;

    //#�����ߺ�������������ʱ�䣬��λ����
    ACE_UINT32 PUB_SUB_HEARTBEAT_INTERVAL;

    //#������������ʱ����
    ACE_UINT64 SUBSCRIBE_TIMEOUT_TIMES;

    //#������������ʱʱ�䣬��λ����
    ACE_UINT64 SUBSCRIBE_TIMEOUT_TIME;

    //#�������Ϣ�ڶ����еĵȴ�ʱ��,��λ����
    ACE_UINT64 ASYN_RPN_WAIT_TIMEOUT;

    //#������ڻص�������Ϣ��ʱʱ��,��λ����
    ACE_UINT32 REPONSE_HANDLE_MQ_TIMEOUT;

    //#������Ϣ�ص����������
    ACE_UINT32 MSGCALLBACK_WORKER_COUNT;

    //#socket�������ʱ�䣨�룩,������ᱻ�ر�
    ACE_UINT64 MAX_IDLE_TIME;

	//#login��ʱʱ��
    ACE_UINT32 LOGIN_TIMEOUT;
    
	//#�Ƿ�֧��2.1ZMQ
    bool OLD_SUPPORT;

	//#�ͻ���������ϢĬ�ϳ�ʱʱ��
	ACE_UINT32 DEFAULT_REQUEST_TIMEOUT;

	//#�ͻ���Ĭ�Ϸ��ʹ���
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
    Returns:     true:����������false:����������
    Description: ���Ϲ�����������Ϣ�Żᱻ����
    Parameter:   IMessage* msg- ��Ϣ
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
    Returns:     true:����������false:����������
    Description: ���Ϲ�����������Ϣ�Żᱻ����
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
       Returns:     ��Ӧ��ϢRPN*��ʧ�ܷ���NULL��MQ��������
       Description: reponse��Ϣ���մ�����
       Parameter:   IMessageHeader* header- ��Ϣͷ��MQ��������
       Parameter:   IMessage* msgbody - ��Ϣ�壬��MQ��������
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
       Returns:     ACE_INT32 0:�ɹ�,������ʧ��
       Description: �첽��Ϣ���մ�����
       Parameter:   IMessageHeader* header- ��Ϣͷ��MQ��������
       Parameter:   IMessage* msgbody - ��Ϣ�壬��MQ��������
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
       Returns:     ��Ӧ��ϢRPN*��ʧ�ܷ���NULL��MQ��������
       Description: ��Ϣ���մ�����
       Parameter:   IMessageHeader* header- ��Ϣͷ��MQ��������
       Parameter:   IMessage* msgbody - ��Ϣ�壬��MQ��������
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

    //socket�������ʱ�䣬��λms
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
    //��ѯSocket��ȡ��Ϣ
    //timeout-��ѯ����ʱ�䣬��λ΢��;0:������ֱ�ӷ���;-1:����ֱ������Ϣ����
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
