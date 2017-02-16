/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

/********************************************************************
filename    :    XMLProcess.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    XML入参解析，XML出参赋值
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12
*********************************************************************/

#ifndef __ELTE_OCX_XML_PROCESS_H__
#define __ELTE_OCX_XML_PROCESS_H__

#include "mq/gisreportindicator.h"

#define SET_SDS_XML() \
	(void)xml.AddElem("SdsFrom");	\
	(void)xml.SetElemValue(pSdsMessage->getSdsFrom().c_str());	\
	(void)xml.AddElem("SdsSubject");	\
	(void)xml.SetElemValue(pSdsMessage->getSdsSubject().c_str());	\
	(void)xml.AddElem("SdsDirection");	\
	(void)xml.SetElemValue("false");	\
	(void)xml.AddElem("SdsDate");	\
	(void)xml.SetElemValue(pSdsMessage->getSdsDate().c_str());	\
	(void)xml.AddElem("SdsTime");	\
	(void)xml.SetElemValue(pSdsMessage->getSdsTime().c_str());	\
	if("0001" != pSdsMessage->getSdsType())	\
	{	\
		(void)xml.AddElem("StatusCode");	\
		(void)xml.SetElemValue(eLTE_Tool::UInt2String(pSdsMessage->getStatusCode()).c_str());	\
	}	\
	xml.OutOfElem();	\

#define SET_GROUPINFO_XML() \
	(void)xml.AddChildElem("GroupNumber");	\
	(void)xml.IntoElem();	\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupInfo->GroupNumber).c_str());	\
	(void)xml.AddElem("SetupDcId");	\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupInfo->SetupDcId).c_str());	\
	(void)xml.AddElem("PGPriority");	\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupInfo->PGPriority).c_str());	\
	(void)xml.AddElem("DcPatchIndex");	\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupInfo->DcPatchIndex).c_str());	\
	(void)xml.AddElem("PGName");	\
	(void)xml.SetElemValue(pPatchGroupInfo->PGName.c_str());	\
	(void)xml.AddElem("VPNID");	\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupInfo->VPNID).c_str());	\
	xml.OutOfElem();	\

#define GET_XML_STR(ret) \
	unsigned int uiLen = 0;	\
	const char* pXmlStr = xml.GetXMLStream(uiLen);	\
	if (NULL == pXmlStr)	\
	{	\
		LOG_RUN_ERROR("pXmlStr is null.");	\
		return ret;	\
	}	\
	xmlStr = pXmlStr;	\

#define PARSE_GROUPLIST_AND_USERLIST() \
	uiMaxLen = XML_VAR_LENGTH;	\
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);		\
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Priority", srcValue, elemValue, uiMaxLen);	\
	param.priority = eLTE_Tool::String2Int(elemValue);	\
	uiMaxLen = XML_VAR_LENGTH;	\
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	\
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "MaxPeriod", srcValue, elemValue, uiMaxLen);	\
	param.maxPeriod = eLTE_Tool::String2Int(elemValue);	\
	if (!xmlParse.FindElem("GroupList"))	\
	{	\
		LOG_RUN_ERROR("FindElem GroupList failed.");	\
		return eLTE_SVC_ERR_XML_FIND_ELEM;	\
	}	\
	if (xmlParse.IntoElem())	\
	{	\
		int iIndex = 0;	\
		do	\
		{	\
			uiMaxLen = XML_VAR_LENGTH;	\
			eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	\
			srcValue = xmlParse.GetElemValue();	\
			if (NULL == srcValue)	\
			{	\
				LOG_RUN_ERROR("GetElemValue GroupID failed.");	\
				return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;	\
			}	\
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);	\
			eSDK_MEMCPY(elemValue, sizeof(elemValue), srcValue, uiMaxLen);	\
			param.grpList[iIndex++] = eLTE_Tool::String2Int(elemValue);	\
			if (iIndex >= 8)	\
			{	\
				LOG_RUN_ERROR("grpList is out of memory. the max index is 8.");	\
				break;	\
			}	\
		} while (xmlParse.NextElem());	\
		xmlParse.OutOfElem();	\
	}	\
	if (!xmlParse.FindElem("UserList"))	\
	{	\
		LOG_RUN_ERROR("FindElem UserList failed.");	\
		return eLTE_SVC_ERR_XML_FIND_ELEM;	\
	}	\
	if (xmlParse.IntoElem())	\
	{	\
		int iIndex = 0;	\
		do	\
		{	\
			uiMaxLen = XML_VAR_LENGTH;	\
			eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	\
			srcValue = xmlParse.GetElemValue();	\
			if (NULL == srcValue)	\
			{	\
				LOG_RUN_ERROR("GetElemValue UserID failed.");	\
				return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;	\
			}	\
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);	\
			eSDK_MEMCPY(elemValue, sizeof(elemValue), srcValue, uiMaxLen);	\
			param.userList[iIndex++] = eLTE_Tool::String2Int(elemValue);	\
			if (iIndex >= 200)	\
			{	\
				LOG_RUN_ERROR("grpList is out of memory. the max index is 200.");	\
				break;	\
			}	\
		} while (xmlParse.NextElem());	\
		xmlParse.OutOfElem();	\
	}	\

