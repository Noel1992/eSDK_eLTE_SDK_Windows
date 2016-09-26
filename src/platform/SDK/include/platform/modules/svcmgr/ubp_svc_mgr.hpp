/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    ubp_svc_mgr.hpp
Author:      ZhongWei
Created:     2012/10/26 10:28
Description: interface for  ServiceMgr
*********************************************************************/

#ifndef  PLATFORM_MODULES_SCA_UBP_SVC_MGR_HPP_
#define  PLATFORM_MODULES_SCA_UBP_SVC_MGR_HPP_

#include "service_info.hpp"
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"

#ifdef WIN32
#define SUPRRESS_UNUSED_WARN
#else
#define SUPRRESS_UNUSED_WARN __attribute__ ((used))
#endif//WIN32

#define UBP_EAPP_4_0 1

namespace ubp
{
namespace platform
{
namespace svcmgr
{

static const char *SVCMODE_MDC SUPRRESS_UNUSED_WARN = "MDC";
static const char *SVCMODE_MRS_C  SUPRRESS_UNUSED_WARN = "MRS_C";
static const char *SVCMODE_MRS_D  SUPRRESS_UNUSED_WARN = "MRS_D";
static const char *SVCMODE_BCC_ONLY SUPRRESS_UNUSED_WARN = "MDC(BCC Only)";
static const char *SVCMODE_BDC_ONLY SUPRRESS_UNUSED_WARN = "MDC(BDC Only)";
static const char *SVCMODE_MDC_MRS SUPRRESS_UNUSED_WARN = "MDC+MRS";

//定义服务变更的事件类型
enum SERIVCE_CHANGE_EVENT_TYPE
{
  SERVICE_ADD = 0,      //新增服务
  SERVICE_DEL,          //删除服务
  SERVICE_MOD           //修改服务
};

//定义服务变更事件处理器原型
class ServiceInfoChangeHandler
{
  public:
    virtual ACE_INT32 handle_change(SERIVCE_CHANGE_EVENT_TYPE event_type,
                                    ServiceInfo *service_info_ptr) = 0;

    void *parent_;
};

class SVCMGR_IMPORT_EXPORT ServiceMgr
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    //Todo:　后面加入本Module对外提供的接口
    //脱离ModuleContext环境，创建一个实例,不建议使用
    static ServiceMgr *CreateInstance(ubp::platform::frame::ModuleContext *context);

    //从deploy_policy.xml加载数据,only_local为true表示只加载本机的服务
    virtual ACE_INT32 LoadPolicy(const std::string &service_name, const char *file_name = NULL) = 0;

    //是否初始化本地服务
    virtual bool IsInitLocalService() = 0;

    //是否兼容2.1旧版本
//    virtual void SetCompatibleService(bool flag) = 0;

    //获取本机所属的daem的Service Key
    virtual std::string GetDaemServiceKey(void) = 0;

    //GetServiceKey:获取指定服务的Service Key
    //svc_name:服务名；host_ip:服务的IP地址，NULL表示是本机
    //返回Service Key
    virtual std::string GetServiceKey(const char *svc_name,
                                      const char *host_ip = NULL) = 0;

    //函数功能：获取制定的服务类型的Service Key
    //返回值：service数量，svc_key_array: Service Key集合
    //domain:域名称，其中，NULL：本域； * ：所有域；
    //host_ip:主机所在的IP;NULL:所有主机
    virtual ACE_INT32 GetServiceKeyByType(ServiceKeySet &svc_key_array,
                                          const char *svc_type,
                                          const char *domain = NULL,
                                          const char *host_ip = NULL) = 0;

    //根据主机名获取
    virtual ACE_INT32 GetServiceKeyByTypeH(ServiceKeySet &svc_key_array,
                                           const char *svc_type,
                                           const char *domain = NULL,
                                           const char *host_name = NULL) = 0;

    //函数功能：获取制定的服务类型的ServiceInfo
    //返回值：service数量，svc_array: ServiceInfo集合
    //domain:域名称，其中，NULL：本域； * ：所有域；
    //host_ip:主机所在的IP;NULL:所有主机
    virtual ACE_INT32 GetServicesByType(std::vector<ServiceInfo> &svc_array,
                                        const char *svc_type,
                                        const char *domain = NULL,
                                        const char *host_ip = NULL) = 0;

    //获取本系统的服务,如果有masterip取masterip的服务,没有则取本机的服务
    virtual ACE_INT32 GetLocalSysSvcsByType(std::vector<ServiceInfo> &svc_array,
                                            const char *svc_type) = 0;

    //根据主机名获取
    virtual ACE_INT32 GetServicesByTypeH(std::vector<ServiceInfo> &svc_array,
                                         const char *svc_type,
                                         const char *domain = NULL,
                                         const char *host_name = NULL) = 0;

    //获取Service Info相关接口
    //GetCurrentServiceInfo:获取本服务的Service Info，返回到svcinfo中
    virtual ACE_INT32 GetCurrentServiceInfo(ServiceInfo &svcinfo) = 0;

    //GetServiceInfo:获取指定ServiceKey的ServiceInfo，返回到svcinfo中
    virtual ACE_INT32 GetServiceInfo(ServiceInfo &svcinfo,
                                     const std::string &svcKey) = 0;

    //判断ip是否包含在ServiceInfo的signal ip或者om ip内
    //return:true/false
    virtual bool HasIPService(const std::string &ip) = 0;

