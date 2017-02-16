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

#include "StdAfx.h"
#include "XMLProcess.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "eLTE_Xml.h"
#include "mq/provchange_indicator.h"


XMLProcess::XMLProcess(void)
{
}


XMLProcess::~XMLProcess(void)
{
}

// 解析入参Mute_UL_parameter
eLTE_RET XMLProcess::XmlParseMuteULParameter(const std::string& xmlStr,	// 入参
		std::string& strCallType)			// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<MuteParam>
			<CallType>"0"点呼，"1"组呼，"2"视频回传</CallType>
		</MuteParam>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM1(xmlStr);

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("MuteParam"))
	{
		LOG_RUN_ERROR("FindElem MuteParam failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CallType", srcValue, elemValue, uiMaxLen);
	strCallType = elemValue;

	return eLTE_ERR_SUCCESS;
}


// 解析入参Video_UL_parameter
eLTE_RET XMLProcess::XmlParseVideoULParameter(const std::string& xmlStr,	// 入参
		std::string& strVideoFormat, std::string& strCameraType,			// 出参
		std::string& strUserConfirmType, std::string& strMuteType)			// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<VideoParam>
			<VideoFormat>D1、CIF、QCIF、720P或1080P</VideoFormat>
			<CameraType>“0”表示前置，“1”表示后置。</CameraType>
			<UserConfirmType>“0”表示不需要用户确认，“1”表示需要用户确认。</UserConfirmType>
			<MuteType>“0”表示需要伴音；“1”表示不需要伴音</MuteType>
		</VideoParam>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM1(xmlStr);

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("VideoParam"))
	{
		LOG_RUN_ERROR("FindElem VideoParam failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "VideoFormat", srcValue, elemValue, uiMaxLen);
	strVideoFormat = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CameraType", srcValue, elemValue, uiMaxLen);
	strCameraType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserConfirmType", srcValue, elemValue, uiMaxLen);
	strUserConfirmType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "MuteType", srcValue, elemValue, uiMaxLen);
	strMuteType = elemValue;

	return eLTE_ERR_SUCCESS;
}