#define SET_SDS_XML_HEADER() \
	CXml xml;															\
	(void)xml.AddElem("Content");										\
	(void)xml.AddChildElem("SdsType");									\
	(void)xml.IntoElem();												\
	(void)xml.SetElemValue(pSdsMessage->getSdsType().c_str());			\
	if(!(pSdsMessage->getSdsContent().empty()))							\
	{																	\
		(void)xml.AddElem("SdsContent");								\
		(void)xml.SetElemValue(pSdsMessage->getSdsContent().c_str());	\
	}																	\

// XML节点解析
#define GET_XML_ELEM_VALUE_CHAR(xml, elem, srcValue, retValue, maxLen)			\
	if (!xml.FindElem(elem))													\
	{																			\
		LOG_RUN_ERROR("FindElem %s failed.", elem);								\
		return eLTE_SVC_ERR_XML_FIND_ELEM;											\
	}																			\
	srcValue = xml.GetElemValue();												\
	if (NULL == srcValue)														\
	{																			\
		LOG_RUN_ERROR("GetElemValue %s failed.", elem);							\
		return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;										\
	}																			\
	maxLen = (strlen(srcValue) < (maxLen)) ? strlen(srcValue) : ((maxLen)-1);	\
	eSDK_MEMCPY(retValue, sizeof(retValue), srcValue, maxLen)

#define PARSE_XML_DATA(xmlStr)	\
	LOG_TRACE();							\
if (NULL == xmlStr)							\
{											\
	LOG_RUN_ERROR("xml is null.");		\
	return eLTE_SVC_ERR_INVALID_PARAM;		\
}											\
CXml xmlParse;								\
if (!xmlParse.Parse(xmlStr))				\
{											\
	LOG_RUN_ERROR("parse xml failed.");		\
	return eLTE_SVC_ERR_XML_PARSE;			\
}											\
if (!xmlParse.FindElem("Content"))			\
{											\
	LOG_RUN_ERROR("FindElem Content failed.");	\
	return eLTE_SVC_ERR_XML_FIND_ELEM;			\
}												\
(void)xmlParse.IntoElem();						\

#define IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml) \
	if (!pInfo->empty())	\
	{						\
		xml.OutOfElem();	\
	}						\
	xml.OutOfElem();		\
	unsigned int uiLen = 0;	\
	const char* pXmlStr = xml.GetXMLStream(uiLen);	\
	if (NULL == pXmlStr)							\
	{												\
		LOG_RUN_ERROR("pXmlStr is null.");			\
		return eLTE_SVC_ERR_XML_CONSTRUCT;			\
	}												\