    //GetServicesAll:获取所有主机上所有服务的Service Info的MAP
    //const ServiceInfoMap &GetServicesAll(); 修改成如下接口，以便多线程并发控制
    virtual ACE_INT32 GetServicesAll(std::vector<ServiceInfo> &svc_array) = 0;

    //GetServicesByHost:获取指定主机上所有服务的Service Info放入svc_array，host_ip不填表示本机，返回service数量
    virtual ACE_INT32 GetServicesByHost(std::vector<ServiceInfo> &svc_array,
                                        const char *host_ip = NULL) = 0;

    //GetServicesByHostName:获取指定主机上所有服务的Service Info放入svc_array，aHostName 不填表示本机，返回service数量
    virtual ACE_INT32 GetServicesByHostName(std::vector<ServiceInfo> &svc_array,
                                            const char *aHostName = NULL) = 0;
    virtual const std::string &GetHostInfoByKey(const std::string &key) = 0;
    virtual const std::string &OmIP() = 0;
    virtual const std::string &MedIP() = 0;
    virtual const std::string &SignalIP() = 0;
    virtual const std::string &MasterIP() = 0;
    virtual const std::string &ServiceMode() = 0;
    virtual const std::string &VeritasMode() = 0;

    //for udc
    virtual const std::string &UDCOmIP() = 0;
    virtual const std::string &UDCMedIP() = 0;
    virtual const std::string &UDCSignalIP() = 0;


    //for NAT

    virtual void SetBrokerPort(const std::string &ip, int dealport, int pubport) = 0;

    //virtual bool IsLocalIP(const std::string& ip) = 0;
    //注册服务信息变更处理器，当服务信息发生变更时，会调用注册了的处理函数
    virtual ACE_INT32 RegistServiceChangeHandler(ServiceInfoChangeHandler *phandler, void *parent) = 0;

    //注销服务信息变更处理器
    virtual ACE_INT32 UnregistServiceChangeHandler(ServiceInfoChangeHandler *phandler) = 0;

    //检查本机的服务是否有变化，并调用相应的handler
    virtual ACE_INT32 CheckChanged() = 0;

    //更新一台主机ServiceInfo,然后检查是否有变化，并调用相应的handler
    //virtual ACE_INT32 UpdateServiceInfosByHost(std::string host_ip,
    //    std::vector<ServiceInfo> svcs_ter) = 0;

    //更新一台主机ServiceInfo,然后检查是否有变化，不调用相应的handler
    virtual ACE_INT32 UpdateServiceInfosByHostNameA(std::string host_name,
        std::vector<ServiceInfo> svcs_ter) = 0;

    //更新所有ServiceInfo,然后检查是否有变化，并调用相应的handler
    virtual ACE_INT32 UpdateServiceInfos(std::vector<ServiceInfo> svcs_ter,
                                         ACE_INT32 version_id = 0) = 0;

    //更新所有ServiceInfo,检查是否有变化，但不调用handler
    //virtual ACE_INT32 UpdateServiceInfosA(std::vector<ServiceInfo> svcs_ter,
    //  ACE_INT32 version_id = 0)=0;

    //取得ServiceInfo的版本
    virtual ACE_INT32 GetVersion(void) = 0;

    virtual ACE_INT32 SetVersion(ACE_INT32 aVersion) = 0;

    //获取主机ip
    virtual const std::string &GetMasterIP() = 0;

    //删除服务
    virtual ACE_INT32 DelSvcsByHostName(const std::string &host_name) = 0;
    //根据ip删除服务
    //virtual ACE_INT32 DelSvcsByIP(const std::string &aIP)=0;

    virtual ACE_INT32 GetPortOffset() const = 0;

    virtual std::string GetOmBrokerDealEndpoint() const = 0;
    virtual std::string GetOmBrokerPubEndpoint() const = 0;
    virtual std::string GetSgnBrokerDealEndpoint() const = 0;
    virtual std::string GetSgnBrokerPubEndpoint() const = 0;
    virtual std::string GetMQBrokerXSubEndpoint() const = 0;

    //for udc
    virtual std::string GetUDCOmBrokerDealEndpoint() const = 0;
    virtual std::string GetUDCOmBrokerPubEndpoint() const = 0;
    virtual std::string GetUDCSgnBrokerDealEndpoint() const = 0;
    virtual std::string GetUDCSgnBrokerPubEndpoint() const = 0;

    virtual std::string GetProxyDealEndpoint(const ServiceInfo &svcinfo) = 0;
    virtual std::string GetProxyPubEndpoint(const ServiceInfo &svcinfo) = 0;

    //get the dependent services
    virtual std::vector<std::string> GetServicesDependOn(const std::string &svc_name) = 0;
    //get the services depended by
    virtual std::vector<std::string> GetServicesDependBy(const std::string &svc_name) = 0;
    //当前的应用模式是不是从机
    virtual bool IsServiceModeSlave() = 0;
    virtual const std::string GetHostCfg(const std::string &key_name) = 0;

    //for process status
    virtual void setStatus(const std::string &status) = 0;

    virtual const std::string &getStatus() = 0;

    virtual bool IsRunningStatus() const = 0;

    virtual ACE_INT32 GetExtraInfoFromAgent(const std::string &svc_type,
                                    ServiceRunInfo *svc_info_ptr)  = 0;

};

}
}
}

#endif //PLATFORM_MODULES_SCA_UBP_SVC_MGR_HPP_

