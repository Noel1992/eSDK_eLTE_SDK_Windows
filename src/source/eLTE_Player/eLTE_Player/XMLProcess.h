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
created     :    2014/07/01
description :    XML入参解析，XML出参赋值
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_XML_PROCESS_H__
#define __ELTE_OCX_XML_PROCESS_H__

#include "DataType.h"


// XML返回值赋值
#define GET_RETURN_CODE_XML(xml, iRet, strResult)								\
	(void)xml.AddElem("Content");												\
	(void)xml.AddChildElem("ResultCode");										\
	(void)xml.IntoElem();														\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(iRet).c_str());				\
	IVS_UINT32 uiLen = 0;														\
	strResult = eLTE_Tool::ANSIToUnicode(xml.GetXMLStream(uiLen)).c_str()

// XML获取版本
#define GET_VERSION_XML(xml, pVersion, strResult)								\
	(void)xml.AddElem("Content");												\
	(void)xml.AddChildElem("Version");											\
	(void)xml.IntoElem();														\
	(void)xml.SetElemValue(pVersion);											\
	IVS_UINT32 uiLen = 0;														\
	strResult = eLTE_Tool::ANSIToUnicode(xml.GetXMLStream(uiLen)).c_str()

// XML声请临时组号
#define GET_NEXT_TEMP_GROUPID_XML(xml, iGroupID, strResult)						\
	(void)xml.AddElem("Content");												\
	(void)xml.AddChildElem("NextTempGroupID");									\
	(void)xml.IntoElem();														\
	(void)xml.SetElemValue(eLTE_Tool::Int2String(iGroupID).c_str());			\
	IVS_UINT32 uiLen = 0;														\
	strResult = eLTE_Tool::ANSIToUnicode(xml.GetXMLStream(uiLen)).c_str()

// XML节点解析
#define GET_XML_ELEM_VALUE_CHAR(xml, elem, srcValue, retValue, maxLen)			\
	if (!xml.FindElem(elem))													\
	{																			\
		LOG_RUN_ERROR("FindElem %s failed.", elem);								\
		return eLTE_ERR_XML_FIND_ELEM;											\
	}																			\
	srcValue = xml.GetElemValue();												\
	if (NULL == srcValue)														\
	{																			\
		LOG_RUN_ERROR("GetElemValue %s failed.", elem);							\
		return eLTE_ERR_XML_GET_ELEM_VALUE;										\
	}																			\
	maxLen = (strlen(srcValue) < (maxLen)) ? strlen(srcValue) : ((maxLen)-1);	\
	memcpy(retValue, srcValue, maxLen)

// 检查OCX Load模式是否正确
#define CHECK_ELTE_OCX_LOAD_TYPE(type, xml, strResult)							\
	if (eLTE_LOAD_OPERATE_MGR != type)											\
	{																			\
		GET_RETURN_CODE_XML(xml, eLTE_ERR_LOAD_TYPE, strResult);				\
		return strResult.AllocSysString();										\
	}


//////////////////////////////////////////////////////////////////////////
class XMLProcess
{
public:
	// 解析入参Video_UL_parameter
	static eLTE_RET XmlParseVideoULParameter(const std::string& xmlStr,			// 入参
		std::string& strVideoFormat, std::string& strCameraType,				// 出参
		std::string& strUserConfirmType, std::string& strMuteType);				// 出参

	// 解析入参Mute_UL_parameter
	static eLTE_RET XmlParseMuteULParameter(const std::string& xmlStr,	// 入参
		std::string& strCallType);	            // 出参

	// 解析入参GISSubscribeParameter
	static eLTE_RET XmlParseGISSubscribeParameter(const std::string& xmlStr,	// 入参
		std::string& strSubType, std::string& strResourceList, std::string& strSubscriber);	            // 出参

	// 解析入参视频播放参数
	static eLTE_RET XmlParsePlayVideoParam(const std::string& xmlStrLocal, const std::string& xmlStrRemote,	// 入参
		std::string& strLocalIP, unsigned int& uiLocalVideoPort, unsigned int& uiLocalAudioPort,			// 出参
		std::string& strRemoteIP, unsigned int& uiRemoteVideoPort, unsigned int& uiRemoteAudioPort);		// 出参

	// 解析查询录音文件信息的查询条件
	static eLTE_RET XmlParseRECQueryInfo(const std::string& xmlStr,
		RECQueryInfo& queryInfo);

	// 解析创建动态或者临时群组参数
	static eLTE_RET XmlParseDGNAParam(const std::string& xmlStr, DGNA_parameter& param);

	//解析发短信/彩信参数
	static eLTE_RET XmlParseSDSParam(const std::string& xmlStr,SDS_parameter& param);

public:
	// 设置DcGroups XML出参
	static eLTE_RET SetXmlStringDcGroups(const DcGroups* pDcGroups,			// 入参
		CString& xmlStr);													// 出参

	// 设置DcUsers XML出参
	static eLTE_RET SetXmlStringDcUsers(const DcUsers* pDcUsers,			// 入参
		CString& xmlStr);													// 出参

	// 设置GrpUserList XML出参
	static eLTE_RET SetXmlStringGrpUserList(const GrpUserList* pGrpUserList,// 入参
		CString& xmlStr);													// 出参

	// 设置GroupInfo XML出参
	static eLTE_RET SetXmlStringGroupInfo(const GroupInfo* pGroupInfo,		// 入参
		CString& xmlStr);													// 出参

	// 设置UserInfo XML出参
	static eLTE_RET SetXmlStringUserInfo(const UserInfo* pUserInfo,			// 入参
		CString& xmlStr);													// 出参

	// 设置DcProperty XML出参
	static eLTE_RET SetXmlStringDcProperty(const DcProperty* pDcProperty,	// 入参
		CString& xmlStr);													// 出参

	// 设置UserRecFileInfoList XML出参
	static eLTE_RET SetXmlStringRecFileInfoList(const UserRecFileInfoList* pInfoList,
		CString& xmlStr);

	// Event事件XML出参
public:
	static eLTE_RET SetXmlStringEventNotifyUserStatus(const NotifyUserStatusInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyP2pvideocallStatus(const NotifyP2pvideocallStatusInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyResourceStatus(const NotifyResourceStatusInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyProvisionChange(const NotifyProvisionChangeInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyP2pcallStatus(const NotifyP2pcallStatusInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyGroupStatus(const NotifyGroupStatusInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyModuleStatus(const NotifyModuleStatusInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyGisReport(const GisReportInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifyGisStatus(const GisReportInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifySDSReport(const SdsMessageInfo* pInfo,
		CString& xmlStr);
	static eLTE_RET SetXmlStringEventNotifySDSStatus(const SdsMessageInfo* pInfo,
		CString& xmlStr);

	// PlayerEvent事件XML出参
public:
	static eLTE_RET SetXmlStringPlayerEvent(int iEventId, const PlayerEventParamInfo* pInfo,
		CString& xmlStr);

private:
	XMLProcess(void);
	~XMLProcess(void);
};

#endif
