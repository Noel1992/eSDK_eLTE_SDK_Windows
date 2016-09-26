/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      ubp_svc_env.hpp
Author:        ZhongWei
Created:       2012/12/28 15/00
Description:   interface for ServiceEnv
*********************************************************************/
#ifndef  PLATFORM_MODULES_SCA_UBP_SVC_ENV_HPP_
#define  PLATFORM_MODULES_SCA_UBP_SVC_ENV_HPP_

#include "sca_def.hpp"
#include "ubp_proc_proxy.hpp"
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"

namespace ubp
{
namespace platform
{
namespace sca
{
class SCA_IMPORT_EXPORT ServiceEnv
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    //Todo:　后面加入本Module对外提供的接口
    static ServiceEnv *instance(void);
    virtual ACE_INT32 Init(const std::string service_name, const std::string service_type) = 0;
    //获取本服务相关的数据
    virtual std::string ServiceName(void) = 0;
    virtual std::string ServiceType(void) = 0;
	virtual ACE_INT32 LoadCommonModules( void ) { return 1; }

    template<typename T>
    T* GetModule()
    {
        T* ptr = NULL;      
        if (Context() != NULL)
        {
            ptr = Context()->ModuleRef<T>();
        }
        return ptr;
    }

    //virtual ubp::platform::mq::IMessageContext *GetMQContext(void) = 0;
    //已取消，使用GetModule<IMessageContext>()接口代替

    //virtual ConfigMgr *GetCofigMgr(void) = 0; 
    //已取消，使用GetModule<ConfigMgr>()接口代替

    //virtual ubp::platform::svcmgr::ServiceMgr *GetServiceMgr(void) = 0;
    //已取消，使用GetModule<ConfigMgr>()接口代替

    //virtual LF_ThreadPool *GetThreadPool(void) = 0;
    //已取消，使用GetModule<ThreadPool>()接口代替

    //virtual ubp::platform::timer::TimerDispatcher *GetTimerDispatcher(void) = 0;
    //已取消，使用GetModule<TimerDispatcher>()接口代替

    //进程的对象的Attach和调用
    virtual void AttachProxy(ProcessProxy *proxy) = 0;
    virtual ProcessProxy *GetProcProxy(void) = 0;
};
typedef ServiceEnv ServiceEnv_T;
}
}
}


#endif //PLATFORM_MODULES_SCA_UBP_SVC_ENV_HPP_
