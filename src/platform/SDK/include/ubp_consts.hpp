/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    
  Author:      
  Created:     
  Description: 定义UPB常量

*********************************************************************/

#ifndef UBP_CONST_HPP__
#define UBP_CONST_HPP__

#include "platform/modules/util/disabe_warning.hpp"
//#ifdef UNIX
#pragma GCC system_header  //屏蔽告警："-Wunused-variable"
//#endif
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic pop
//#pragma GCC diagnostic ignored "-Wall"

//////////////////////////////////////////////////////////////////////////
//   订阅消息TOPIC全局定义
//////////////////////////////////////////////////////////////////////////
//配置变更消息
static const char *SVC_NAME_DAEM = "ubp_daem";
static const char *SVC_NAME_CM = "ubp_cm";
static const char *SVC_NAME_FM = "ubp_fm";
static const char *SVC_NAME_LM = "ubp_lm";
static const char *SVC_NAME_MONITOR = "ubp_monitor";
static const char *SVC_NAME_BCC = "ubp_bcc";
static const char *SVC_NAME_BDC01 = "ubp_bdc01";
static const char *SVC_NAME_DEC_VGW = "ubp_dec_vgw";
static const char *SVC_NAME_GIS = "ubp_gis";
static const char *SVC_NAME_MRS_MGR = "ubp_mrs_mgr";
static const char *SVC_NAME_LIC_MAN = "ubp_lic_man";
static const char *SVC_NAME_MRS_VGW = "ubp_mrs_vgw";
static const char *SVC_NAME_SDS_ADAPTER = "ubp_sds_adapter";
static const char *SVC_NAME_MRS_NODE_MGR = "ubp_mrs_node_mgr";
static const char *SVC_NAME_MRS_REC_MGR = "ubp_mrs_rec_man";
static const char *SVC_NAME_MRS_REC_SINKER = "ubp_mrs_rec_sinker";
static const char *SVC_NAME_MRS_UE = "ubp_ue";
static const char *SVC_NAME_UDC_OPM = "udc_opm";
static const char *SVC_NAME_UDC_PUSH = "udc_push";
static const char *SVC_NAME_UDC_UOM = "udc_uom";

static const char *SVC_TYPE_DAEM = "daem_agent";
static const char *SVC_TYPE_CM = "cm_agent";
static const char *SVC_TYPE_FM = "fm_agent";
static const char *SVC_TYPE_LM = "lm_agent";
static const char *SVC_TYPE_MONITOR = "mon_agent";
static const char *SVC_TYPE_BCC = "bcc_agent";
static const char *SVC_TYPE_BDC01 = "bdc_agent";
static const char *SVC_TYPE_DEC_VGW = "dec_vgw_agent";
static const char *SVC_TYPE_GIS = "gis_agent";
static const char *SVC_TYPE_MRS_MGR = "mrs_mgr_agent";
static const char *SVC_TYPE_LIC_MAN = "lic_man_agent";
static const char *SVC_TYPE_MRS_VGW = "mrs_vgw_agent";
static const char *SVC_TYPE_SDS_ADAPTER = "sds_adapter_agent";
static const char *SVC_TYPE_MRS_NODE_MGR = "mrs_node_mgr_agent";
static const char *SVC_TYPE_MRS_REC_MGR = "mrs_rec_man_agent";
static const char *SVC_TYPE_MRS_REC_SINKER = "mrs_rec_sinker_agent";
static const char *SVC_TYPE_MRS_UE = "ue_agent";
static const char *SVC_TYPE_UDC_OPM = "opm_agent";
static const char *SVC_TYPE_UDC_PUSH = "push_agent";
static const char *SVC_TYPE_UDC_UOM = "uom_agent";
static const char *SVC_TYPE_UDC_EBS = "web_agent"; //added by wyj for F0012_0120ebs

#endif //UBP_CONST_HPP__
