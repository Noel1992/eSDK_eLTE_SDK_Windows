/********************************************************************
Copyright 2014,TD-Tech. Co., Ltd.
Filename:    ubp_timer.hpp
Author:      XiaoLongguang
Created:     2012/11/13 10:28
Description: 定时器接口
*********************************************************************/

#ifndef  PLATFORM_MODULES_TIMER_UBP_TIMER_HPP_
#define  PLATFORM_MODULES_TIMER_UBP_TIMER_HPP_

#ifdef WIN32
#ifdef UBP_BUILD_PLATFORM_TIMER_DLL
#define TIMER_IMPORT_EXPORT __declspec(dllexport)
#define TIMER_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
  template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define TIMER_IMPORT_EXPORT __declspec(dllimport)
#define TIMER_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
  extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define TIMER_IMPORT_EXPORT
#define TIMER_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

#include "ace/Event_Handler.h"
#include "platform/modules/frame/module.hpp"

namespace ubp
{
namespace platform
{
namespace timer
{

enum TimerTaskMode
{
  SYNC_MODE,
  ASYN_MODE
};

//////////////////////////////////////////////////////////////////////////////
// class UbpTimerHandler
//////////////////////////////////////////////////////////////////////////////

class TIMER_IMPORT_EXPORT UbpTimerHandler: public ACE_Event_Handler
{
  public:
    UbpTimerHandler(void);
    virtual ~UbpTimerHandler(void);

    //提供重载handle_timeout，放置使用者的EventHandler
    virtual int handle_timeout(const ACE_Time_Value &current_time,
                               const void *act = 0) = 0;
  public:
    long timer_id_;
    bool auto_delete_;
};

//////////////////////////////////////////////////////////////////////////////
// class TimerDispatcher
//////////////////////////////////////////////////////////////////////////////

class TIMER_IMPORT_EXPORT TimerDispatcher
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    //脱离ModuleContext环境，创建一个实例,不建议使用
    static TimerDispatcher *CreateInstance();

    //Todo:　后面加入本Module对外提供的接口
    static TimerDispatcher *instance(void);

    //deprecated:建议使用PostSchedule替换，兼容之前的接口使用
    //注册同步定时任务，同步模式是所有任务在一个线程执行
    //参数：handler - 定时任务处理指针类
    //注意handler默认是调用者自己管理（负责释放）
    //参数：handler - 定时任务处理指针类
    //参数：arg - 定时任务处理时带入的指针，调用者管理
    //参数：abs_time - 注册定时器后开始运行定时任务的时间,单位：秒
    //参数：interval - 定时任务运行的周期，单位：秒
    virtual long Schedule(UbpTimerHandler *handler,
                          void *arg,
                          const int abs_time,
                          const int interval) = 0;

    //deprecated:建议使用PostSchedule替换，兼容之前的接口使用
    virtual long Schedule(UbpTimerHandler *handler,
                          void *arg,
                          const ACE_Time_Value &abs_time,
                          const ACE_Time_Value &interval) = 0;

    //deprecated:建议使用CancelSchedule替换，兼容之前的接口使用
    virtual int Cancel(long timer_id) = 0;

    //注册定时任务，注意handler是Timer负责释放，调用者无需管理
    //参数：handler - 定时任务处理指针类
    //注意handler默认是timer负责释放,也可设置auto_delete_=false开关,则由调用者释放
    //参数：arg - 定时任务处理时带入的指针，调用者管理
    //参数：abs_time - 注册定时器后开始运行定时任务的时间,单位：秒
    //参数：interval - 定时任务运行的周期，单位：秒
    //参数：mode - 同步或异步模式，同步模式指所有定时任务都在一个定时器的调度线程中执行，
    //异步模式则是单独的线程执行
    virtual long PostSchedule(UbpTimerHandler *handler,
                              void *arg,
                              const int abs_time,
                              const int interval,
                              TimerTaskMode mode = ASYN_MODE) = 0;

    virtual long PostSchedule(UbpTimerHandler *handler,
                              void *arg,
                              const ACE_Time_Value &abs_time,
                              const ACE_Time_Value &interval,
                              TimerTaskMode mode = ASYN_MODE) = 0;

    //终止已注册的定时任务
    //参数:timer_id - 定时任务ID
    virtual int CancelSchedule(long timer_id) = 0;
};

}
}
}
#endif //PLATFORM_MODULES_TIMER_UBP_TIMER_HPP_

