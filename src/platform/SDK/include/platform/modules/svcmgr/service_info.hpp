/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      service_info.hpp
Author:        ZhongWei
Created:       2013/01/05 17/18
Description:   Define ServiceInfo
*********************************************************************/

#ifndef  PLATFORM_MODULES_SCA_SERVICEINFO_HPP_
#define  PLATFORM_MODULES_SCA_SERVICEINFO_HPP_

#include <vector>
#include <string>
#include "ace/Basic_Types.h"
#include "ace/Time_Value.h"
#include "svc_mgr_def.hpp"


namespace ubp
{
namespace platform
{
namespace svcmgr
{
class SVCMGR_IMPORT_EXPORT ServiceType
{
  public:
    ServiceType();
    ServiceType(
      const std::string &svc_type,
      const std::string &default_name,
      const std::string &svc_mode,
      ACE_INT32 pub_port,
      ACE_INT32 deal_port,
      ACE_INT32 offset_port,
      ACE_INT32 ip_type,
      const std::string& cpu_group);

    virtual ~ServiceType();

  public:
    std::string svc_type_;
    std::string default_name_;
    std::string svc_mode_;
    ACE_INT32 pub_port_;
    ACE_INT32 deal_port_;
    ACE_INT32 offset_port_;
    ACE_INT32 ip_type_;
    //ACE_INT32 cpu_type_;
	std::string cpu_group_;
};

class SVCMGR_IMPORT_EXPORT ServiceInfo
{
  public:
    ServiceInfo();
    ServiceInfo(const std::string &host_ip,  //signal ip
                const std::string &om_ip,  //OM ip
                const std::string &med_ip, //media ip
                const std::string &svc_name,
                const std::string &svc_type,
                const std::string &svc_mode,
                ACE_INT32 pub_port,
                ACE_INT32 deal_port,
                ACE_INT32 offset_port,
                const std::string &core,
                const std::string &hostname);

    ServiceInfo(const ServiceInfo &service_info);
    ServiceInfo operator=(const ServiceInfo &svcinfo); //赋值操作

    virtual ~ServiceInfo();

    const std::string &GetServiceKey(void) const;

    bool operator==(const ServiceInfo &svcinfo) const;//对比操作

    void Copy(const std::string &host_ip,
                     const std::string &om_ip,
                     const std::string &med_ip,
                     const ServiceType &svctype); //从service type构造

    std::string ToString();

    const std::string GetDealEndpoint() const;

    const std::string GetPubEndpoint() const;

  private:
    void InitEndpoint(void);

  public:
    std::string host_ip_; //控制面IP
    std::string om_ip_; //管理面IP
    std::string med_ip_; //管理面IP
    std::string svc_name_;
    std::string svc_type_;
    std::string svc_mode_;
    std::string svc_key_;
    ACE_INT32 pub_port_;
    ACE_INT32 deal_port_;
    ACE_INT32 offset_port_;
    std::string core_;      //绑定服务进程到指定的CPU核
    std::string hostname_;  //服务所属的hostname
	std::string packagename_;

  private:
    std::string pubEndpoint_;
    std::string dealEndpoint_;
};

typedef std::vector<std::string> ServiceKeySet;


class SVCMGR_IMPORT_EXPORT ServiceRunInfo: public ServiceInfo
{
public:
    ServiceRunInfo(const ServiceInfo &service_info);
    ServiceRunInfo(const ServiceInfo &service_info, const std::string &code_type);
    ServiceRunInfo();
    ServiceRunInfo(const ServiceRunInfo &service_info);
    ServiceRunInfo operator=(const ServiceRunInfo &service_info);
    virtual ~ServiceRunInfo();
    ACE_INT32 GetLostTimes(void) const;
    ACE_INT32 IncreaseLostTimes();
    ACE_INT32 DecreaseLostTimes();
    ACE_INT32 GetLostLimit(void) const;
    void SetLostLimit(ACE_INT32 limit);
    void ResetLostTimes(void);
public:
    pid_t pid_;
    std::string  state_;   //服务的运行状态
    ACE_INT32 retry_times_;       //服务被自动拉起的次数
    ACE_Time_Value last_start_time_;
    ACE_Time_Value last_begin_running_time_;
    std::string code_type_;
    std::string valgrind_trigger_;
    std::string tcmalloc_trigger_;
    ACE_INT32 init_wait_time_; //服务启动最大等待时间
    ACE_INT32 fini_wait_time_; //服务启动最大等待时间
    ACE_INT32 lost_times_;  //当前失败次数
    ACE_INT32 lost_limit_;  //最大失败次数
    ACE_UINT64 schedule_start_start_; //daem调度：启动该服务的时间点
    ACE_UINT64 schedule_start_waitend_; //daem调度：启动服务等待依赖服务结束时间点
    ACE_UINT64 process_start_start_; //进程启动的时间点(for java)
    ACE_UINT64 process_start_end_; //进程启动完成的时间点(for java)
    ACE_UINT64 schedule_stop_start_; //daem调度：停止服务的时间点
    ACE_UINT64 schedule_stop_waitend_; //daem调度：停止服务等待依赖服务结束时间点
    ACE_UINT64 process_stop_start_; //进程停止的时间点(for java)
    ACE_UINT64 process_stop_end_; //进程停止完成的时间点(for java)
    ACE_INT64 startTaskId_;
    ACE_INT64 stopTaskId_;
    ACE_INT64 pStartHandleAddr_;
    ACE_INT64 pStopHandleAddr_;
    std::string start_java_;
    std::string stop_java_;
    std::string pid_java_;
};


}
}
}

#endif //PLATFORM_MODULES_SCA_SERVICEINFO_HPP_