// 解析入参视频播放参数
eLTE_RET XMLProcess::XmlParsePlayVideoParam(const std::string& xmlStrLocal, const std::string& xmlStrRemote,	// 入参
		std::string& strLocalIP, unsigned int& uiLocalVideoPort, unsigned int& uiLocalAudioPort,				// 出参
		std::string& strRemoteIP, unsigned int& uiRemoteVideoPort, unsigned int& uiRemoteAudioPort)				// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<LocalMediaAddr>
			<LocalIP></LocalIP>
			<VideoPort></VideoPort>
			<AudioPort></AudioPort>
		</LocalMediaAddr>
	</Content>
	<Content>
		<RemoteMediaAddr>
			<RemoteIP></RemoteIP>
			<VideoPort></VideoPort>
			<AudioPort></AudioPort>
		</RemoteMediaAddr>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM2(xmlStrLocal, xmlStrRemote);

	// 解析本地视频参数
	CXml xmlLocal;
	if (!xmlLocal.Parse(xmlStrLocal.c_str()))
	{
		LOG_RUN_ERROR("parse xmlStrLocal failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlLocal.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlLocal.IntoElem();
	if (!xmlLocal.FindElem("LocalMediaAddr"))
	{
		LOG_RUN_ERROR("FindElem LocalMediaAddr failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlLocal.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlLocal, "LocalIP", srcValue, elemValue, uiMaxLen);
	strLocalIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlLocal, "VideoPort", srcValue, elemValue, uiMaxLen);
	uiLocalVideoPort = eLTE_Tool::String2UInt(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlLocal, "AudioPort", srcValue, elemValue, uiMaxLen);
	uiLocalAudioPort = eLTE_Tool::String2UInt(elemValue);

	// 解析远端视频参数
	CXml xmlRemote;
	if (!xmlRemote.Parse(xmlStrRemote.c_str()))
	{
		LOG_RUN_ERROR("parse xmlStrRemote failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlRemote.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlRemote.IntoElem();
	if (!xmlRemote.FindElem("RemoteMediaAddr"))
	{
		LOG_RUN_ERROR("FindElem RemoteMediaAddr failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlRemote.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlRemote, "RemoteIP", srcValue, elemValue, uiMaxLen);
	strRemoteIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlRemote, "VideoPort", srcValue, elemValue, uiMaxLen);
	uiRemoteVideoPort = eLTE_Tool::String2UInt(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlRemote, "AudioPort", srcValue, elemValue, uiMaxLen);
	uiRemoteAudioPort = eLTE_Tool::String2UInt(elemValue);

	return eLTE_ERR_SUCCESS;
}

// 解析查询录音文件信息的查询条件
eLTE_RET XMLProcess::XmlParseRECQueryInfo(const std::string& xmlStr,
	RECQueryInfo& queryInfo)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<RECQueryInfo>
			<CallType></CallType>
			<Caller></Caller>
			<Callee></Callee>
			<ResourceID></ResourceID>
			<StartSec></StartSec>
			<EndSec></EndSec>
		</RECQueryInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("RECQueryInfo"))
	{
		LOG_RUN_ERROR("FindElem RECQueryInfo failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CallType", srcValue, elemValue, uiMaxLen);
	int iType = eLTE_Tool::String2Int(elemValue);
	switch (iType)
	{
	case ENUM_REC_UNKNOW_CALL_TYPE:
		{
			queryInfo.call_type = ENUM_REC_UNKNOW_CALL_TYPE;
		}
		break;
	case ENUM_REC_P2P_AUDIO:
		{
			queryInfo.call_type = ENUM_REC_P2P_AUDIO;
		}
		break;
	case ENUM_REC_P2P_VIDEO:
		{
			queryInfo.call_type = ENUM_REC_P2P_VIDEO;
		}
		break;
	case ENUM_REC_VIDEO_UPLOAD:
		{
			queryInfo.call_type = ENUM_REC_VIDEO_UPLOAD;
		}
		break;
	case ENUM_REC_PTT:
		{
			queryInfo.call_type = ENUM_REC_PTT;
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid call_type. (%d)", iType);
			return eLTE_ERR_FAILED;
		}
	}

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Caller", srcValue, elemValue, uiMaxLen);
	queryInfo.caller = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Callee", srcValue, elemValue, uiMaxLen);
	queryInfo.callee = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	queryInfo.resourceid = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "StartSec", srcValue, elemValue, uiMaxLen);
	queryInfo.start_sec = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "EndSec", srcValue, elemValue, uiMaxLen);
	queryInfo.end_sec = elemValue;

	return eLTE_ERR_SUCCESS;
}

// 解析创建动态或者临时群组参数
eLTE_RET XMLProcess::XmlParseDGNAParam(const std::string& xmlStr, DGNA_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID></GroupID>			//临时组号
		<DcID></DcID>				//创建该临时组的DC用户号
		<Alias></Alias>				//该组别名，可以不填
		<Priority></Priority>		//不用填写
		<MaxPeriod></MaxPeriod>		//不用填写
		<GroupList>					//组成员列表 8个
			<GroupID></GroupID>
		</GroupList>
		<UserList>					//用户成员列表 200个
			<UserID></UserID>
		</UserList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		INFO_PARAM1(xmlStr);
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	param.grpId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DcID", srcValue, elemValue, uiMaxLen);
	param.dcId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Alias", srcValue, elemValue, uiMaxLen);
	param.alias = eLTE_Tool::ANSIToUTF8(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Priority", srcValue, elemValue, uiMaxLen);
	param.priority = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "MaxPeriod", srcValue, elemValue, uiMaxLen);
	param.maxPeriod = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("GroupList"))
	{
		LOG_RUN_ERROR("FindElem GroupList failed.");
		return eLTE_ERR_FAILED;
	}
	if (xmlParse.IntoElem())
	{
		int iIndex = 0;
		do 
		{
			uiMaxLen = XML_VAR_LENGTH;
			eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);

			srcValue = xmlParse.GetElemValue();
			if (NULL == srcValue)
			{
				LOG_RUN_ERROR("GetElemValue GroupID failed.");
				return eLTE_ERR_XML_GET_ELEM_VALUE;
			}
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);
			eSDK_MEMCPY(elemValue, uiMaxLen+1, srcValue, uiMaxLen);
			param.grpList[iIndex++] = eLTE_Tool::String2Int(elemValue);

			if (iIndex >= 8)
			{
				LOG_RUN_ERROR("grpList is out of memory. the max index is 8.");
				break;
			}
		} while (xmlParse.NextElem());
		xmlParse.OutOfElem();
	}

	if (!xmlParse.FindElem("UserList"))
	{
		LOG_RUN_ERROR("FindElem UserList failed.");
		return eLTE_ERR_FAILED;
	}
	if (xmlParse.IntoElem())
	{
		int iIndex = 0;
		do 
		{
			uiMaxLen = XML_VAR_LENGTH;
			eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);

			srcValue = xmlParse.GetElemValue();
			if (NULL == srcValue)
			{
				LOG_RUN_ERROR("GetElemValue UserID failed.");
				return eLTE_ERR_XML_GET_ELEM_VALUE;
			}
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);
			eSDK_MEMCPY(elemValue, uiMaxLen+1, srcValue, uiMaxLen);
			param.userList[iIndex++] = eLTE_Tool::String2Int(elemValue);

			if (iIndex >= 200)
			{
				LOG_RUN_ERROR("grpList is out of memory. the max index is 200.");
				break;
			}

		} while (xmlParse.NextElem());
		xmlParse.OutOfElem();
	}

	return eLTE_ERR_SUCCESS;
}

