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

//������������¼�����
enum SERIVCE_CHANGE_EVENT_TYPE
{
  SERVICE_ADD = 0,      //��������
  SERVICE_DEL,          //ɾ������
  SERVICE_MOD           //�޸ķ���
};

//����������¼�������ԭ��
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

    //Todo:��������뱾Module�����ṩ�Ľӿ�
    //����ModuleContext����������һ��ʵ��,������ʹ��
    static ServiceMgr *CreateInstance(ubp::platform::frame::ModuleContext *context);

    //��deploy_policy.xml��������,only_localΪtrue��ʾֻ���ر����ķ���
    virtual ACE_INT32 LoadPolicy(const std::string &service_name, const char *file_name = NULL) = 0;

    //�Ƿ��ʼ�����ط���
    virtual bool IsInitLocalService() = 0;

    //�Ƿ����2.1�ɰ汾
//    virtual void SetCompatibleService(bool flag) = 0;

    //��ȡ����������daem��Service Key
    virtual std::string GetDaemServiceKey(void) = 0;

    //GetServiceKey:��ȡָ�������Service Key
    //svc_name:��������host_ip:�����IP��ַ��NULL��ʾ�Ǳ���
    //����Service Key
    virtual std::string GetServiceKey(const char *svc_name,
                                      const char *host_ip = NULL) = 0;

    //�������ܣ���ȡ�ƶ��ķ������͵�Service Key
    //����ֵ��service������svc_key_array: Service Key����
    //domain:�����ƣ����У�NULL������ * ��������
    //host_ip:�������ڵ�IP;NULL:��������
    virtual ACE_INT32 GetServiceKeyByType(ServiceKeySet &svc_key_array,
                                          const char *svc_type,
                                          const char *domain = NULL,
                                          const char *host_ip = NULL) = 0;

    //������������ȡ
    virtual ACE_INT32 GetServiceKeyByTypeH(ServiceKeySet &svc_key_array,
                                           const char *svc_type,
                                           const char *domain = NULL,
                                           const char *host_name = NULL) = 0;

    //�������ܣ���ȡ�ƶ��ķ������͵�ServiceInfo
    //����ֵ��service������svc_array: ServiceInfo����
    //domain:�����ƣ����У�NULL������ * ��������
    //host_ip:�������ڵ�IP;NULL:��������
    virtual ACE_INT32 GetServicesByType(std::vector<ServiceInfo> &svc_array,
                                        const char *svc_type,
                                        const char *domain = NULL,
                                        const char *host_ip = NULL) = 0;

    //��ȡ��ϵͳ�ķ���,�����masteripȡmasterip�ķ���,û����ȡ�����ķ���
    virtual ACE_INT32 GetLocalSysSvcsByType(std::vector<ServiceInfo> &svc_array,
                                            const char *svc_type) = 0;

    //������������ȡ
    virtual ACE_INT32 GetServicesByTypeH(std::vector<ServiceInfo> &svc_array,
                                         const char *svc_type,
                                         const char *domain = NULL,
                                         const char *host_name = NULL) = 0;

    //��ȡService Info��ؽӿ�
    //GetCurrentServiceInfo:��ȡ�������Service Info�����ص�svcinfo��
    virtual ACE_INT32 GetCurrentServiceInfo(ServiceInfo &svcinfo) = 0;

    //GetServiceInfo:��ȡָ��ServiceKey��ServiceInfo�����ص�svcinfo��
    virtual ACE_INT32 GetServiceInfo(ServiceInfo &svcinfo,
                                     const std::string &svcKey) = 0;

    //�ж�ip�Ƿ������ServiceInfo��signal ip����om ip��
    //return:true/false
    virtual bool HasIPService(const std::string &ip) = 0;

    //GetServicesAll:��ȡ�������������з����Service Info��MAP
    //const ServiceInfoMap &GetServicesAll(); �޸ĳ����½ӿڣ��Ա���̲߳�������
    virtual ACE_INT32 GetServicesAll(std::vector<ServiceInfo> &svc_array) = 0;

    //GetServicesByHost:��ȡָ�����������з����Service Info����svc_array��host_ip�����ʾ����������service����
    virtual ACE_INT32 GetServicesByHost(std::vector<ServiceInfo> &svc_array,
                                        const char *host_ip = NULL) = 0;

    //GetServicesByHostName:��ȡָ�����������з����Service Info����svc_array��aHostName �����ʾ����������service����
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
    //ע�������Ϣ�������������������Ϣ�������ʱ�������ע���˵Ĵ�����
    virtual ACE_INT32 RegistServiceChangeHandler(ServiceInfoChangeHandler *phandler, void *parent) = 0;

    //ע��������Ϣ���������
    virtual ACE_INT32 UnregistServiceChangeHandler(ServiceInfoChangeHandler *phandler) = 0;

    //��鱾���ķ����Ƿ��б仯����������Ӧ��handler
    virtual ACE_INT32 CheckChanged() = 0;

    //����һ̨����ServiceInfo,Ȼ�����Ƿ��б仯����������Ӧ��handler
    //virtual ACE_INT32 UpdateServiceInfosByHost(std::string host_ip,
    //    std::vector<ServiceInfo> svcs_ter) = 0;

    //����һ̨����ServiceInfo,Ȼ�����Ƿ��б仯����������Ӧ��handler
    virtual ACE_INT32 UpdateServiceInfosByHostNameA(std::string host_name,
        std::vector<ServiceInfo> svcs_ter) = 0;

    //��������ServiceInfo,Ȼ�����Ƿ��б仯����������Ӧ��handler
    virtual ACE_INT32 UpdateServiceInfos(std::vector<ServiceInfo> svcs_ter,
                                         ACE_INT32 version_id = 0) = 0;

    //��������ServiceInfo,����Ƿ��б仯����������handler
    //virtual ACE_INT32 UpdateServiceInfosA(std::vector<ServiceInfo> svcs_ter,
    //  ACE_INT32 version_id = 0)=0;

    //ȡ��ServiceInfo�İ汾
    virtual ACE_INT32 GetVersion(void) = 0;

    virtual ACE_INT32 SetVersion(ACE_INT32 aVersion) = 0;

    //��ȡ����ip
    virtual const std::string &GetMasterIP() = 0;

    //ɾ������
    virtual ACE_INT32 DelSvcsByHostName(const std::string &host_name) = 0;
    //����ipɾ������
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
    //��ǰ��Ӧ��ģʽ�ǲ��Ǵӻ�
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

