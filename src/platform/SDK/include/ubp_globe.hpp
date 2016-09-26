/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    ubp_globe.hpp
  Author:      Wu Gang
  Created:     2012/12/19 20:11
  Description: 定义UPB全局变量

*********************************************************************/

#ifndef UBP_GLOBE_HPP__
#define UBP_GLOBE_HPP__

#include "ace/Basic_Types.h"

#ifdef WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4068)
#else
#pragma GCC system_header  //屏蔽告警："-Wunused-variable"
#endif // WIN32
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic pop
//#pragma GCC diagnostic ignored "-Wall"

//////////////////////////////////////////////////////////////////////////
//   订阅消息TOPIC全局定义
//////////////////////////////////////////////////////////////////////////
//配置变更消息
static const char *TOPIC_CM_MO_CHANGE = "UBP.CM.BMO";  
//配置变更消息,兼容2.1
static const char *TOPIC_CM_MO_CHANGE_CMB = "UBP.CM.MO";  
//DC 不要密码
static const char *TOPIC_CM_MO_CHANGE_NPWD = "UBP.CM.NPWD";
//UDC配置变更消息
static const char *TOPIC_CM_MO_CHANGE_UDC = "UBP.CM.UDC";  
//DLD下载状态同步消息
static const char *TOPIC_CM_ACCOUNT_DLD_STATE ="UBP.CM.DLDSYNC";
//配置状态同步消息
static const char *TOPIC_CM_ACCOUNT_SYNC_STATE = "UBP.CM.ACTSYNC";  
//导出配置同步消息
static const char *TOPIC_CM_EXP_CFG_STATE = "UBP.CM.EXPCFG";  
//日志集中采集消息
static const char *TOPIC_MAINTAIN_LOGCOLLECT = "UBP.MAINTAIN.LOGCOLLECT";  
//服务信息变更消息
static const char *TOPIC_SCA_SVC_INFO_CHANGE = "UBP.SCA.INFO.C";  
//服务信息变更消息(daem之间,目前没有注册,因为服务信息在启动之后不会再变化)
static const char *TOPIC_SCA_SVC_INFO_CHANGE_DAEM = "UBP.SCA.INFO.TO_DAEM";  
//服务状态变更消息
static const char *TOPIC_SCA_SVC_STATE_CHANGE = "UBP.SCA.STAT.C";  
//服务状态变更消息(DAEM之间传送)
static const char *TOPIC_SCA_SVC_STATE_CHANGE_DAEM = "UBP.SCA.STAT.TO_DAEM";  
//登陆服务器的登陆信息状态(本机DAEM下的服务接收)
static const char *TOPIC_SCA_LOGIN_INFO_STATE = "UBP.SCA.LGNINFO.STATE"; 

//OMC通道数据备份上传状态同步消息
static const char *TOPIC_EOMC_DLD_STATE ="UBP.EOMC.BKPCFGFILE";

//告警消息
static const char *TOPIC_FM_ALARM = "UBP.FM.ALARM";  
static const char *TOPIC_FM_ALARM_MDC = "MDC"; 
static const char *TOPIC_FM_ALARM_UDC = "UDC"; 
static const char *TOPIC_FM_ALARM_PLT = "PLT"; 
//软件升级消息
static const char *TOPIC_SWM_UPGRADE ="UBP.SWM.UPGRADE";

static const char *TOPIC_MON_SVCSTATE ="UBP.MONITOR.SVCSTATE";
static const char *TOPIC_MON_DISKSTATE ="UBP.MONITOR.DISKSTATE";
static const char *TOPIC_MON_MEMSTATE ="UBP.MONITOR.MEMSTATE";
static const char *TOPIC_MON_CPUSTATE ="UBP.MONITOR.CPUSTATE";
static const char *TOPIC_MON_DBSTATE  ="UBP.MONITOR.DBSTATE";

static const char *TOPIC_LICENSE_BROADCAST ="UBP.LICENSE.BROADCAST";//软件升级消息
static const char *TOPIC_LICENSE_EXPIREDDAYS ="UBP.LICENSE.BROADCAST";//
static const char *TOPIC_HEARTBEAT ="UBP.HEARTBEAT.BROADCAST";//心跳广播
static const char *TOPIC_LICENSE_FTPTRANSFER ="UBP.LIC_MAN.FTPTRANSFER";//上报eOMC进度

static const char *TOPIC_BCC_USER_STATUS = "PeriodStatusInfo";  //bcc to dc/bcc , user status report
static const char *TOPIC_BCC_ALL_STATUS = "AllStatusInfo";  //bcc to dc/bcc , all status report
static const char *TOPIC_BCC_BCC_ALL_STATUS = "SccAllStatusInfo";
static const char *TOPIC_BCC_BCC_PERIOD_STATUS = "SccPeriodStatusInfo";
static const char *TOPIC_BCC_PATCH_CONFIG = "PatchConfigInd";  //bcc to dc , patch group config ind
static const char *TOPIC_BCC_DMG_FAIL_LIST = "SccDcAssignDgnaFailedList";  //bcc to dc , dmg fail list
static const char *TOPIC_BCC_PRES_ALL_STATUS = "PresAllStatusInfo";  //bcc to pres , all status report
static const char *TOPIC_BCC_PRES_PERIOD_STATUS = "PresPeriodStatusInfo";  //bcc to pres , all status report