//解析发短信/彩信参数
eLTE_RET XMLProcess::XmlParseSDSParam(const std::string& xmlStr, SDS_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<SDSType></SDSType>			//SDS短数据类型
		<MsgBody></MsgBody>	        //SDS短数据内容
		<Receiver></Receiver>	    //收件人，格式为“MSISDN@broadcast.SERVER_IP”的字符串，比如“1001@broadcast.10.1.1.1”；从2.1版本开始可以只填用户号，例如“1001”
		<AttachFileList>			//附件地址列表
			<AttachFile></AttachFile>
			<AttachFile></AttachFile>
		</AttachFileList>
		<MsgId></MsgId>
	</Content>
	************************************************************************/

	LOG_TRACE();

	std::string xmlDst;
	std::string::size_type iStart = xmlStr.find("<MsgBody>");
	if(std::string::npos != iStart)
	{
		std::string::size_type iEnd = xmlStr.find("</MsgBody>", iStart);
		if(std::string::npos != iEnd)
		{
			std::string strTemp;
			iStart += strlen("<MsgBody>");
			eLTE_Tool::ReplaceString(xmlStr.substr(iStart, iEnd - iStart), strTemp, "&", "&amp;");
			eLTE_Tool::ReplaceString(strTemp, strTemp, "<", "&lt;");
			xmlDst = xmlStr.substr(0, iStart) + strTemp + xmlStr.substr(iEnd, xmlStr.size() - iEnd);
		}
	}

	CXml xmlParse;
	if (!xmlParse.Parse(xmlDst.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		INFO_PARAM1(xmlStr);
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "SDSType", srcValue, elemValue, uiMaxLen);
	//param.sdstype = eLTE_Tool::String2Int(elemValue);
	param.sdstype = elemValue;

	if (xmlParse.FindElem("MsgBody"))													
	{																			
		srcValue = xmlParse.GetElemValue();
		if(NULL != srcValue)
		{
			param.msg_body = srcValue;
		}
	}																			

	if (!xmlParse.FindElem("Receiver"))													
	{																			
		LOG_RUN_ERROR("FindElem Receiver failed.");								
		return eLTE_ERR_XML_FIND_ELEM;											
	}																			
	srcValue = xmlParse.GetElemValue();												
	if (NULL == srcValue)														
	{																			
		LOG_RUN_ERROR("GetElemValue Receiver failed.");							
		return eLTE_ERR_XML_GET_ELEM_VALUE;										
	}			
	param.receiver = srcValue;

	//彩信
	if (EXMPP_MSG_TYPE_MMS == param.sdstype && xmlParse.FindElem("AttachFileList") && xmlParse.IntoElem())
	{
		std::list<std::string>::iterator it = param.files.begin();

		do 
		{																		
			srcValue = xmlParse.GetElemValue();
			if(NULL != srcValue)
			{
				param.files.push_back(srcValue);
			}
		} while (xmlParse.NextElem());
		xmlParse.OutOfElem();
	}

	if (xmlParse.FindElem("MsgId"))													
	{																			
		srcValue = xmlParse.GetElemValue();
		if(NULL != srcValue)
		{
			param.msgid = srcValue;
		}
	}

	return eLTE_ERR_SUCCESS;
}


// 设置DcGroups XML出参
eLTE_RET XMLProcess::SetXmlStringDcGroups(const DcGroups* pDcGroups,			// 入参
	CString& xmlStr)															// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<GroupInfoList>
			<GroupInfo>
				<GroupID></GroupID>
				<GroupCategory></GroupCategory>
				<GroupPriority></GroupPriority>
				<GroupName></GroupName>
			</GroupInfo>
		</GroupInfoList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pDcGroups)
	{
		LOG_RUN_ERROR("pDcGroups is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("GroupInfoList");

	DcGroups::const_iterator it_b = pDcGroups->begin();
	DcGroups::const_iterator it_e = pDcGroups->end();
	for (DcGroups::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("GroupInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("GroupInfo");
		}

		GroupInfo* const &pGroupInfo = *itor;

		(void)xml.AddChildElem("GroupID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grpid).c_str());
		(void)xml.AddElem("GroupCategory");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grpcategory).c_str());
		(void)xml.AddElem("GroupPriority");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grppriority).c_str());
		(void)xml.AddElem("GroupName");
		(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pGroupInfo->grpname.c_str()).c_str());

		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

// 设置DcUsers XML出参
eLTE_RET XMLProcess::SetXmlStringDcUsers(const DcUsers* pDcUsers,				// 入参
	CString& xmlStr)															// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<UserInfoList>
			<UserInfo>
				<UserID></UserID>
				<UserCategory></UserCategory>
				<UserPriority></UserPriority>
				<UserName></UserName>
			</UserInfo>
		</UserInfoList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pDcUsers)
	{
		LOG_RUN_ERROR("pDcUsers is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("UserInfoList");

	DcUsers::const_iterator it_b = pDcUsers->begin();
	DcUsers::const_iterator it_e = pDcUsers->end();
	for (DcUsers::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("UserInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("UserInfo");
		}

		UserInfo* const &pUserInfo = *itor;

		(void)xml.AddChildElem("UserID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->userid).c_str());
		(void)xml.AddElem("UserCategory");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->usercategory).c_str());
		(void)xml.AddElem("UserPriority");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->userpriority).c_str());
		(void)xml.AddElem("UserName");
		(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pUserInfo->username.c_str()).c_str());

		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