struct VIDEO_PARAM
{
	std::string VideoFormat;
	std::string CameraType;
	std::string UserConfirmType;
	std::string MuteType;
};

struct VWALL_PARAM
{
	MSISDN DstObjId;
	MSISDN SrcObjId;
	std::string StrFmt;
	std::string MuteType;
};

struct VIDEO_DISPATCH_PARAM
{
	std::string     fmtvalue;           
	int             dispatch_num;         
	std::list<int>  dstviewerlist;  
	int             channel;    
};

//////////////////////////////////////////////////////////////////////////
class XMLProcess
{
	// DisplayManager/GISReportManager 回调消息xml构造
public:
	static int SetXml_NotifyGroupStatus(GrpDspInfo* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyResourceStatus(ResourceStatusIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyP2pcallStatus(P2pcallStatusIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyP2pvideocallStatus(P2pvideocallStatusIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyUserStatus(ResourceStatusIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyModuleStatus(DCModuleStatusIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyProvisionChange(ProvChangeIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyGISReport(GISReportIndicator* const pInfo, std::string& xmlStr);
	static int SetXml_NotifyGISStatus(GISReportIndicator* const pInfo, std::string& xmlStr);

	static int SetXml_NotifySDSReport(SdsMessageIndicator* const pSdsMessage, std::string& xmlStr);
	static int SetXml_NotifySDSSendStatus(SdsMessageIndicator* const pSdsMessage, std::string& xmlStr);
	static int SetXml_NotifyUserSdsStatusReport(SdsMessageIndicator* const pSdsMessage, std::string& xmlStr);
	// 请求消息xml解析
public:
	static int GetXml_Connect_Req(const char* xmlStr, std::string& passwd);
	static int GetXml_Login_Req(const char* xmlStr, std::string& userId, std::string& pwd, std::string& serverIP, std::string& localIP, std::string& sipPort);
	static int GetXml_Logout_Req(const char* xmlStr, std::string& userId);
	static int GetXml_TriggerStatusReport_Req(const char* xmlStr, unsigned int& uiEnable);
	static int GetXml_ProvisionManagerInit_Req(const char* xmlStr, std::string& serverIP, int& userId);
	static int GetXml_ProvisionManagerInitMRS_Req(const char* xmlStr, std::string& serverIP);
	static int GetXml_GetDcGroups_Req(const char* xmlStr, int& userId);
	static int GetXml_GetDcUsers_Req(const char* xmlStr, int& userId);
	static int GetXml_GetGroupUsers_Req(const char* xmlStr, int& groupId);
	static int GetXml_GetGroupInfo_Req(const char* xmlStr, int& groupId);
	static int GetXml_GetUserInfo_Req(const char* xmlStr, int& userId);
	static int GetXml_GetDcInfo_Req(const char* xmlStr, int& userId);
	static int GetXml_SubscribeGroup_Req(const char* xmlStr, int& groupId, unsigned int& uiEnable);
	static int GetXml_GetUserRECFileInfoList_Req(const char* xmlStr, RECQueryInfo& query);
	static int GetXml_CreateDynamicGroup_Req(const char* xmlStr, DGNA_parameter& param);
	static int GetXml_CancelDynamicGroup_Req(const char* xmlStr, int& resId);
	static int GetXml_CreateTempGroup_Req(const char* xmlStr, DGNA_parameter& param);
	static int GetXml_StartRealPlay_Req(const char* xmlStr, int& resId, VIDEO_PARAM& param);
	static int GetXml_PTZControl_Req(const char* xmlStr, int& resId, FVG_control& param);
	static int GetXml_StopRealPlay_Req(const char* xmlStr, int& resId);
	static int GetXml_StartVideoDispatch_Req(const char* xmlStr, int& resId, VIDEO_DISPATCH_PARAM& param);
	static int GetXml_StopVideoDispatch_Req(const char* xmlStr, int& resId, Video_Dispatch_Delete& param);	
	static int Common_GetResourceID(const char* xmlStr, int& resId);
	static int Common_GetGroupID(const char* xmlStr, int& groupId);

	static int GetXml_VolControl_Req(const char* xmlStr, int& resId, int& callType);
	static int GetXml_GISSubscribe_Req(const std::string& xmlStr, int& subType, std::string& strResourceList, std::string& strSubscriber);
	static int GetXml_SDSSend_Req(const std::string& xmlStr, std::string& strResId, SDS_parameter& param);

	static int GetXml_RecvVideoPlay_Req(const char* xmlStr, int& resId);
	
	static int GetXml_StartVWall_Req(const char* xmlStr, int& resId, VWALL_PARAM& param);
	static int GetXml_VWallStop_Req(const char* xmlStr, int& resId, VWallStop_parameter& param);

// 	static int GetXml_TelephoneDial_Req(const char* xmlStr, std::string& PhoneNumStr);
// 	static int GetXml_TelephoneHangup_Req(const char* xmlStr, std::string& PhoneNumStr);
	
	static int GetXml_GetResourceID_Req(const char* xmlStr, std::string& ResourceID);
	static int GetXml_P2PTransfer_Req(const char* xmlStr, int& resId, transfer_parameter& param);
	static int GetXml_OperatePatchGroup_Req(const char* xmlStr, PCHGRP_Para& param);
	static int GetXml_ModifyDynamicGroup_Req(const char* xmlStr, DGNA_Modify_parameter& param);
//	static int GetXml_TempUserJoinGroup_Req(const char* xmlStr, int& resId, PhonePatch_parameter& param);
	static int GetXml_CancelPatchGroup_Req(const char* xmlStr, int& resId);
	static int GetXml_SetUserSpecificGISCfg_Req(const char* xmlStr, unsigned& resId, unsigned& reportPeriod, unsigned& reportDistance);
	// 答复消息xml构造
public:
	static int SetXml_GetDcGroups_Rsp(const DcGroups* pInfo, std::string& xmlStr);
	static int SetXml_GetDcUsers_Rsp(const DcUsers* pInfo, std::string& xmlStr);
	static int SetXml_GetGroupUsers_Rsp(const GrpUserList* pInfo, std::string& xmlStr);
	static int SetXml_GetGroupInfo_Rsp(const GroupInfo* pInfo, std::string& xmlStr);
	static int SetXml_GetUserInfo_Rsp(const UserInfo* pInfo, std::string& xmlStr);
	static int SetXml_GetDcInfo_Rsp(const DcProperty* pInfo, std::string& xmlStr);
	static int SetXml_GetUserRECFileInfoList_Rsp(const UserRecFileInfoList* pInfo, std::string& xmlStr);
	static int SetXml_GetTempGroupID_Rsp(const int groupId, std::string& xmlStr);
	static int SetXml_GetDcVWallIDList_Rsp(const VWallInfoList* pInfo, std::string& xmlStr);
	static int SetXml_GetPatchGroups_Rsp(const PatchGroupsList* pInfo, std::string& xmlStr);
	static int SetXml_GetGroupMemberByPatchId_Rsp(const PatchGroupMembers* pInfo, std::string& xmlStr);
	static int SetXml_GetPatchGroupInfo_Rsp(const PatchGroupInfo* pPatchGroupInfo, std::string& xmlStr);
	static int SetXml_GetUserSpecificGISCfg_Rsp(const ProvGisCfg* pProvGisCfg, std::string& xmlStr);
	static int SetXml_GetGisSubscription_Rsp(const GisQuerySubList* pInfo, std::string& xmlStr);

private:
	XMLProcess(void);
	~XMLProcess(void);

public:
	static std::string m_speakerID;
	static std::string m_transferNum;
};

#endif
