/*
Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
	   eSDK is licensed under the Apache License, Version 2.0 (the "License");
	   you may not use this file except in compliance with the License.
	   You may obtain a copy of the License at
	
	       http://www.apache.org/licenses/LICENSE-2.0

	
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

*/
/********************************************************************
filename    :    XMLProcess.h
author      :    zWX229156
created     :    2015/10/13
description :    XML入参解析，XML出参赋值
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/10/13
*********************************************************************/

#ifndef __ELTE_OCX_XML_PROCESS_H__
#define __ELTE_OCX_XML_PROCESS_H__

#include <string>
#include "eLTE_Types.h"
//Json definition class 
#include "json.h"

// XML节点解析
#define GET_XML_ELEM_VALUE_CHAR(xml, elem, srcValue, retValue, maxLen)			\
	if (!xml.FindElem(elem))													\
	{																			\
	LOG_RUN_ERROR("FindElem %s failed.", elem);								\
	return eLTE_SDK_ERR_XML_FIND_ELEM;											\
	}																			\
	srcValue = xml.GetElemValue();												\
	if (NULL == srcValue)														\
	{																			\
	LOG_RUN_ERROR("GetElemValue %s failed.", elem);							\
	return eLTE_SDK_ERR_XML_GET_ELEM_VALUE;										\
	}																			\
	maxLen = (strlen(srcValue) < (maxLen)) ? strlen(srcValue) : ((maxLen)-1);	\
	eSDK_MEMCPY(retValue, maxLen+1, srcValue, maxLen)

struct VIDEO_PARAM
{
	std::string VideoFormat;
	std::string CameraType;
	std::string UserConfirmType;
	std::string MuteType;
};

//////////////////////////////////////////////////////////////////////////
class XMLProcess
{
private:
	XMLProcess(void);
	~XMLProcess(void);

public:
	static ELTE_INT32 SetXml_NotifyResourceStatus(const std::string& strResourceID, const std::string& strResourceName, const ELTE_INT32& iStatusType, 
		const ELTE_INT32& iStatusValue, const std::string& strAttachingGroup, const std::string& strCause, std::string& xmlStr);
	
	static int SetXml_NotifyP2pvideocallStatus(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_NotifyUserStatus(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_NotifyModuleStatus(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_GetDcUsers_Rsp(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_GetDcGroups_Rsp(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_GetGroupUsers_Rsp(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_GetUserInfo_Rsp(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_GetGroupInfo_Rsp(const Json::Value& root, std::string& xmlStr);

	static ELTE_INT32 SetXml_GetDcInfo_Rsp(const Json::Value& root, std::string& xmlStr);

	static int GetXml_StartRealPlay_Req(const ELTE_CHAR* xmlStr, VIDEO_PARAM& param);
};

#endif