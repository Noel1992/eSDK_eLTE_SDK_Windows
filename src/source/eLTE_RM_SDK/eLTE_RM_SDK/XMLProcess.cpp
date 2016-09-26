
#include "stdafx.h"
#include "XMLProcess.h"
//log manage class 
#include "eLTE_Log.h"
//Error Code define class
#include "eLTE_Error.h"
//public function interface class 
#include "eLTE_Tool.h"
#include "eLTE_Xml.h"

XMLProcess::XMLProcess(void)
{
}


XMLProcess::~XMLProcess(void)
{
}

ELTE_INT32 XMLProcess::SetXml_NotifyResourceStatus(const std::string& strResourceID, const std::string& strResourceName, const ELTE_INT32& iStatusType, 
	const ELTE_INT32& iStatusValue, const std::string& strAttachingGroup, const std::string& strCause, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID></ResourceID>
		<ResourceName></ResourceName>
		<StatusType></StatusType>
		<StatusValue></StatusValue>
		<AttachingGroup></AttachingGroup>
		<Cause></Cause> //当type为USERDGNASTATUS时才有
	</Content>
	************************************************************************/

	LOG_TRACE();

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strResourceID.c_str());
	(void)xml.AddElem("ResourceName");
	(void)xml.SetElemValue(strResourceName.c_str());
	(void)xml.AddElem("StatusType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(iStatusType).c_str());

	// 安全红线
	int iValue = (STATUS_PASSWORD_WRONG == iStatusValue) ? STATUS_NOT_FOUND : iStatusValue;

	(void)xml.AddElem("StatusValue");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(iValue).c_str());
	(void)xml.AddElem("AttachingGroup");
	(void)xml.SetElemValue(strAttachingGroup.c_str());


	// 动态组添加cause字段
	if (!strCause.empty())
	{
		(void)xml.AddElem("Cause");
		(void)xml.SetElemValue(strCause.c_str());
	}
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_GetDcUsers_Rsp(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
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
	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserInfoList");
	(void)xml.IntoElem();

	Json::Value arrayObj = root["list"];
	for(ELTE_UINT32 i = 0; i < arrayObj.size(); ++i)
	{
		if(0 == i)
		{
			(void)xml.AddChildElem("UserInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("UserInfo");
		}
		(void)xml.AddChildElem("UserID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(arrayObj[i]["isdn"].asString().c_str());
		(void)xml.AddElem("UserCategory");
		(void)xml.SetElemValue(arrayObj[i]["category"].asString().c_str());
		(void)xml.AddElem("UserPriority");
		(void)xml.SetElemValue(arrayObj[i]["priority"].asString().c_str());
		(void)xml.AddElem("UserName");
		(void)xml.SetElemValue(arrayObj[i]["name"].asString().c_str());

		xml.OutOfElem();
	}
	if (0 < arrayObj.size())
	{
		xml.OutOfElem();
	}
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_GetGroupUsers_Rsp(const Json::Value& root, std::string& xmlStr)
{
		/************************************************************************
	--- XML 格式 ---
	<Content> 
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

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupUserInfoList");
	(void)xml.IntoElem();

	Json::Value arrayObj = root["list"];
	for(ELTE_UINT32 i = 0; i < arrayObj.size(); ++i)
	{
		if(0 == i)
		{
			(void)xml.AddChildElem("GroupUserInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("GroupUserInfo");
		}
		(void)xml.AddChildElem("UserID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(arrayObj[i]["isdn"].asString().c_str());
		(void)xml.AddElem("GroupID");
		(void)xml.SetElemValue(arrayObj[i]["group"].asString().c_str());
		(void)xml.AddElem("UserPriorityInGroup");
		(void)xml.SetElemValue(arrayObj[i]["userpriority"].asString().c_str());
		(void)xml.AddElem("MemberType");
		(void)xml.SetElemValue(arrayObj[i]["membertype"].asString().c_str());

		xml.OutOfElem();
	}
	if (0 < arrayObj.size())
	{
		xml.OutOfElem();
	}
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_GetUserInfo_Rsp(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<UserInfo>
			<UserID></UserID>
			<UserCategory></UserCategory>
			<UserPriority></UserPriority>
			<UserName></UserName>
		</UserInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();
	
	Json::Value value = root["value"];

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("UserID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(value["isdn"].asString().c_str());
	(void)xml.AddElem("UserCategory");
	(void)xml.SetElemValue(value["category"].asString().c_str());
	(void)xml.AddElem("UserPriority");
	(void)xml.SetElemValue(value["priority"].asString().c_str());
	(void)xml.AddElem("UserName");
	(void)xml.SetElemValue(value["name"].asString().c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_GetGroupInfo_Rsp(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupInfo>
			<GroupID></GroupID>
			<GroupCategory></GroupCategory>
			<GroupPriority></GroupPriority>
			<GroupName></GroupName>
		</GroupInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();
	
	Json::Value value = root["value"];

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("GroupID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(value["group"].asString().c_str());
	(void)xml.AddElem("GroupCategory");
	(void)xml.SetElemValue(value["category"].asString().c_str());
	(void)xml.AddElem("GroupPriority");
	(void)xml.SetElemValue(value["priority"].asString().c_str());
	(void)xml.AddElem("GroupName");
	(void)xml.SetElemValue(value["name"].asString().c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_GetDcInfo_Rsp(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<DcInfo>
			<DcID></DcID>
			<Priviledge></Priviledge>
			<Role></Role>
			<Alias></Alias>
		</DcInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();
	Json::Value value = root["value"];

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("DcInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("DcID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(value["dcid"].asString().c_str());
	(void)xml.AddElem("Priviledge");
	(void)xml.SetElemValue(value["priviledge"].asString().c_str());
	(void)xml.AddElem("Role");
	(void)xml.SetElemValue(value["priority"].asString().c_str());
	(void)xml.AddElem("Alias");
	(void)xml.SetElemValue(value["alias"].asString().c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_GetDcGroups_Rsp(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
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

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupInfoList");
	(void)xml.IntoElem();

	Json::Value arrayObj = root["list"];
	for(ELTE_UINT32 i = 0; i < arrayObj.size(); ++i)
	{
		if(0 == i)
		{
			(void)xml.AddChildElem("GroupInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("GroupInfo");
		}
		(void)xml.AddChildElem("GroupID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(arrayObj[i]["group"].asString().c_str());
		(void)xml.AddElem("GroupCategory");
		(void)xml.SetElemValue(arrayObj[i]["category"].asString().c_str());
		(void)xml.AddElem("GroupPriority");
		(void)xml.SetElemValue(arrayObj[i]["priority"].asString().c_str());
		(void)xml.AddElem("GroupName");
		(void)xml.SetElemValue(arrayObj[i]["name"].asString().c_str());

		xml.OutOfElem();
	}
	if (0 < arrayObj.size())
	{
		xml.OutOfElem();
	}
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_INT32 XMLProcess::SetXml_NotifyUserStatus(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID></UserID>
		<UserName></UserName>
		<StatusType></StatusType>
		<StatusValue></StatusValue>
		<AttachingGroup></AttachingGroup>
		<PeerID></PeerID>			//当type为USERSTATUS时才有
		<Direction></Direction>		//当type为USERSTATUS时才有
		<CallType></CallType>		//当type为USERSTATUS时才有
		<Encrypt></Encrypt>			//当type为USERSTATUS时才有
		<PeerNumber></PeerNumber>	//当type为USERSTATUS时才有
	</Content>
	************************************************************************/

	LOG_TRACE();

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(root["isdn"].asString().c_str());
	(void)xml.AddElem("UserName");
	if(root.isMember("name"))
	{
		(void)xml.SetElemValue(root["name"].asString().c_str());
	}
	else
	{
		(void)xml.SetElemValue("");
	}
	(void)xml.AddElem("StatusType");
	(void)xml.SetElemValue(root["statustype"].asString().c_str());
	(void)xml.AddElem("StatusValue");
	(void)xml.SetElemValue(root["statusvalue"].asString().c_str());
	(void)xml.AddElem("AttachingGroup");
	(void)xml.SetElemValue(root["attaching"].asString().c_str());

	// 用户状态
	if (USERSTATUS == eLTE_Tool::String2UInt(root["statustype"].asString().c_str()))
	{
		Json::Value valueObj = root["value"];
		(void)xml.AddElem("PeerID");
		(void)xml.SetElemValue(valueObj["peerid"].asString().c_str());
		(void)xml.AddElem("Direction");
		(void)xml.SetElemValue(valueObj["direction"].asString().c_str());
		(void)xml.AddElem("CallType");
		(void)xml.SetElemValue(valueObj["calltype"].asString().c_str());
		(void)xml.AddElem("Encrypt");
		(void)xml.SetElemValue(valueObj["encrypt"].asString().c_str());
		(void)xml.AddElem("PeerNumber");
		(void)xml.SetElemValue(valueObj["resid"].asString().c_str());
	}

	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyP2pvideocallStatus(const Json::Value& root, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<CallStatus></CallStatus>
		<Callee></Callee>
		<Caller></Caller>
		<LocalAudioPort></LocalAudioPort>
		<LocalVideoPort></LocalVideoPort>
		<RemoteAudioPort></RemoteAudioPort>
		<RemoteVideoPort></RemoteVideoPort>
		<RemoteIp></RemoteIp>
		<Uri></Uri>
		<Channel></Channel>
		<SoundMute></SoundMute>
		<UserConfirm></UserConfirm>
		<Camera></Camera>
		<SoundPtype></SoundPtype>
		<VideoFormatType></VideoFormatType>
		<CallID></CallID>
		<SignalError></SignalError>
		<FromString></FromString>
		<ToString></ToString>
	</Content>
	************************************************************************/

	LOG_TRACE();

	Json::Value paramValue=root["value"];

	//截取IP和端口
	std::string local_audio_temp=paramValue["local_audio"].asString();
	std::string local_audio_IP;
	std::string local_audio_Port;
	ELTE_INT32 iRet=eLTE_Tool::subStrIPort(local_audio_temp,local_audio_IP,local_audio_Port);

	if (RET_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("subStrIPort error!");
	}

	std::string server_audio_temp=paramValue["server_audio"].asString();
	std::string server_audio_IP;
	std::string server_audio_Port;
	iRet=eLTE_Tool::subStrIPort(server_audio_temp,server_audio_IP,server_audio_Port);
	
	if (RET_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("subStrIPort error!");
	}

	std::string local_video_temp=paramValue["local_video"].asString();
	std::string local_video_IP;
	std::string local_video_Port;
	iRet=eLTE_Tool::subStrIPort(local_video_temp,local_video_IP,local_video_Port);

	if (RET_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("subStrIPort error!");
	}

	std::string server_video_temp=paramValue["server_video"].asString();
	std::string server_video_IP;
	std::string server_video_Port;
	iRet=eLTE_Tool::subStrIPort(server_video_temp,server_video_IP,server_video_Port);
	
	if (RET_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("subStrIPort error!");
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("CallStatus");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(root["rsp"].asString().c_str());
	(void)xml.AddElem("Callee");
	(void)xml.SetElemValue(paramValue["callee"].asString().c_str());
	(void)xml.AddElem("Caller");
	(void)xml.SetElemValue(paramValue["calller"].asString().c_str());
	(void)xml.AddElem("LocalAudioPort");
	(void)xml.SetElemValue(local_audio_Port.c_str());
	(void)xml.AddElem("LocalVideoPort");
	(void)xml.SetElemValue(local_video_Port.c_str());
	(void)xml.AddElem("RemoteAudioPort");
	(void)xml.SetElemValue(server_audio_Port.c_str());
	(void)xml.AddElem("RemoteVideoPort");
	(void)xml.SetElemValue(server_video_Port.c_str());
	(void)xml.AddElem("RemoteIp");
	(void)xml.SetElemValue(server_video_IP.c_str());
	(void)xml.AddElem("Uri");
	(void)xml.SetElemValue(paramValue["uri"].asString().c_str());
	(void)xml.AddElem("Channel");
	(void)xml.SetElemValue("0");
	(void)xml.AddElem("SoundMute");
	(void)xml.SetElemValue(paramValue["mute"].asString().c_str());
	(void)xml.AddElem("UserConfirm");
	(void)xml.SetElemValue(paramValue["confirm"].asString().c_str());
	(void)xml.AddElem("Camera");
	(void)xml.SetElemValue("1");
	(void)xml.AddElem("SoundPtype");
	(void)xml.SetElemValue(paramValue["mute"].asString().c_str());
	(void)xml.AddElem("VideoFormatType");
	(void)xml.SetElemValue(paramValue["fmt"].asString().c_str());
	(void)xml.AddElem("CallID");
	(void)xml.SetElemValue("1");
	(void)xml.AddElem("SignalError");
	(void)xml.SetElemValue("0");
	(void)xml.AddElem("FromString");
	(void)xml.SetElemValue("0");
	(void)xml.AddElem("ToString");
	(void)xml.SetElemValue("1");
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;	
}

ELTE_INT32 XMLProcess::SetXml_NotifyModuleStatus(const Json::Value& root, std::string& xmlStr)
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

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(root["isdn"].asString().c_str());
	(void)xml.AddElem("ModuleType");
	(void)xml.SetElemValue(root["statustype"].asString().c_str());
	(void)xml.AddElem("ModuleStatus");
	(void)xml.SetElemValue(root["statusvalue"].asString().c_str());

	//
	(void)xml.AddElem("CallBackMsgType");
	if(root.isMember("callbackmsgtype"))
	{
		(void)xml.SetElemValue(root["callbackmsgtype"].asString().c_str());
	}
	else
	{
		(void)xml.SetElemValue("");
	}

	(void)xml.AddElem("ModulePara");
	if(root.isMember("para"))
	{
		std::string strPara;
		Json::Value para = root["para"];
		for(ELTE_UINT32 i = 0; i < para.size(); ++i)
		{
			if(para[i].isMember("newip"))
			{
				strPara.append("newip:");
				strPara.append(para[i]["newip"].asString().c_str());
			}
			if(para[i].isMember("ret"))
			{
				strPara.append("ret:");
				if (para[i]["ret"].isString())
				{
					strPara.append(para[i]["ret"].asString().c_str());
				}
				else if(para[i]["ret"].isInt())
				{
					strPara.append(eLTE_Tool::Int2String(para[i]["ret"].asInt()));
				}
				
			}
		}
		(void)xml.SetElemValue(strPara.c_str());
	}
	else
	{
		(void)xml.SetElemValue("");
	}

	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SDK_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SDK_ERR_SUCCESS;
}

int XMLProcess::GetXml_StartRealPlay_Req(const ELTE_CHAR* xmlStr, VIDEO_PARAM& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<VideoParam>
			<VideoFormat></VideoFormat>//D1、CIF、QCIF、720P或1080P
			<CameraType></CameraType>//“0”表示前置，“1”表示后置。
			<UserConfirmType></UserConfirmType>//“0”表示不需要用户确认，“1”表示需要用户确认。
			<MuteType></MuteType>//“0”表示需要伴音；“1”表示不需要伴音
		</VideoParam>
	</Content>
	************************************************************************/

	LOG_TRACE();
	if (NULL == xmlStr)
	{
		LOG_RUN_ERROR("xmlStr is null.");
		return eLTE_SDK_ERR_INVALID_PARAM;
	}

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	if (!xmlParse.FindElem("VideoParam"))
	{
		LOG_RUN_ERROR("FindElem VideoParam failed.");
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "VideoFormat", srcValue, elemValue, uiMaxLen);
	param.VideoFormat = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CameraType", srcValue, elemValue, uiMaxLen);
	param.CameraType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserConfirmType", srcValue, elemValue, uiMaxLen);
	param.UserConfirmType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "MuteType", srcValue, elemValue, uiMaxLen);
	param.MuteType = elemValue;

	return eLTE_SDK_ERR_SUCCESS;
}