// 设置GrpUserList XML出参
eLTE_RET XMLProcess::SetXmlStringGrpUserList(const GrpUserList* pGrpUserList,	// 入参
	CString& xmlStr)															// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<GroupUserInfoList>
			<GroupUserInfo>
				<UserID></UserID>
				<GroupID></GroupID>
				<UserPriorityInGroup></UserPriorityInGroup>
				<MemberType></MemberType>
			</GroupUserInfo>
		</GroupUserInfoList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pGrpUserList)
	{
		LOG_RUN_ERROR("pGrpUserList is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("GroupUserInfoList");

	GrpUserList::const_iterator it_b = pGrpUserList->begin();
	GrpUserList::const_iterator it_e = pGrpUserList->end();
	for (GrpUserList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("GroupUserInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("GroupUserInfo");
		}

		GrpUserInfo* const &grpUserInfo = *itor;

		(void)xml.AddChildElem("UserID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(grpUserInfo->userid).c_str());
		(void)xml.AddElem("GroupID");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(grpUserInfo->grpid).c_str());
		(void)xml.AddElem("UserPriorityInGroup");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(grpUserInfo->userpriorityInGrp).c_str());
		(void)xml.AddElem("MemberType");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(grpUserInfo->memberType).c_str());

		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

// 设置GroupInfo XML出参
eLTE_RET XMLProcess::SetXmlStringGroupInfo(const GroupInfo* pGroupInfo,			// 入参
	CString& xmlStr)															// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<GroupInfo>
			<GroupID></GroupID>
			<GroupCategory></GroupCategory>
			<GroupPriority></GroupPriority>
			<GroupName></GroupName>
		</GroupInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pGroupInfo)
	{
		LOG_RUN_ERROR("pGroupInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("GroupInfo");
	(void)xml.AddChildElem("GroupID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grpid).c_str());
	(void)xml.AddElem("GroupCategory");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grpcategory).c_str());
	(void)xml.AddElem("GroupPriority");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grppriority).c_str());
	(void)xml.AddElem("GroupName");
	(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pGroupInfo->grpname.c_str()).c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

// 设置UserInfo XML出参
eLTE_RET XMLProcess::SetXmlStringUserInfo(const UserInfo* pUserInfo,			// 入参
	CString& xmlStr)															// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<UserInfo>
			<UserID></UserID>
			<UserCategory></UserCategory>
			<UserPriority></UserPriority>
			<UserName></UserName>
		</UserInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pUserInfo)
	{
		LOG_RUN_ERROR("pUserInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("UserInfo");
	(void)xml.AddChildElem("UserID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->userid).c_str());
	(void)xml.AddElem("UserCategory");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->usercategory).c_str());
	(void)xml.AddElem("UserPriority");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->userpriority).c_str());
	(void)xml.AddElem("UserName");
	(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pUserInfo->username.c_str()).c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

// 设置DcProperty XML出参
eLTE_RET XMLProcess::SetXmlStringDcProperty(const DcProperty* pDcProperty,		// 入参
	CString& xmlStr)															// 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<DcInfo>
			<DcID></DcID>
			<Priviledge></Priviledge>
			<Role></Role>
			<Alias></Alias>
		</DcInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pDcProperty)
	{
		LOG_RUN_ERROR("pDcProperty is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("DcInfo");
	(void)xml.AddChildElem("DcID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pDcProperty->dcid).c_str());
	(void)xml.AddElem("Priviledge");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pDcProperty->priviledge).c_str());
	(void)xml.AddElem("Role");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pDcProperty->role).c_str());
	(void)xml.AddElem("Alias");
	(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pDcProperty->Alias.c_str()).c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