//////////////////////////////////////////////////////////////////////////
//UBP系统的package全局定义,在服务端处理消息响应时，
//同一服务的不同的模块可以注册不同的responser,以package名称区分不同的response
//////////////////////////////////////////////////////////////////////////
static const char *MSG_PACKAGE_PLATFORM = "ubp.platform";
static const char *MSG_PACKAGE_SCA = "ubp.platform.sca";
static const char *MSG_PACKAGE_BCC = "ubp.bcc";
static const char *MSG_PACKAGE_DIAM = "ubp.diam";
static const char *MSG_PACKAGE_BDC = "ubp.bdc";
static const char *MSG_PACKAGE_CM = "ubp.cm";
static const char *MSG_PACKAGE_EAPP_UDC = "eapp.udc";
static const char *MSG_PACKAGE_FM = "ubp.fm";
static const char *MSG_PACKAGE_MAINTAIN = "ubp.maintain";
static const char *MSG_DcCheck_MAINTAIN= "ubp.maintain.DcCheckSend";
static const char *MSG_PACKAGE_LM = "ubp.lm";
static const char *MSG_PACKAGE_MRS = "ubp.mrs";
static const char *MSG_PACKAGE_VGW = "ubp.vgw";
static const char *MSG_PACKAGE_MONITOR ="ubp.monitor";
static const char *MSG_PACKAGE_SWM ="ubp.swm";
static const char *MSG_PACKAGE_LIC_MAN  =   "ubp.lic_man";
static const char *MSG_PACKAGE_HPGW  =   "ubp.hpgw";
static const char *MSG_PACKAGE_PRES  =   "ubp.pres";
static const char *MSG_PACKAGE_EOMC  = "ubp.eomc";


//////////////////////////////////////////////////////////////////////////
//MQ查询消息头的ID
//////////////////////////////////////////////////////////////////////////
static const char *MQ_QUERY_MSG_ID = "ubp.platform.MqQueryMsg";
static const char *SYNC_MSG_ID = "eapp.udc.syncinfo";
static const char *DLDCFG_MSG_ID = "eapp.udc.dldcfgfiles";
static const char *ACTCFG_MSG_ID = "eapp.udc.actcfgfile";
static const char *LOGCOLLECT_MSG_ID = "ubp.maintain.logcollect";
static const char *LOGRESULT_MSG_ID = "ubp.maintain.logresult";
static const char *ULDCFG_MSG_ID = "eapp.udc.uldcfgfile";
static const char *EXPCFG_MSG_ID = "eapp.udc.expcfgfile";

static const char *OMC_BKP_CFG_MSG_ID  =   "ubp.eomc.bkpcfgfile";
static const char *OMC_DLD_CFG__MSG_ID  =   "ubp.eomc.dldcfgfile";
static const char *OMC_CFG_RESP_MSG_ID  =   "ubp.eomc.bkpcfgfileResponse";

//////////////////////////////////////////////////////////////////////////
//IMessageHeader中操作码码的全局定义
//message IMessageHeader
//{
//    required string msg_id = 1;   //待发送消息的ID，全局唯一，
//                                    类似"ubp.platform.ScaMessage"的字符串
//    required string src_svc_id = 2;  //发送方的svc_key,用于唯一标识发起方
//    optional string dest_svc_id = 3;  //目标方的svc_key,用于唯一标识目标方
//    optional uint32 operator_code = 4;  //全局操作码，有规划的区段
//    optional string module = 5;   //所属模块，预留暂时不用
//}
// Platform : 0   ~ 999 (CM:0~99;SWM:100~199)     
// BCC      :1000 ~ 1999;
// BDC      :2000 ~ 2999;
// MRS      :3000 ~ 3999;
// VGW      :4000 ~ 4999;
// DC       :5000 ~ 5999;
// 数据中心  :10000~ 10999;
//////////////////////////////////////////////////////////////////////////
enum OperateType {
	OP_ADD = 1,
	OP_MODIFY,
	OP_DELETE,
	OP_QUERY,
	OP_BATCH_ADD,
	OP_BATCH_MODIFY,
	OP_BATCH_DELETE,
	OP_BATCH_IMPORT,
	OP_BATCH_IMPORT_CSV,
	OP_BATCH_IMPORT_XML,
	OP_COLLECT_TRACE,
	OP_QUERY_SYNC_INFO,
	OP_DLDCFG,
	OP_ACTCFG,
	OP_VIEW_UPDATE,
  OP_FULL_FIELD_MODIFY,
  OP_BATCH_FULL_FIELD_MODIFY,
  OP_NOTIFY
};

//////////////////////////////////////////////////////////////////////////
//MQ查询消息头的ID
//////////////////////////////////////////////////////////////////////////
//内部ReponseTask（同步）的binding地址，加前缀避免重复
static const char *BCC_BINDING_MSG_QUEUE_STR = "inproc://ubp.bcc.msgqueue_"; 
static const char *DIAM_BINDING_MSG_QUEUE_STR = "inproc://ubp.diam.msgqueue_"; 
static const char *PRES_BINDING_MSG_QUEUE_STR = "inproc://ubp.pres.msgqueue_"; 
static const char *SIP_SCC_BINDING_MSG_QUEUE_STR = "inproc://ubp.sip_scc.msgqueue_"; 
static const char *SIP_SCC_BINDING_SIPEVENT_MSG_QUEUE_STR = "inproc://ubp.sip_scc.sipevent.msgqueue_";


//CM每批量配置下发给终端的最大个数
static const ACE_UINT32 MAX_BATCH_DISPATCH_TO_UE = 300;

static const ACE_UINT32 MDC_TYPEID = 1800;
static const ACE_UINT32 MRS_TYPEID = 1850;
static const ACE_UINT32 UDC_TYPEID = 1860;

#endif //UBP_GLOBE_HPP__
