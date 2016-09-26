/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      thread_def.hpp
Author:        XiaoLongguang
Created:       2012/01/07 14:45
Description:
*********************************************************************/
#ifndef  PLATFORM_MODULES_THREAD_UBP_THREAD_POOL_HPP_
#define  PLATFORM_MODULES_THREAD_UBP_THREAD_POOL_HPP_

#include <ace/Method_Request.h>
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "platform/modules/error/ubp_error.hpp"
#include "thread_def.hpp"

namespace ubp
{
namespace platform
{
namespace thread
{

/* 用于使用者放置任务执行 */
class THREAD_IMPORT_EXPORT WorkerRequest : public ACE_Method_Request
{
public:
    /* 重载call，放置使用者的任务，如多线程使用同资源，请注意加锁 */
    virtual ACE_INT32 call(void) = 0;

public:
    ACE_UINT64 task_id_;
};

class THREAD_IMPORT_EXPORT ThreadPool// : public ACE_Task_Base
      : public ubp::platform::frame::Module
{
  public:
      //Module must support a static function "ModuleName"
      static const std::string &ModuleName(void);

    static ThreadPool *Instance(void);

    /* 将任务放入线程池执行，函数内部会释放，不需要使用者释放 */
    virtual ACE_UINT64 PostRequest(WorkerRequest *request) = 0;

    virtual void SetMaxThread(ACE_INT32) = 0;

    /* 返回值：0-领导者，1-跟随者，2-工作中，3-准备就绪，4-停止 */
    virtual ACE_INT32 GetStatusFromId(ACE_UINT64 task_id) = 0;

    /* 杀掉任务挂死的线程 */
    virtual ACE_INT32 KillTask(ACE_UINT64 task_id) = 0;

    ////等待task结束
    //virtual ACE_INT32 WaitTask(ACE_UINT64 task_id) = 0;

    /* 停止线程池 */
    virtual ACE_INT32 StopThreadPool(void) = 0;

};

}
}
}

#endif //PLATFORM_MODULES_THREAD_UBP_THREAD_MGR_HPP_