// 设置UserRecFileInfoList XML出参
eLTE_RET XMLProcess::SetXmlStringRecFileInfoList(const UserRecFileInfoList* pInfoList,
	CString& xmlStr)	
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<RECFileInfoList>
			<RECFileInfo>
				<CallType></CallType>
				<Caller></Caller>
				<Callee></Callee>
				<ResourceID></ResourceID>
				<StartSec></StartSec>
				<EndSec></EndSec>
				<UrlFTP></UrlFTP>
				<UrlRTSP></UrlRTSP>
			</RECFileInfo>
		<RECFileInfoList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfoList)
	{
		LOG_RUN_ERROR("pInfoList is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(eLTE_ERR_SUCCESS).c_str());
	(void)xml.AddElem("RECFileInfoList");

	UserRecFileInfoList::const_iterator it_b = pInfoList->begin();
	UserRecFileInfoList::const_iterator it_e = pInfoList->end();
	for (UserRecFileInfoList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("RECFileInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("RECFileInfo");
		}

		UserRecFileInfo* const &recFileInfo = *itor;

		(void)xml.AddChildElem("CallType");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(recFileInfo->call_type).c_str());
		(void)xml.AddElem("Caller");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(recFileInfo->caller).c_str());
		(void)xml.AddElem("Callee");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(recFileInfo->callee).c_str());
		(void)xml.AddElem("ResourceID");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(recFileInfo->resourceid).c_str());
		(void)xml.AddElem("StartSec");
		(void)xml.SetElemValue(recFileInfo->start_sec.c_str());
		(void)xml.AddElem("EndSec");
		(void)xml.SetElemValue(recFileInfo->end_sec.c_str());
		(void)xml.AddElem("UrlFTP");
		(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(recFileInfo->urlFTP.c_str()).c_str());
		(void)xml.AddElem("UrlRTSP");
		(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(recFileInfo->urlRTSP.c_str()).c_str());

		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyUserStatus(const NotifyUserStatusInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID></UserID>
		<StatusType>USERSTATUS</StatusType>
		<StatusValue></StatusValue>
		<PeerID></PeerID>
		<Direction></Direction>
		<CallType></CallType>
	</Content>
	************************************************************************/
	
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}
	
	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iUserID).c_str());
	(void)xml.AddElem("StatusType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iStatusType).c_str());
	(void)xml.AddElem("StatusValue");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iStatusValue).c_str());
	(void)xml.AddElem("PeerID");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iPeerID).c_str());
	(void)xml.AddElem("Direction");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iDirection).c_str());
	(void)xml.AddElem("CallType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCallType).c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyP2pvideocallStatus(const NotifyP2pvideocallStatusInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<CallStatus></CallStatus>
		<LocalAudioPort></LocalAudioPort>
		<LocalVideoPort></LocalVideoPort>
		<RemoteAudioPort></RemoteAudioPort>
		<RemoteVideoPort></RemoteVideoPort>
		<RemoteIp></RemoteIp>
		<Callee></Callee>
		<Caller></Caller>
		<SoundMute></SoundMute>
		<Channel></Channel>
		<UserConfirm></UserConfirm>
		<Camera></Camera>
		<SoundPtype></SoundPtype>
		<VideoFormatType></VideoFormatType>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("CallStatus");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCallStatus).c_str());
	(void)xml.AddElem("LocalAudioPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iLocalAudioPort).c_str());
	(void)xml.AddElem("LocalVideoPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iLocalVideoPort).c_str());
	(void)xml.AddElem("RemoteAudioPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iRemoteAudioPort).c_str());
	(void)xml.AddElem("RemoteVideoPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iRemoteVideoPort).c_str());
	(void)xml.AddElem("RemoteIp");
	(void)xml.SetElemValue(pInfo->strRemoteIp.c_str());
	(void)xml.AddElem("Callee");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCallee).c_str());
	(void)xml.AddElem("Caller");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCaller).c_str());
	(void)xml.AddElem("SoundMute");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iSoundMute).c_str());
	(void)xml.AddElem("Channel");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iChannel).c_str());
	(void)xml.AddElem("UserConfirm");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iUserConfirm).c_str());
	(void)xml.AddElem("Camera");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCamera).c_str());
	(void)xml.AddElem("SoundPtype");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iSoundPtype).c_str());
	(void)xml.AddElem("VideoFormatType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iVideoFormatType).c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyResourceStatus(const NotifyResourceStatusInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID></ResourceID>
		<StatusType></StatusType>
		<StatusValue></StatusValue>
		<AttachingGroup></AttachingGroup>
		<Cause></Cause> //当type为USERDGNASTATUS时才有
	</Content>
	************************************************************************/
	
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iResourceID).c_str());
	(void)xml.AddElem("StatusType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iStatusType).c_str());
	(void)xml.AddElem("StatusValue");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iStatusValue).c_str());
	(void)xml.AddElem("AttachingGroup");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iAttachingGroup).c_str());
	// 动态组添加cause字段
	if (USERDGNASTATUS == pInfo->iStatusType)
	{
		(void)xml.AddElem("Cause");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCause).c_str());
	}
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyProvisionChange(const NotifyProvisionChangeInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ChangeType></ChangeType>
		<ChangeMO></ChangeMO>
		<xxxInfo>
		....
		</xxxInfo>
	</Content>
	************************************************************************/
	
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ChangeType");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iChangeType).c_str());
	(void)xml.AddElem("ChangeMO");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iChangeMO).c_str());

	INFO_PARAM1(pInfo->iChangeMO);
	switch (pInfo->iChangeMO)
	{
	case MO_PTTUser:
		{
			MO_PTTUser_Info* pIf = (MO_PTTUser_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("PTTUserInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserID).c_str());
			(void)xml.AddElem("UserCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserCategory).c_str());
			(void)xml.AddElem("UserPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserPriority).c_str());
			(void)xml.AddElem("UserName");
			(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pIf->strUserName.c_str()).c_str());
			(void)xml.AddElem("VpnID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnID).c_str());
			(void)xml.AddElem("VpnOut");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnOut).c_str());
			(void)xml.AddElem("VpnIn");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnIn).c_str());
			(void)xml.AddElem("Location");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iLocation).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_FixedUser:
		{
			MO_FixedUser_Info* pIf = (MO_FixedUser_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("FixedUserInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserID).c_str());
			(void)xml.AddElem("UserCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserCategory).c_str());
			(void)xml.AddElem("UserPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserPriority).c_str());
			(void)xml.AddElem("UserName");
			(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pIf->strUserName.c_str()).c_str());
			(void)xml.AddElem("VpnID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnID).c_str());
			(void)xml.AddElem("VpnOut");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnOut).c_str());
			(void)xml.AddElem("VpnIn");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnIn).c_str());
			(void)xml.AddElem("Location");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iLocation).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_DCAttribute:
		{
			MO_DCAttribute_Info* pIf = (MO_DCAttribute_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("DCAttributeInfo");
			(void)xml.AddChildElem("DcID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iDcID).c_str());
			(void)xml.AddElem("Priviledge");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iPriviledge).c_str());
			(void)xml.AddElem("Role");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iRole).c_str());
			(void)xml.AddElem("Alias");
			(void)xml.SetElemValue(pIf->strAlias.c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_Group:
		{
			MO_Group_Info* pIf = (MO_Group_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("GroupInfo");
			(void)xml.AddChildElem("GroupID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iGroupID).c_str());
			(void)xml.AddElem("GroupCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iGroupCategory).c_str());
			(void)xml.AddElem("GroupPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iGroupPriority).c_str());
			(void)xml.AddElem("GroupName");
			(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(pIf->strGroupName.c_str()).c_str());
			(void)xml.AddElem("GroupState");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iGroupState).c_str());
			(void)xml.AddElem("VpnID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iVpnID).c_str());
			(void)xml.AddElem("SetupDcID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iSetupDcID).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_GroupPTTUser:
	case MO_GroupFixedUser:
		{
			MO_GroupUser_Info* pIf = (MO_GroupUser_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("GroupUserInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserID).c_str());
			(void)xml.AddElem("GroupID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iGroupID).c_str());
			(void)xml.AddElem("UserPriorityInGroup");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserPriorityInGroup).c_str());
			(void)xml.AddElem("MemberType");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iMemberType).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_RangeList:
		{
			MO_RangeList_Info* pIf = (MO_RangeList_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("RangeListInfo");
			(void)xml.AddChildElem("RangeID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iRangeID).c_str());
			(void)xml.AddElem("RangeCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iRangeCategory).c_str());
			(void)xml.AddElem("From");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iFrom).c_str());
			(void)xml.AddElem("To");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iTo).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_DCRanges:
		{
			MO_DCRanges_Info* pIf = (MO_DCRanges_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("DCRangesInfo");
			(void)xml.AddChildElem("RangeID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iRangeID).c_str());
			(void)xml.AddElem("DcID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iDcID).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_HOTLINECFG:
		{
			MO_HotlineCfg_Info* pIf = (MO_HotlineCfg_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("HotlineCfgInfo");
			(void)xml.AddChildElem("DcID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iDcID).c_str());
			(void)xml.AddElem("HotIndex");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iHotIndex).c_str());
			(void)xml.AddElem("Number");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iNumber).c_str());
			(void)xml.AddElem("Alias");
			(void)xml.SetElemValue(pIf->strAlias.c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_STMSGCFG:
		{
			MO_StMsgCfg_Info* pIf = (MO_StMsgCfg_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("StMsgCfgInfo");
			(void)xml.AddChildElem("ID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iID).c_str());
			(void)xml.AddElem("StMsg");
			(void)xml.SetElemValue(pIf->strStMsg.c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	//case MO_DECUSER:
	//	{
	//		// 没有找到对应类
	//	}
	//	break;
	case MO_GISCFG:
		{
			MO_GisCfg_Info* pIf = (MO_GisCfg_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("GisCfgInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserID).c_str());
			(void)xml.AddElem("ReportSwitch");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pIf->uiReportSwitch).c_str());
			(void)xml.AddElem("ReportPeriod");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pIf->uiReportPeriod).c_str());
			(void)xml.AddElem("ReportCapability");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pIf->uiReportCapability).c_str());
			(void)xml.AddElem("ReportDistance");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pIf->uiReportDistance).c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	case MO_GISIPC:
		{
			MO_GisIpc_Info* pIf = (MO_GisIpc_Info*)pInfo->pInfo;
			if (NULL == pIf)
			{
				LOG_RUN_ERROR("pIf is null.");
				return eLTE_ERR_FAILED;
			}

			(void)xml.AddElem("GisIpcInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pIf->iUserID).c_str());
			(void)xml.AddElem("Latitude");
			(void)xml.SetElemValue(eLTE_Tool::Double2String(pIf->dLatitude).c_str());
			(void)xml.AddElem("Longitude");
			(void)xml.SetElemValue(eLTE_Tool::Double2String(pIf->dLongitude).c_str());
			(void)xml.AddElem("Altitude");
			(void)xml.SetElemValue(eLTE_Tool::Double2String(pIf->dAltitude).c_str());
			(void)xml.AddElem("Address");
			(void)xml.SetElemValue(pIf->strAddress.c_str());
			xml.OutOfElem();

			delete pIf;
		}
		break;
	default:
		{
			// 其他不做事件回调
			// 没有找到对应类
			LOG_RUN_ERROR("Invalid ChangeMO.");
			return eLTE_ERR_FAILED;
		}
	}

	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyP2pcallStatus(const NotifyP2pcallStatusInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<P2pcallStatus></P2pcallStatus>
		<Caller></Caller>
		<Callee></Callee>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("P2pcallStatus");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCallStatus).c_str());
	(void)xml.AddElem("Caller");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCaller).c_str());
	(void)xml.AddElem("Callee");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCallee).c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyGroupStatus(const NotifyGroupStatusInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID></GroupID>
		<GroupCallStatus></GroupCallStatus>
		<Speaker></Speaker>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iGroupID).c_str());
	(void)xml.AddElem("GroupCallStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iGroupCallStatus).c_str());
	(void)xml.AddElem("Speaker");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iSpeaker).c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyModuleStatus(const NotifyModuleStatusInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID></ResourceID>
		<ModuleType></ModuleType>
		<ModuleStatus></ModuleStatus>
		<CallBackMsgType></CallBackMsgType>
		<ModulePara></ModulePara>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iResourceID).c_str());
	(void)xml.AddElem("ModuleType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iModuleType).c_str());
	(void)xml.AddElem("ModuleStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iModuleStatus).c_str());
	(void)xml.AddElem("CallBackMsgType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iCallBackMsgType).c_str());
	(void)xml.AddElem("ModulePara");
	(void)xml.SetElemValue(pInfo->strModulePara.c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringPlayerEvent(int iEventId, const PlayerEventParamInfo* pInfo,
	CString& xmlStr)
{
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;

	switch (iEventId)
	{
	case eLTE_PLAYER_REVERSE:		// OCX 播放器切换镜头
		{
			/************************************************************************
			--- XML 格式 ---
			<Content>
				<VideoFormat></VideoFormat>
				<CameraType></CameraType>
				<UserConfirmType></UserConfirmType>
				<MuteType></MuteType>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("VideoFormat");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(pInfo->strFmtValue.c_str());
			(void)xml.AddElem("CameraType");
			(void)xml.SetElemValue(pInfo->strCameraType.c_str());
			(void)xml.AddElem("UserConfirmType");
			(void)xml.SetElemValue(pInfo->strUserCfmType.c_str());
			(void)xml.AddElem("MuteType");
			(void)xml.SetElemValue(pInfo->strMuteType.c_str());
			xml.OutOfElem();
		}
		break;
	case eLTE_PLAYER_SNAPSHOT:		// OCX 播放器抓图
		{
			/************************************************************************
			--- XML 格式 ---
			<Content>
				<SnapshotPath></SnapshotPath>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("SnapshotPath");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(pInfo->strSnapshotPath.c_str());
			xml.OutOfElem();
		}
		break;
	case eLTE_PLAYER_POSITION:		// OCX 播放器坐标变动
		{
			/************************************************************************
			--- XML 格式 ---
			<Content>
				<Left></Left>
				<Top></Top>
				<Width></Width>
				<Height></Height>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("Left");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iLeft).c_str());
			(void)xml.AddElem("Top");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iTop).c_str());
			(void)xml.AddElem("Width");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iWidth).c_str());
			(void)xml.AddElem("Height");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iHeight).c_str());
			xml.OutOfElem();
		}
		break;
	case eLTE_PLAYER_TITLE:			// OCX 播放器窗口标题
		{
			/************************************************************************
			--- XML 格式 ---
			<Content>
				<TitleText></TitleText>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("TitleText");
			(void)xml.IntoElem();
			(void)xml.SetElemValue((pInfo->strTitle).c_str());
			xml.OutOfElem();
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid eventId.");
			return eLTE_ERR_FAILED;
		}
	}

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	//INFO_PARAM1(xmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyGisReport(const GisReportInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<ResourceID>上报方</ResourceID>
			<Time>最后一次上报时间</Time>
			<Altitude>高度</Altitude>
			<Latitude>纬度</Latitude>
			<Longtitude>经度</Longtitude>
			<TriggerCode>周期上报(0)，特定事件（1、紧急呼叫）</TriggerCode>
			<ReportStatus>GPS正常开启(0)，GPS搜星失败(1)或其它异常(2)</ReportStatus>
			</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strSender.c_str());
	(void)xml.AddElem("Time");
	(void)xml.SetElemValue(pInfo->strTime.c_str());
	(void)xml.AddElem("Altitude");
	(void)xml.SetElemValue(pInfo->strAltitude.c_str());
	(void)xml.AddElem("Latitude");
	(void)xml.SetElemValue(pInfo->strLatitude.c_str());
	(void)xml.AddElem("Longtitude");
	(void)xml.SetElemValue(pInfo->strLongtitude.c_str());
	(void)xml.AddElem("TriggerCode");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iTriggerCode).c_str());
	(void)xml.AddElem("ReportStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iReportState).c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifySDSStatus(const SdsMessageInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<SdsMsgId>消息ID</SdsMsgId>
			<SdsDirection>true，表示为发送状态的通知</SdsDirection>
			<SdsRetCode>消息发送状态</SdsRetCode>
			</Content>
	************************************************************************/
	LOG_TRACE();
	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("SdsMsgId");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strSubject.c_str());

	(void)xml.AddElem("SdsDirection");
	(void)xml.SetElemValue("true");

	(void)xml.AddElem("SdsRetCode");
	(void)xml.SetElemValue(pInfo->strRetCode.c_str());

	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifySDSReport(const SdsMessageInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<SdsType>消息类型</SdsType>
			<SdsContent>消息内容</SdsContent>
			<SdsFrom>发件人</SdsFrom>
			<SdsMmsFileNameList>
				<SdsMmsFileName>附件文件名列表</SdsMmsFileName>
				<SdsMmsFileName>附件文件名列表</SdsMmsFileName>
			</SdsMmsFileNameList>
			<SdsSubject>消息主题</SdsSubject>
			<SdsDirection>=false，表示接收到短信</SdsDirection>
			<SdsDate>消息发送日期</SdsDate>
			<SdsTime>消息发送时间</SdsTime>
			<StatusCode>状态短信的状态码（普通短信不适用）</StatusCode>
			</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("SdsType");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strType.c_str());

	if(!(pInfo->strContent.empty()))
	{
		(void)xml.AddElem("SdsContent");
		(void)xml.SetElemValue(pInfo->strContent.c_str());
	}

	(void)xml.AddElem("SdsFrom");
	(void)xml.SetElemValue(pInfo->strFrom.c_str());

	if(EXMPP_MSG_TYPE_MMS == pInfo->strType)
	{
		(void)xml.AddElem("SdsMmsFileNameList");

		std::list<std::string>::iterator it = const_cast<SdsMessageInfo*>(pInfo)->listMmsFileName.begin();
		//std::list<std::string> RecvfileList;
		for (; it != pInfo->listMmsFileName.end(); ++it)
		{
			if(it == pInfo->listMmsFileName.begin())
			{
				(void)xml.AddChildElem("SdsMmsFileName");
				(void)xml.IntoElem();
			}
			else
			{
				(void)xml.AddElem("SdsMmsFileName");
			}
			(void)xml.SetElemValue(eLTE_Tool::UTF8ToANSI(*it).c_str());
		}
		xml.OutOfElem();
	}

	(void)xml.AddElem("SdsSubject");
	(void)xml.SetElemValue(pInfo->strSubject.c_str());

	(void)xml.AddElem("SdsDirection");
	(void)xml.SetElemValue("false");

	(void)xml.AddElem("SdsDate");
	(void)xml.SetElemValue(pInfo->strDate.c_str());

	(void)xml.AddElem("SdsTime");
	(void)xml.SetElemValue(pInfo->strTime.c_str());

	if(!(pInfo->strStatusCode.empty()))
	{
		(void)xml.AddElem("StatusCode");
		(void)xml.SetElemValue(pInfo->strStatusCode.c_str());
	}

	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyGisStatus(const GisReportInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<ResourceID>上报方</ResourceID>
			<AckStatusList>
			<AckStatus>终端号：订阅状态，举例如下：
			1001:0。</AckStatus>
			</AckStatusList>
			</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strSender.c_str());
	(void)xml.AddElem("AckStatusList");

	AckStateMap::iterator itor_b = (const_cast<GisReportInfo*>(pInfo))->ackStateMap.begin();
	AckStateMap::iterator itor_e = (const_cast<GisReportInfo*>(pInfo))->ackStateMap.end();
	std::string strAckStatus;
	for(; itor_b != itor_e; ++itor_b)
	{
		if(itor_b == pInfo->ackStateMap.begin())
		{
			(void)xml.AddChildElem("AckStatus");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("AckStatus");
		}
		strAckStatus.clear();
		strAckStatus = "";
		strAckStatus.append(eLTE_Tool::Int2String(itor_b->first).c_str());
		strAckStatus.append(":");
		strAckStatus.append(eLTE_Tool::Int2String(itor_b->second).c_str());
		(void)xml.SetElemValue(strAckStatus.c_str());
	}
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::XmlParseGISSubscribeParameter(const std::string& xmlStr,     // 入参
	std::string& strSubType, std::string& strResourceList, std::string& strSubscriber)   // 出参
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GISParam>
			<SubType>7/8</SubType>
			<ResourceList>多个资源ID用分号分隔，最多200。例如1001;1002;1003</ResourceList>
			<Subscriber>目前可不填</Subscriber>
		</GISParam>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM1(xmlStr);

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("GISParam"))
	{
		LOG_RUN_ERROR("FindElem GISParam failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "SubType", srcValue, elemValue, uiMaxLen);
	strSubType = elemValue;

	if (!xmlParse.FindElem("ResourceList"))													
	{																			
		LOG_RUN_ERROR("FindElem ResourceList failed.");								
		return eLTE_ERR_XML_FIND_ELEM;											
	}																			
	srcValue = xmlParse.GetElemValue();												
	if (NULL == srcValue)														
	{																			
		LOG_RUN_ERROR("GetElemValue ResourceList failed.");							
		return eLTE_ERR_XML_GET_ELEM_VALUE;										
	}
	strResourceList = srcValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Subscriber", srcValue, elemValue, uiMaxLen);
	strSubscriber = elemValue;

	return eLTE_ERR_SUCCESS;
}
