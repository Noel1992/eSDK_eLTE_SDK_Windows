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

#include "stdafx.h"
#include "XMLProcess.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Tool.h"
#include "eLTE_Xml.h"
#include "mq/p2pvideocallstatus_indicator.h"
#include "mq/sds_message_indicator.h"
#include "mq/provchange_indicator.h"
#include "mq/grpcallstatus_indicator.h"
#include "mq/p2pcallstatus_indicator.h"
#include "mq/modulestatus_indicator.h"
#include <Shlwapi.h>

std::string XMLProcess::m_speakerID = "";
std::string XMLProcess::m_transferNum = "";

XMLProcess::XMLProcess(void)
{
}


XMLProcess::~XMLProcess(void)
{
}

int XMLProcess::SetXml_NotifyGroupStatus(GrpDspInfo* const pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID></GroupID>
		<GroupCallStatus></GroupCallStatus>
		<Speaker></Speaker>
		<LocalPort></LocalPort>
		<RemotePort></RemotePort>
		<RemoteIP></RemoteIP>
		<SoundPtype></SoundPtype>
		<SpeakerName></SpeakerName>
		<CallID></CallID>
		<SignalError></SignalError>
		<FromString></FromString>
		<ToString></ToString>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getGrpId()).c_str());
	(void)xml.AddElem("GroupCallStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSpStatus()).c_str());
	(void)xml.AddElem("Speaker");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSpeaker()).c_str());
	(void)xml.AddElem("LocalPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getLocalPort()).c_str());
	(void)xml.AddElem("RemotePort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getRemotePort()).c_str());
	(void)xml.AddElem("RemoteIP");
	(void)xml.SetElemValue(eLTE_Tool::GetIPByUIntValue(pInfo->getRemoteIP()).c_str());
	(void)xml.AddElem("SoundPtype");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSoundPtype()).c_str());
	(void)xml.AddElem("SpeakerName");
	(void)xml.SetElemValue(pInfo->getSpeakerName().c_str());
	(void)xml.AddElem("CallID");
	(void)xml.SetElemValue(eLTE_Tool::UInt2String(pInfo->getCallID()).c_str());
	(void)xml.AddElem("SignalError");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSignalError()).c_str());
	(void)xml.AddElem("FromString");
	(void)xml.SetElemValue(pInfo->getFromString());
	(void)xml.AddElem("ToString");
	(void)xml.SetElemValue(pInfo->getToString());
	xml.OutOfElem();

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyResourceStatus(ResourceStatusIndicator* const pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getResId()).c_str());
	(void)xml.AddElem("ResourceName");
	(void)xml.SetElemValue(pInfo->getUserName());
	(void)xml.AddElem("StatusType");
	resource_status_t eStatusType = pInfo->getStatusType();
	(void)xml.SetElemValue(eLTE_Tool::Int2String((int)eStatusType).c_str());

	res_status_value_t eStatusValue = pInfo->getStatusValue();
	// 安全红线
	int iValue = (STATUS_PASSWORD_WRONG == eStatusValue) ? STATUS_NOT_FOUND : eStatusValue;

	(void)xml.AddElem("StatusValue");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(iValue).c_str());
	(void)xml.AddElem("AttachingGroup");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getAttachingGrp()).c_str());

	if (GRPCALLSTATUS == eStatusType)
	{
		//组呼当前的主讲人
		GrpResourceStatusIndicator *pGrpResourceStatus = dynamic_cast<GrpResourceStatusIndicator*>(pInfo);
		if (NULL == pGrpResourceStatus)
		{
			LOG_RUN_ERROR("pGrpResourceStatus is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		
		(void)xml.AddElem("Speaker");
		(void)xml.SetElemValue(pGrpResourceStatus->getSpeaker().c_str());
	}
	
	// 动态组添加cause字段
	else if (USERDGNASTATUS == eStatusType)
	{
		DGNAStatusIndicator *pDGNAStatus = dynamic_cast<DGNAStatusIndicator*>(pInfo);
		if (NULL == pDGNAStatus)
		{
			LOG_RUN_ERROR("pDGNAStatusIndicator is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		(void)xml.AddElem("Cause");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pDGNAStatus->getCause()).c_str());
	}
	// 派接组组添加cause字段
	else if (GRPPATCHSTATUS == eStatusType)
	{
		GroupPatchStatusIndicator *pPCHGRPStatus = dynamic_cast<GroupPatchStatusIndicator*>(pInfo);
		if (NULL == pPCHGRPStatus)
		{
			LOG_RUN_ERROR("pPCHGRPStatus is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		(void)xml.AddElem("Cause");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pPCHGRPStatus->getCause()).c_str());
		(void)xml.AddElem("MemberID");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pPCHGRPStatus->getMember()).c_str());
	}
	// 视频分发状态
	else if (VIDEODISPATCHSTATUS == eStatusType)
	{
		VideoDispatchStatusIndicator *pVideoDispatchStatus = dynamic_cast<VideoDispatchStatusIndicator*>(pInfo);
		if (NULL == pVideoDispatchStatus)
		{
			LOG_RUN_ERROR("pVideoDispatchStatus is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		(void)xml.AddElem("CameraID");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pVideoDispatchStatus->getCamID()).c_str());
	}
	// 静音状态
	else if (MUTESTATUS == eStatusType)
	{
		MuteStatusIndicator *pMuteStatus = dynamic_cast<MuteStatusIndicator*>(pInfo);
		if (NULL == pMuteStatus)
		{
			LOG_RUN_ERROR("pMuteStatus is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		MUTE_LIST groupList = pMuteStatus->getGroupLst();
		LOG_RUN_ERROR("zzq %d", groupList.size());
		if (groupList.size() > 0)
		{
			(void)xml.AddElem("GroupIDList");
			(void)xml.IntoElem();
			MUTE_LIST::const_iterator it_b = groupList.begin();
			MUTE_LIST::const_iterator it_e = groupList.end();
			for (MUTE_LIST::const_iterator itor = it_b; it_e != itor; itor++)
			{
				if (it_b == itor)
				{
					(void)xml.AddChildElem("GroupID");
					(void)xml.IntoElem();
				}
				else
				{
					(void)xml.AddElem("GroupID");
				}
				(void)xml.SetElemValue(eLTE_Tool::Int2String(*itor).c_str());

				xml.OutOfElem();
			}
		}
	}

	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyP2pcallStatus(P2pcallStatusIndicator* const pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<P2pcallStatus></P2pcallStatus>
		<Caller></Caller>
		<Callee></Callee>
		<Inserter></Inserter>
		<Targeter></Targeter>
		<Transfer></Transfer>
		<LocalPort></LocalPort>
		<RemotePort></RemotePort>
		<RemoteIP></RemoteIP>
		<SoundPtype></SoundPtype>
		<CallID></CallID>
		<SignalError></SignalError>
		<FromString></FromString>
		<ToString></ToString>
		<DiscreetListenee></DiscreetListenee>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("P2pcallStatus");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getStatus()).c_str());
	(void)xml.AddElem("Caller");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getCaller()).c_str());
	(void)xml.AddElem("Callee");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getCallee()).c_str());
	(void)xml.AddElem("Inserter");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getInserter()).c_str());
	(void)xml.AddElem("Targeter");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getTargeter()).c_str());
	(void)xml.AddElem("Transfer");
	(void)xml.SetElemValue(pInfo->getTransfer());
	(void)xml.AddElem("LocalPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getLocalPort()).c_str());
	(void)xml.AddElem("RemotePort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getRemotePort()).c_str());
	(void)xml.AddElem("RemoteIP");
	(void)xml.SetElemValue(eLTE_Tool::GetIPByUIntValue(pInfo->getRemoteIP()).c_str());
	(void)xml.AddElem("SoundPtype");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSoundPtype()).c_str());
	(void)xml.AddElem("CallID");
	(void)xml.SetElemValue(eLTE_Tool::UInt2String(pInfo->getCallID()).c_str());
	(void)xml.AddElem("SignalError");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSignalError()).c_str());
	(void)xml.AddElem("FromString");
	(void)xml.SetElemValue(pInfo->getFromString());
	(void)xml.AddElem("ToString");
	(void)xml.SetElemValue(pInfo->getToString());
	(void)xml.AddElem("DiscreetListenee");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getDiscreetListenee()).c_str());
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyP2pvideocallStatus(P2pvideocallStatusIndicator* const pInfo, std::string& xmlStr)
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
		<Ptz></Ptz>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("CallStatus");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getStatus()).c_str());
	(void)xml.AddElem("Callee");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getCallee()).c_str());
	(void)xml.AddElem("Caller");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getCaller()).c_str());
	(void)xml.AddElem("LocalAudioPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getlocalaudioport()).c_str());
	(void)xml.AddElem("LocalVideoPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getlocalvideoport()).c_str());
	(void)xml.AddElem("RemoteAudioPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getremoteaudioport()).c_str());
	(void)xml.AddElem("RemoteVideoPort");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getremotevideoport()).c_str());
	(void)xml.AddElem("RemoteIp");
	(void)xml.SetElemValue(eLTE_Tool::GetIPByUIntValue(pInfo->getRemoteIp()).c_str());
	(void)xml.AddElem("Uri");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getUri()).c_str());
	(void)xml.AddElem("Channel");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getChannel()).c_str());
	(void)xml.AddElem("SoundMute");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSoundMute()).c_str());
	(void)xml.AddElem("UserConfirm");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getUserCfm()).c_str());
	(void)xml.AddElem("Camera");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getCamera()).c_str());
	(void)xml.AddElem("SoundPtype");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSoundPtype()).c_str());
	(void)xml.AddElem("VideoFormatType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getFmt()).c_str());
	(void)xml.AddElem("CallID");
	(void)xml.SetElemValue(eLTE_Tool::UInt2String(pInfo->getCallID()).c_str());
	(void)xml.AddElem("SignalError");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getSignalError()).c_str());
	(void)xml.AddElem("FromString");
	(void)xml.SetElemValue(pInfo->getFromString());
	(void)xml.AddElem("ToString");
	(void)xml.SetElemValue(pInfo->getToString());
	(void)xml.AddElem("Ptz");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getPTZFlag()).c_str());
	xml.OutOfElem();
	LOG_RUN_DEBUG("SetXml_NotifyP2pvideocallStatus SET XML finished.");

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyUserStatus(ResourceStatusIndicator* const pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getResId()).c_str());
	(void)xml.AddElem("UserName");
	(void)xml.SetElemValue(pInfo->getUserName());
	(void)xml.AddElem("StatusType");
	resource_status_t eStatusType = pInfo->getStatusType();
	(void)xml.SetElemValue(eLTE_Tool::Int2String((int)eStatusType).c_str());
	(void)xml.AddElem("StatusValue");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getStatusValue()).c_str());
	(void)xml.AddElem("AttachingGroup");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getAttachingGrp()).c_str());

	// 用户状态
	if (USERSTATUS == eStatusType)
	{
		UserStatusIndicator* pUserStatus = dynamic_cast<UserStatusIndicator*>(pInfo);
		if (NULL == pUserStatus)
		{
			LOG_RUN_ERROR("pUserStatus is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		(void)xml.AddElem("PeerID");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserStatus->getPeerUser()).c_str());
		(void)xml.AddElem("Direction");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserStatus->getDirection()).c_str());
		(void)xml.AddElem("CallType");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserStatus->getCallType()).c_str());
		(void)xml.AddElem("Encrypt");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserStatus->getEncrypt()).c_str());
		(void)xml.AddElem("PeerNumber");
		(void)xml.SetElemValue(pUserStatus->getPeerNumber());
	}

	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifySDSReport(SdsMessageIndicator* const pSdsMessage, std::string& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<SdsType>消息类型</SdsType>
			<SdsContent>消息内容</SdsContent>
			<SdsFrom>发件人</SdsFrom>
			<SdsTo>收件人</SdsTo>
			<MsgState>状态短信报在线还是离线</MsgState>
			<RetCode><RetCode>
			<EmergGroupID>紧急状态的终端ID</EmergGroupID>
			<EmergUeID></EmergUeID>
			<GroupID></GroupID>
			<SdsMmsFileNameList>
				<SdsMmsFileName>附件文件名列表</SdsMmsFileName>
				<SdsMmsFileName>附件文件名列表</SdsMmsFileName>
			</SdsMmsFileNameList>
			<SdsSubject>消息主题</SdsSubject>
			<SdsDirection>接发短信标志</SdsDirection>
			<SdsDate>消息发送日期</SdsDate>
			<SdsTime>消息发送时间</SdsTime>
			<StatusCode>状态短信的状态码（普通短信不适用）</StatusCode>
			</Content>
	************************************************************************/
	LOG_TRACE();
	SET_SDS_XML_HEADER();

	(void)xml.AddElem("SdsFrom");
	(void)xml.SetElemValue(pSdsMessage->getSdsFrom().c_str());
	(void)xml.AddElem("SdsTo");
	(void)xml.SetElemValue(pSdsMessage->getSdsTo().c_str());
	(void)xml.AddElem("MsgState");
	(void)xml.SetElemValue(eLTE_Tool::Int2String((int)(pSdsMessage->getMsgState())).c_str());
	(void)xml.AddElem("RetCode");
	(void)xml.SetElemValue(pSdsMessage->getRetCode().c_str());
	(void)xml.AddElem("EmergGroupID");
	(void)xml.SetElemValue(pSdsMessage->getEmergencyStatusGpId().c_str());
	(void)xml.AddElem("EmergUeID");
	(void)xml.SetElemValue(pSdsMessage->getEmergencyStatusUEId().c_str());
	(void)xml.AddElem("GroupID");
	(void)xml.SetElemValue(pSdsMessage->getGroupId().c_str());

	RecvfileList recvFileList = pSdsMessage->getSdsFilename();

	if (0 < recvFileList.size())
	{
		(void)xml.AddElem("SdsMmsFileNameList");
		std::list<std::string>::iterator it = recvFileList.begin();
		
		for (; it != recvFileList.end(); ++it)
		{
			if(it == recvFileList.begin())
			{
				(void)xml.AddChildElem("SdsMmsFileName");
				(void)xml.IntoElem();
			}
			else
			{
				(void)xml.AddElem("SdsMmsFileName");
			}

			std::string strFilePath = *it;
			std::string strTemp;
			if (PathIsRelative(strFilePath.c_str()))
			{
				eLTE_Tool::ReplaceString(strFilePath, strTemp, "./", "");
				eLTE_Tool::ReplaceString(strTemp, strTemp, "//", "/");
				eLTE_Tool::ReplaceString(strTemp, strTemp, "/", "\\");
				strFilePath = "";
				strFilePath.append(eLTE_Tool::ANSIToUTF8(eLTE_Tool::GetServicePath()));
				strFilePath.append(strTemp);
			}
			(void)xml.SetElemValue(strFilePath.c_str());			
		}
		xml.OutOfElem();
	}

	SET_SDS_XML();

	GET_XML_STR(eLTE_SVC_ERR_FAILED);

	//INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyUserSdsStatusReport(SdsMessageIndicator* const pSdsMessage, std::string& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<SdsType>消息类型</SdsType>
			<SdsContent>消息内容</SdsContent>
			<SdsFrom>发件人</SdsFrom>
			<SdsSubject>消息主题</SdsSubject>
			<SdsDirection>接发短信标志</SdsDirection>
			<SdsDate>消息发送日期</SdsDate>
			<SdsTime>消息发送时间</SdsTime>
			<StatusCode>状态短信的状态码（普通短信不适用）</StatusCode>
			</Content>
	************************************************************************/
	LOG_TRACE();
	SET_SDS_XML_HEADER();

	SET_SDS_XML();

	GET_XML_STR(eLTE_SVC_ERR_FAILED);

	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifySDSSendStatus(SdsMessageIndicator* const pSdsMessage, std::string& xmlStr)
{
	/************************************************************************
			--- XML 格式 ---
			<Content>
			<SdsReceiver>接收者</SdsReceiver>
			<SdsSubject>一般为msgid</SdsSubject>
			<SdsDirection>true，表示为发送状态的通知</SdsDirection>
			<SdsRetCode>表示发送状态</SdsRetCode>
			</Content>
	************************************************************************/
	LOG_TRACE();

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("SdsReceiver");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pSdsMessage->getSdsTo().c_str());
	(void)xml.AddElem("SdsSubject");
	(void)xml.SetElemValue(pSdsMessage->getSdsSubject().c_str());
	(void)xml.AddElem("SdsDirection");
	(void)xml.SetElemValue("true");
	(void)xml.AddElem("SdsRetCode");
	(void)xml.SetElemValue(pSdsMessage->getRetCode().c_str());
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyModuleStatus(DCModuleStatusIndicator* const pInfo, std::string& xmlStr)
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
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getResid()).c_str());
	(void)xml.AddElem("ModuleType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getModuleType()).c_str());
	(void)xml.AddElem("ModuleStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getModuleStatus()).c_str());
	(void)xml.AddElem("CallBackMsgType");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getCallBackBizType()).c_str());
	(void)xml.AddElem("ModulePara");
	(void)xml.SetElemValue(pInfo->getModulePara().c_str());
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyProvisionChange(ProvChangeIndicator* const pInfo, std::string& xmlStr)
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
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ChangeType");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getChangeType()).c_str());
	(void)xml.AddElem("ChangeMO");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getChangeMO()).c_str());

	INFO_PARAM1(pInfo->getChangeMO());
	switch (pInfo->getChangeMO())
	{
	case MO_PTTUser:
		{
			ProvChangeIndicator_PTTUser* pPTTUser = dynamic_cast<ProvChangeIndicator_PTTUser*>(pInfo);
			if (NULL == pPTTUser)
			{
				LOG_RUN_ERROR("pPTTUser is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("PTTUserInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->userid).c_str());
			(void)xml.AddElem("UserCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->usercategory).c_str());
			(void)xml.AddElem("UserPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->userpriority).c_str());
			(void)xml.AddElem("UserName");
			(void)xml.SetElemValue(pPTTUser->username.c_str());
			(void)xml.AddElem("VpnID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->vpnid).c_str());
			(void)xml.AddElem("VpnOut");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->vpnout).c_str());
			(void)xml.AddElem("VpnIn");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->vpnin).c_str());
			(void)xml.AddElem("Location");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPTTUser->location).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_FixedUser:
		{
			ProvChangeIndicator_FixedUser* pFixedUser = dynamic_cast<ProvChangeIndicator_FixedUser*>(pInfo);
			if (NULL == pFixedUser)
			{
				LOG_RUN_ERROR("pFixedUser is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("FixedUserInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->userid).c_str());
			(void)xml.AddElem("UserCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->usercategory).c_str());
			(void)xml.AddElem("UserPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->userpriority).c_str());
			(void)xml.AddElem("UserName");
			(void)xml.SetElemValue(pFixedUser->username.c_str());
			(void)xml.AddElem("VpnID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->vpnid).c_str());
			(void)xml.AddElem("VpnOut");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->vpnout).c_str());
			(void)xml.AddElem("VpnIn");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->vpnin).c_str());
			(void)xml.AddElem("Location");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pFixedUser->location).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_DCAttribute:
		{
			ProvChangeIndicator_DCAttribute* pDCAttribute = dynamic_cast<ProvChangeIndicator_DCAttribute*>(pInfo);
			if (NULL == pDCAttribute)
			{
				LOG_RUN_ERROR("pDCAttribute is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("DCAttributeInfo");
			(void)xml.AddChildElem("DcID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pDCAttribute->dcid).c_str());
			(void)xml.AddElem("Priviledge");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pDCAttribute->priviledge).c_str());
			(void)xml.AddElem("Role");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pDCAttribute->role).c_str());
			(void)xml.AddElem("Alias");
			(void)xml.SetElemValue(pDCAttribute->Alias.c_str());
			xml.OutOfElem();
		}
		break;
	case MO_Group:
	case MO_DMGroup:
		{
			ProvChangeIndicator_Group* pGroup = dynamic_cast<ProvChangeIndicator_Group*>(pInfo);
			if (NULL == pGroup)
			{
				LOG_RUN_ERROR("pGroup is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("GroupInfo");
			(void)xml.AddChildElem("GroupID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroup->grpid).c_str());
			(void)xml.AddElem("GroupCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroup->grpcategory).c_str());
			(void)xml.AddElem("GroupPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroup->grppriority).c_str());
			(void)xml.AddElem("GroupName");
			(void)xml.SetElemValue(pGroup->grpname.c_str());
			(void)xml.AddElem("GroupState");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroup->grpstate).c_str());
			(void)xml.AddElem("VpnID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroup->vpnid).c_str());
			(void)xml.AddElem("SetupDcID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroup->setupDcId).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_GroupPTTUser:
	case MO_GroupFixedUser:
	case MO_DynamicGroupMember:
		{
			ProvChangeIndicator_GrpUserInfo* pGroupUserInfo = dynamic_cast<ProvChangeIndicator_GrpUserInfo*>(pInfo);
			if (NULL == pGroupUserInfo)
			{
				LOG_RUN_ERROR("pGroupUserInfo is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("GroupUserInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupUserInfo->userid).c_str());
			(void)xml.AddElem("GroupID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupUserInfo->grpid).c_str());
			(void)xml.AddElem("UserPriorityInGroup");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupUserInfo->userpriorityInGrp).c_str());
			(void)xml.AddElem("MemberType");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupUserInfo->memberType).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_DMGPhotoGroupUser:
		{
			ProvChangeIndicator_PhotoGrpUserInfo* pPhotoGrpUserInfo = dynamic_cast<ProvChangeIndicator_PhotoGrpUserInfo*>(pInfo);
			if (NULL == pPhotoGrpUserInfo)
			{
				LOG_RUN_ERROR("pPhotoGrpUserInfo is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("PhotoGrpUserInfo");
			(void)xml.AddChildElem("DmgID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPhotoGrpUserInfo->dmgid).c_str());
			(void)xml.AddElem("UserID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPhotoGrpUserInfo->userid).c_str());
			(void)xml.AddElem("GroupID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPhotoGrpUserInfo->grpid).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_RangeList:
		{
			ProvChangeIndicator_RangeList* pRangeList = dynamic_cast<ProvChangeIndicator_RangeList*>(pInfo);
			if (NULL == pRangeList)
			{
				LOG_RUN_ERROR("pRangeList is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("RangeListInfo");
			(void)xml.AddChildElem("RangeID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pRangeList->rangid).c_str());
			(void)xml.AddElem("RangeCategory");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pRangeList->Rangecategory).c_str());
			(void)xml.AddElem("From");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pRangeList->From).c_str());
			(void)xml.AddElem("To");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pRangeList->To).c_str());
			(void)xml.AddElem("Alias");
			(void)xml.SetElemValue(pRangeList->Alias.c_str());
			xml.OutOfElem();
		}
		break;
	case MO_DCRanges:
		{
			ProvChangeIndicator_DCRange* pDCRange = dynamic_cast<ProvChangeIndicator_DCRange*>(pInfo);
			if (NULL == pDCRange)
			{
				LOG_RUN_ERROR("pDCRange is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("DCRangesInfo");
			(void)xml.AddChildElem("RangeID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pDCRange->rangid).c_str());
			(void)xml.AddElem("DcID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pDCRange->dcid).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_HOTLINECFG:
		{
			ProvChangeIndicator_HotlineCfg* pHotlineCfg = dynamic_cast<ProvChangeIndicator_HotlineCfg*>(pInfo);
			if (NULL == pHotlineCfg)
			{
				LOG_RUN_ERROR("pHotlineCfg is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("HotlineCfgInfo");
			(void)xml.AddChildElem("DcID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pHotlineCfg->DcId).c_str());
			(void)xml.AddElem("HotIndex");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pHotlineCfg->HotIndex).c_str());
			(void)xml.AddElem("Number");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pHotlineCfg->Number).c_str());
			(void)xml.AddElem("Alias");
			(void)xml.SetElemValue(pHotlineCfg->Alias.c_str());
			xml.OutOfElem();
		}
		break;
	case MO_STMSGCFG:
		{
			ProvChangeIndicator_StMsgCfg* pStMsgCfg = dynamic_cast<ProvChangeIndicator_StMsgCfg*>(pInfo);
			if (NULL == pStMsgCfg)
			{
				LOG_RUN_ERROR("pStMsgCfg is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("StMsgCfgInfo");
			(void)xml.AddChildElem("ID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pStMsgCfg->id).c_str());
			(void)xml.AddElem("StMsg");
			(void)xml.SetElemValue(pStMsgCfg->stmsg.c_str());
			xml.OutOfElem();
		}
		break;
	//case MO_DECUSER:
	//	{
	//		// 没有找到对应类
	//	}
	//	break;
	case MO_GISCFG:
		{
			ProvChangeIndicator_GisCfg* pGisCfg = dynamic_cast<ProvChangeIndicator_GisCfg*>(pInfo);
			if (NULL == pGisCfg)
			{
				LOG_RUN_ERROR("pGisCfg is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("GisCfgInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGisCfg->userid).c_str());
			(void)xml.AddElem("ReportSwitch");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pGisCfg->reportSwitch).c_str());
			(void)xml.AddElem("ReportPeriod");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pGisCfg->reportPeriod).c_str());
			(void)xml.AddElem("ReportCapability");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pGisCfg->reportCapability).c_str());
			(void)xml.AddElem("ReportDistance");
			(void)xml.SetElemValue(eLTE_Tool::UInt2String(pGisCfg->reportDistance).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_GISIPC:
		{
			ProvChangeIndicator_GisIpc* pGisIpc = dynamic_cast<ProvChangeIndicator_GisIpc*>(pInfo);
			if (NULL == pGisIpc)
			{
				LOG_RUN_ERROR("pGisIpc is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("GisIpcInfo");
			(void)xml.AddChildElem("UserID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pGisIpc->userid).c_str());
			(void)xml.AddElem("Latitude");
			(void)xml.SetElemValue(eLTE_Tool::Double2String(pGisIpc->latitude).c_str());
			(void)xml.AddElem("Longitude");
			(void)xml.SetElemValue(eLTE_Tool::Double2String(pGisIpc->longitude).c_str());
			(void)xml.AddElem("Altitude");
			(void)xml.SetElemValue(eLTE_Tool::Double2String(pGisIpc->altitude).c_str());
			(void)xml.AddElem("Address");
			(void)xml.SetElemValue(pGisIpc->address.c_str());
			xml.OutOfElem();
		}
		break;
	case MO_PatchGroup:
		{
			ProvChangeIndicator_PatchGroup* pPatchGroup = dynamic_cast<ProvChangeIndicator_PatchGroup*>(pInfo);
			if (NULL == pPatchGroup)
			{
				LOG_RUN_ERROR("pPatchGroup is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("PatchGroupInfo");
			(void)xml.AddChildElem("GroupNumber");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroup->GroupNumber).c_str());
			(void)xml.AddElem("SetupDcId");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroup->SetupDcId).c_str());
			(void)xml.AddElem("PGPriority");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroup->PGPriority).c_str());
			(void)xml.AddElem("DcPatchIndex");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroup->DcPatchIndex).c_str());
			(void)xml.AddElem("PGName");
			(void)xml.SetElemValue(pPatchGroup->PGName.c_str());
			(void)xml.AddElem("VPNID");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroup->VPNID).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_PatchGroupMember:
		{
			ProvChangeIndicator_PatchGroupMember* pPatchGroupMember = dynamic_cast<ProvChangeIndicator_PatchGroupMember*>(pInfo);
			if (NULL == pPatchGroupMember)
			{
				LOG_RUN_ERROR("pPatchGroupMember is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("PatchGroupMemberInfo");
			(void)xml.AddChildElem("GroupNumber");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupMember->GroupNumber).c_str());
			(void)xml.AddElem("MemberGroupId");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupMember->MemberGroupId).c_str());
			xml.OutOfElem();
		}
		break;
	case MO_EMERGRPTIME:
		{
			ProvChangeIndicator_EmergencyGrp* pEmergencyGrp = dynamic_cast<ProvChangeIndicator_EmergencyGrp*>(pInfo);
			if (NULL == pEmergencyGrp)
			{
				LOG_RUN_ERROR("pEmergencyGrp is null.");
				return eLTE_SVC_ERR_NULL_POINTER;
			}

			(void)xml.AddElem("EmergencyGrpInfo");
			(void)xml.AddChildElem("EmencygrpID");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pEmergencyGrp->EmencygrpID).c_str());
			(void)xml.AddElem("Emencygrptime");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pEmergencyGrp->Emencygrptime).c_str());
			xml.OutOfElem();
		}
		break;
	default:
		{
			// 其他不做事件回调
			// 没有找到对应类
			LOG_RUN_WARN("Invalid ChangeMO.");
			return eLTE_SVC_ERR_FAILED;
		}
	}

	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyGISReport(GISReportIndicator* const pInfo, std::string& xmlStr)
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
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->getSender().c_str());
	(void)xml.AddElem("Time");
	(void)xml.SetElemValue(pInfo->getTime().c_str());
	(void)xml.AddElem("Altitude");
	(void)xml.SetElemValue(pInfo->getAltitude().c_str());
	(void)xml.AddElem("Latitude");
	(void)xml.SetElemValue(pInfo->getLatitude().c_str());
	(void)xml.AddElem("Longtitude");
	(void)xml.SetElemValue(pInfo->getLongitude().c_str());
	(void)xml.AddElem("TriggerCode");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getReasonCode()).c_str());
	(void)xml.AddElem("ReportStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->getRptState()).c_str());
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	//INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_NotifyGISStatus(GISReportIndicator* const pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>上报方</ResourceID>
		<AckStatusList>
			<AckStatus>终端号：订阅状态，举例如下：1001:0。</AckStatus>
		</AckStatusList>
	</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->getSender().c_str());
	(void)xml.AddElem("AckStatusList");

	AckResultMap::iterator itor_b = pInfo->getAckRsp().begin();
	AckResultMap::iterator itor_e = pInfo->getAckRsp().end();
	LOG_RUN_DEBUG("GisSub rsp size is %d", pInfo->getAckRsp().size());
	std::string strAckStatus;
	for(; itor_b != itor_e; ++itor_b)
	{
		if(itor_b == pInfo->getAckRsp().begin())
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

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_Connect_Req(const char* xmlStr, std::string& passwd)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ConnectPassword>加密字符串</ConnectPassword>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 64;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ConnectPassword", srcValue, elemValue, uiMaxLen);
	passwd = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_Login_Req(const char* xmlStr, 
	std::string& userId, std::string& pwd, std::string& serverIP, std::string& localIP, std::string& sipPort)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID>调度员账号</UserID>
		<PWD>调度员密码</PWD>
		<ServerIP>调度机IP地址</ServerIP> 
		<LocalIP>本地主机IP地址</LocalIP>
		<ServerSIPPort>调度机SIP服务端口</ServerSIPPort>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "PWD", srcValue, elemValue, uiMaxLen);
	pwd = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ServerIP", srcValue, elemValue, uiMaxLen);
	serverIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "LocalIP", srcValue, elemValue, uiMaxLen);
	localIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ServerSIPPort", srcValue, elemValue, uiMaxLen);
	sipPort = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_RecvVideoPlay_Req(const char* xmlStr, int& resId)
{
/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源对象id</ResourceID> 
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_Logout_Req(const char* xmlStr, std::string& userId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID>资源对象调度员账号</UserID> 
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_TriggerStatusReport_Req(const char* xmlStr, unsigned int& uiEnable)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<EnableStatusReport>是否开启状态上报</EnableStatusReport>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "EnableStatusReport", srcValue, elemValue, uiMaxLen);
	uiEnable = eLTE_Tool::String2UInt(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_ProvisionManagerInit_Req(const char* xmlStr, std::string& serverIP, int& userId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ServerIP>调度机IP地址</ServerIP>
		<UserID>调度员账号</UserID> 
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ServerIP", srcValue, elemValue, uiMaxLen);
	serverIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_ProvisionManagerInitMRS_Req(const char* xmlStr, std::string& serverIP)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ServerIP>MRS IP地址</ServerIP>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ServerIP", srcValue, elemValue, uiMaxLen);
	serverIP = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetDcGroups_Req(const char* xmlStr, int& userId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID>调度台的用户号</UserID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetDcUsers_Req(const char* xmlStr, int& userId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID>调度台的用户号</UserID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetGroupUsers_Req(const char* xmlStr, int& groupId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID>要查询的群组号</GroupID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	groupId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetGroupInfo_Req(const char* xmlStr, int& groupId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID>要查询的群组号</GroupID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	groupId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetUserInfo_Req(const char* xmlStr, int& userId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID>调度台的用户号</UserID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetDcInfo_Req(const char* xmlStr, int& userId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UserID>调度台的用户号</UserID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, elemValue, uiMaxLen);
	userId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_SubscribeGroup_Req(const char* xmlStr, int& groupId, unsigned int& uiEnable)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID>要订阅的群组号</GroupID>
		<EnableSubscribeGroup>是否订阅群组</EnableSubscribeGroup>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	groupId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "EnableSubscribeGroup", srcValue, elemValue, uiMaxLen);
	uiEnable = eLTE_Tool::String2UInt(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GetUserRECFileInfoList_Req(const char* xmlStr, RECQueryInfo& query)
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
	
	PARSE_XML_DATA(xmlStr);
	if (!xmlParse.FindElem("RECQueryInfo"))
	{
		LOG_RUN_ERROR("FindElem RECQueryInfo failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
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
			query.call_type = ENUM_REC_UNKNOW_CALL_TYPE;
		}
		break;
	case ENUM_REC_P2P_AUDIO:
		{
			query.call_type = ENUM_REC_P2P_AUDIO;
		}
		break;
	case ENUM_REC_P2P_VIDEO:
		{
			query.call_type = ENUM_REC_P2P_VIDEO;
		}
		break;
	case ENUM_REC_VIDEO_UPLOAD:
		{
			query.call_type = ENUM_REC_VIDEO_UPLOAD;
		}
		break;
	case ENUM_REC_PTT:
		{
			query.call_type = ENUM_REC_PTT;
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid call_type. (%d)", iType);
			return eLTE_SVC_ERR_FAILED;
		}
	}

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Caller", srcValue, elemValue, uiMaxLen);
	query.caller = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Callee", srcValue, elemValue, uiMaxLen);
	query.callee = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	query.resourceid = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "StartSec", srcValue, elemValue, uiMaxLen);
	query.start_sec = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "EndSec", srcValue, elemValue, uiMaxLen);
	query.end_sec = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}
// 
// int XMLProcess::GetXml_TempUserJoinGroup_Req(const char* xmlStr, int& resId, PhonePatch_parameter& param)
// {
// /************************************************************************
// 	--- XML 格式 ---
// 	<Content>
// 		<PhonePatchParam>
// 			<ResourceID>普通组或动态组ID</ResourceID>
// 			<DcID>调度台ID</DcID>
// 			<UserID>添加到组内的临时用户ID号</UserID>
// 		</PhonePatchParam>
// 	</Content>
// 	************************************************************************/
// 
// 	PARSE_XML_DATA(xmlStr);
// 
// 	const unsigned int XML_VAR_LENGTH = 20;
// 	char elemValue[XML_VAR_LENGTH] = {0};
// 	const char* srcValue;
// 	unsigned int uiMaxLen = XML_VAR_LENGTH;
// 
// 	if (!xmlParse.FindElem("PhonePatchParam"))
// 	{
// 		LOG_RUN_ERROR("FindElem PhonePatchParam failed.");
// 		return eLTE_SVC_ERR_XML_FIND_ELEM;
// 	}
// 	(void)xmlParse.IntoElem();
// 
// 	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
// 	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
// 	resId = eLTE_Tool::String2Int(elemValue);
// 
// 	uiMaxLen = XML_VAR_LENGTH;
// 	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
// 	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DcID", srcValue, elemValue, uiMaxLen);
// 	param.DCID = eLTE_Tool::String2Int(elemValue);
// 
// 	uiMaxLen = MAX_TELENUMBER_LENGTH;
// 	eSDK_MEMSET(param.UserID, 0, sizeof(char)*MAX_TELENUMBER_LENGTH);	
// 	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserID", srcValue, param.UserID, uiMaxLen);
// 	
// 
// 	return eLTE_SVC_ERR_SUCCESS;	
// }

int XMLProcess::GetXml_ModifyDynamicGroup_Req(const char* xmlStr, DGNA_Modify_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<DynamicGroupInfo>
			<ResourceID></ResourceID>				    //创建动态组的调度台ID
			<GroupID></GroupID>							//动态组ID
			<AddUserList>					            //待添加用户ID，可以是调度台或者终端，不可以是群组
				<AddUserID></AddUserID>
			</AddUserList>
			<DeleteUserList>					        //待删除用户ID，不可以是群组
				<DeleteUserID></DeleteUserID>
			</DeleteUserList>
		</DynamicGroupInfo>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 50;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	if (!xmlParse.FindElem("DynamicGroupInfo"))
	{
		LOG_RUN_ERROR("FindElem DynamicGroupInfo failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	param.dcId = eLTE_Tool::String2Int(elemValue);
	
	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	param.grpId = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("AddUserList"))
	{
		LOG_RUN_ERROR("FindElem AddUserList failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
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
				LOG_RUN_ERROR("GetElemValue AddUserID failed.");
				return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;
			}
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);
			eSDK_MEMCPY(elemValue, sizeof(elemValue), srcValue, uiMaxLen);
			param.addList[iIndex++] = eLTE_Tool::String2Int(elemValue);

			if (iIndex > MAX_UE_IN_DGNA -1)//最后一个成员必须是0
			{
				LOG_RUN_ERROR("addList is out of memory. the max index is 20.");
				break;
			}
		} while (xmlParse.NextElem());

		param.addList[iIndex] = 0;
		xmlParse.OutOfElem();
	}
	
	if (!xmlParse.FindElem("DeleteUserList"))
	{
		LOG_RUN_ERROR("FindElem AddUserList failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
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
				LOG_RUN_ERROR("GetElemValue DeleteUserID failed.");
				return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;
			}
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);
			eSDK_MEMCPY(elemValue, sizeof(elemValue), srcValue, uiMaxLen);
			param.delList[iIndex++] = eLTE_Tool::String2Int(elemValue);

			if (iIndex >= MAX_UE_IN_DGNA -1)//最后一个成员必须是0
			{
				LOG_RUN_ERROR("delList is out of memory. the max index is 20.");
				break;
			}
		} while (xmlParse.NextElem());
		param.delList[iIndex] = 0;
		xmlParse.OutOfElem();
	}

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_OperatePatchGroup_Req(const char* xmlStr, PCHGRP_Para& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<DcID></DcID>				                    //创建该派接组的DC用户号
		<PatchGroupID></PatchGroupID>                   //派接组号
		<PatchGroupName></PatchGroupName>				//该派接组别名
		<PatchGroupMemberList>					        //普通组成员列表 20个
			<PatchGroupMember></PatchGroupMember>
		</PatchGroupMemberList>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 50;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "PatchGroupID", srcValue, elemValue, uiMaxLen);
	param.PatchGroupId = eLTE_Tool::String2Int(elemValue);

// 	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
// 	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DcID", srcValue, elemValue, uiMaxLen);
// 	param.DcId = eLTE_Tool::String2Int(elemValue);

	if (xmlParse.FindElem("PatchGroupName"))													
	{																			
		srcValue = xmlParse.GetElemValue();
		if(NULL != srcValue)
		{
			param.PatchGroupName = eLTE_Tool::ANSIToUTF8(srcValue);
		}
	}

	if (!xmlParse.FindElem("PatchGroupMemberList"))
	{
		LOG_RUN_ERROR("FindElem PatchGroupMemberList failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
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
				LOG_RUN_ERROR("GetElemValue PatchGroupMember failed.");
				return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;
			}
			uiMaxLen = (strlen(srcValue) < (uiMaxLen)) ? strlen(srcValue) : ((uiMaxLen)-1);
			eSDK_MEMCPY(elemValue, sizeof(elemValue), srcValue, uiMaxLen);
			param.memberList[iIndex++] = eLTE_Tool::String2Int(elemValue);

			if (iIndex >= 20)
			{
				LOG_RUN_ERROR("grpList is out of memory. the max index is 20.");
				break;
			}
		} while (xmlParse.NextElem());
		xmlParse.OutOfElem();
	}

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_CreateDynamicGroup_Req(const char* xmlStr, DGNA_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID></GroupID>			//动态组号
		<DcID></DcID>				//创建该动态组的DC用户号
		<Alias></Alias>				//该动态组别名
		<Priority></Priority>		//该动态组优先级，
		<MaxPeriod></MaxPeriod>		//该组的最大通话时长
		<GroupList>					//组成员列表 8个
			<GroupID></GroupID>
		</GroupList>
		<UserList>					//用户成员列表 200个
			<UserID></UserID>
		</UserList>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 50;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	param.grpId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DcID", srcValue, elemValue, uiMaxLen);
	param.dcId = eLTE_Tool::String2Int(elemValue);

	if (xmlParse.FindElem("Alias"))													
	{																			
		srcValue = xmlParse.GetElemValue();
		if(NULL != srcValue)
		{
			param.alias = eLTE_Tool::ANSIToUTF8(srcValue);
		}
	}

	PARSE_GROUPLIST_AND_USERLIST();

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_CancelDynamicGroup_Req(const char* xmlStr, int& resId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>动态组组号</ResourceID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_CreateTempGroup_Req(const char* xmlStr, DGNA_parameter& param)
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

	PARSE_XML_DATA(xmlStr);

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

	PARSE_GROUPLIST_AND_USERLIST();

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_CancelPatchGroup_Req(const char* xmlStr, int& resId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<PatchGroupID>派接组号</PatchGroupID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "PatchGroupID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_StopVideoDispatch_Req(const char* xmlStr, int& resId, Video_Dispatch_Delete& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>发起视频回传的调度员的ID号</ResourceID>
		<UserId>需要挂断的正在分发中的用户的ID号</UserId>
	</Content>
	************************************************************************/
	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);
	param.resId = resId;//待确认

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserId", srcValue, elemValue, uiMaxLen);
	param.userId = eLTE_Tool::String2Int(elemValue);
	
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_StartVideoDispatch_Req(const char* xmlStr, int& resId, VIDEO_DISPATCH_PARAM& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<Fmtvalue></Fmtvalue>//D1、CIF、QCIF、720P或1080P
		<DispatchNum></DispatchNum>//视频源用户号，填写视频源终端ID号。
		<Dstviewerlist>//视频分发的目的终端ID列表
			<Dstviewer></Dstviewer>//视频分发的目的终端或调度台，填写目的终端ID号或者目的调度台ID号
		</Dstviewerlist>
		<Channel></Channel>//Reserved，内容可以不填；
	</Content>
	************************************************************************/
	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Fmtvalue", srcValue, elemValue, uiMaxLen);
	param.fmtvalue = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DispatchNum", srcValue, elemValue, uiMaxLen);
	param.dispatch_num = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("Dstviewerlist"))
	{
		LOG_RUN_ERROR("FindElem Dstviewerlist failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	std::list<int>::iterator it = param.dstviewerlist.begin();
	do 
	{																		
		srcValue = xmlParse.GetElemValue();
		if(NULL != srcValue)
		{
			if(eLTE_SVC_ERR_SUCCESS != eLTE_Tool::IsDigit(srcValue))
			{
				LOG_RUN_ERROR("Parameter %s is not all digital.", srcValue);
				return eLTE_SVC_ERR_INVALID_PARAM;
			}
			param.dstviewerlist.push_back(eLTE_Tool::String2Int(srcValue));
		}
	} while (xmlParse.NextElem());
	xmlParse.OutOfElem();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Channel", srcValue, elemValue, uiMaxLen);
	param.channel = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;

}

int XMLProcess::GetXml_StartRealPlay_Req(const char* xmlStr, int& resId, VIDEO_PARAM& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<VideoParam>
			<VideoFormat></VideoFormat>//D1、CIF、QCIF、720P或1080P
			<CameraType></CameraType>//“0”表示前置，“1”表示后置。
			<UserConfirmType></UserConfirmType>//“0”表示不需要用户确认，“1”表示需要用户确认。
			<MuteType></MuteType>//“0”表示需要伴音；“1”表示不需要伴音
		</VideoParam>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("VideoParam"))
	{
		LOG_RUN_ERROR("FindElem VideoParam failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "VideoFormat", srcValue, elemValue, uiMaxLen);
	param.VideoFormat = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CameraType", srcValue, elemValue, uiMaxLen);
	param.CameraType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserConfirmType", srcValue, elemValue, uiMaxLen);
	param.UserConfirmType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "MuteType", srcValue, elemValue, uiMaxLen);
	param.MuteType = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_PTZControl_Req(const char* xmlStr, int& resId, FVG_control& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>带云台的摄像机资源ID</ResourceID>
		<PTZControlCode>云台控制命令</PTZControlCode>
		<PTZControlValue>云台控制命令对应的参数 </PTZControlValue> 
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "PTZControlCode", srcValue, elemValue, uiMaxLen);
	param.ptz_control_code = eLTE_Tool::String2UInt(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "PTZControlValue", srcValue, elemValue, uiMaxLen);
	param.ptz_control_value = eLTE_Tool::String2UInt(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_StopRealPlay_Req(const char* xmlStr, int& resId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetGisSubscription_Rsp(const GisQuerySubList* pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<GisQuerySubList>
			<GisQuerySubscription>
				<UeID></UeID>
				<UserName></UserName>
			</GisQuerySubscription>
		</GisQuerySubList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GisQuerySubList");
	(void)xml.IntoElem();

	GisQuerySubList::const_iterator it_b = pInfo->begin();
	GisQuerySubList::const_iterator it_e = pInfo->end();

	unsigned int t=pInfo->size();

	//LOG_RUN_INFO((IntToStr()));
	LOG_RUN_INFO("list:%d.",t);

	for (GisQuerySubList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("GisQuerySubscription");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("GisQuerySubscription");
		}

		GisQuerySub* const &pGisQuerySub = *itor;
		if (NULL == pGisQuerySub)
		{
			LOG_RUN_ERROR("pGisQuerySub is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		
		(void)xml.AddChildElem("UeID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(pGisQuerySub->UserId.c_str());
		(void)xml.AddElem("UserName");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(pGisQuerySub->UserName.c_str());
		
		xml.OutOfElem();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::Common_GetResourceID(const char* xmlStr, int& resId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::Common_GetGroupID(const char* xmlStr, int& groupId)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID>群组ID</GroupID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "GroupID", srcValue, elemValue, uiMaxLen);
	groupId = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_SetUserSpecificGISCfg_Req(const char* xmlStr, unsigned& resId, unsigned& reportPeriod, unsigned& reportDistance)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<UeGisCfgInfo>
			<ResourceID>资源ID</ResourceID>
			<ReportPeriod>上报时间，以秒为单位。默认的选择是{0,1,2,5,10,15,30,60}</ReportPeriod	>
			<ReportDistance>上报距离，单位是米。取值范围大于等于0小于 10000</ReportDistance>
		</MuteParam>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);
	if (!xmlParse.FindElem("UeGisCfgInfo"))
	{
		LOG_RUN_ERROR("FindElem UeGisCfgInfo failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	const char* srcValue;	
	const unsigned int XML_VAR_LENGTH = 20;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	char elemValue[XML_VAR_LENGTH] = {0};	

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ReportPeriod", srcValue, elemValue, uiMaxLen);
	reportPeriod = eLTE_Tool::String2UInt(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ReportDistance", srcValue, elemValue, uiMaxLen);
	reportDistance = eLTE_Tool::String2UInt(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_VolControl_Req(const char* xmlStr, int& resId, int& callType)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<MuteParam>
			<ResourceID>资源ID</ResourceID>
			<CallType>"0"点呼，"1"组呼，"2"视频回传</CallType>
		</MuteParam>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	if (!xmlParse.FindElem("MuteParam"))
	{
		LOG_RUN_ERROR("FindElem MuteParam failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CallType", srcValue, elemValue, uiMaxLen);
	callType = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

//解析发短信/彩信参数
int XMLProcess::GetXml_SDSSend_Req(const std::string& xmlStr, std::string& strResId, SDS_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID></ResourceID>   //资源ID
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

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		//INFO_PARAM1(xmlStr);
		return eLTE_SVC_ERR_XML_PARSE;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	strResId = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "SDSType", srcValue, elemValue, uiMaxLen);
	
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
		return eLTE_SVC_ERR_XML_FIND_ELEM;											
	}																			
	srcValue = xmlParse.GetElemValue();												
	if (NULL == srcValue)														
	{																			
		LOG_RUN_ERROR("GetElemValue Receiver failed.");							
		return eLTE_SVC_ERR_XML_GET_ELEM_VALUE;										
	}			
	param.receiver = srcValue;

	//彩信
	if (EXMPP_MSG_TYPE_MMS == param.sdstype && xmlParse.FindElem("AttachFileList") && xmlParse.IntoElem())
	{
		

		do 
		{																		
			srcValue = xmlParse.GetElemValue();
			if(NULL != srcValue)
			{
				param.files.push_back(eLTE_Tool::ANSIToUTF8(srcValue));
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

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_GISSubscribe_Req(const std::string& xmlStr, int& subType, std::string& strResourceList, std::string& strSubscriber)
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
		return eLTE_SVC_ERR_INVALID_PARAM;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_SVC_ERR_XML_PARSE;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("GISParam"))
	{
		LOG_RUN_ERROR("FindElem GISParam failed.");
		return eLTE_SVC_ERR_XML_PARSE;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "SubType", srcValue, elemValue, uiMaxLen);
	subType = eLTE_Tool::String2Int(elemValue);

	if(xmlParse.FindElem("ResourceList"))
	{
		srcValue = xmlParse.GetElemValue();
		if(NULL != srcValue)
		{
			strResourceList = srcValue;
		}
	}

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Subscriber", srcValue, elemValue, uiMaxLen);
	strSubscriber = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetUserSpecificGISCfg_Rsp(const ProvGisCfg* pProvGisCfg, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<UeGisCfgInfo>
			<UeID></UeID>
			<ReportSwitch></ReportSwitch>
			<ReportPeriod></ReportPeriod>
			<ReportCapability></ReportCapability>
			<ReportDistance></ReportDistance>
		</UeGisCfgInfo>
	</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pProvGisCfg)
	{
		LOG_RUN_ERROR("pProvGisCfg is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UeGisCfgInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("UeID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pProvGisCfg->userid).c_str());
	(void)xml.AddElem("ReportSwitch");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pProvGisCfg->reportSwitch).c_str());
	(void)xml.AddElem("ReportPeriod");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pProvGisCfg->reportPeriod).c_str());
	(void)xml.AddElem("ReportCapability");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pProvGisCfg->reportCapability).c_str());
	(void)xml.AddElem("ReportDistance");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pProvGisCfg->reportDistance).c_str());

	xml.OutOfElem();
	xml.OutOfElem();

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;

}

int XMLProcess::SetXml_GetPatchGroupInfo_Rsp(const PatchGroupInfo* pPatchGroupInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<PatchGroupInfo>
			<GroupNumber></GroupNumber>
			<SetupDcId></SetupDcId>
			<PGPriority></PGPriority>
			<DcPatchIndex></DcPatchIndex>
			<PGName></PGName>
			<VPNID></VPNID>
		</PatchGroupInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pPatchGroupInfo)
	{
		LOG_RUN_ERROR("pPatchGroupInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("PatchGroupInfo");
	(void)xml.IntoElem();
	SET_GROUPINFO_XML();
	xml.OutOfElem();

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetPatchGroups_Rsp(const PatchGroupsList* pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<PatchGroupInfoList>
			<PatchGroupInfo>
				<GroupNumber></GroupNumber>
				<SetupDcId></SetupDcId>
				<PGPriority></PGPriority>
				<DcPatchIndex></DcPatchIndex>
				<PGName></PGName>
				<VPNID></VPNID>
			</PatchGroupInfo>
		</PatchGroupInfoList>
	</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("PatchGroupInfoList");
	(void)xml.IntoElem();

	PatchGroupsList::const_iterator it_b = pInfo->begin();
	PatchGroupsList::const_iterator it_e = pInfo->end();
	for (PatchGroupsList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("PatchGroupInfo");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("PatchGroupInfo");
		}

		PatchGroupInfo* const &pPatchGroupInfo = *itor;
		if (NULL == pPatchGroupInfo)
		{
			LOG_RUN_ERROR("pPatchGroupInfo is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		SET_GROUPINFO_XML();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;

}

int XMLProcess::SetXml_GetGroupMemberByPatchId_Rsp(const PatchGroupMembers* pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<PatchGroupMemberList>
			<PatchGroupMember>
				<GroupNumber></GroupNumber>
				<MemberGroupId></MemberGroupId>
			</PatchGroupMember>
		</PatchGroupMemberList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("PatchGroupMemberList");
	(void)xml.IntoElem();

	PatchGroupMembers::const_iterator it_b = pInfo->begin();
	PatchGroupMembers::const_iterator it_e = pInfo->end();
	for (PatchGroupMembers::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("PatchGroupMember");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("PatchGroupMember");
		}

		PatchGroupMemberInfo* const &pPatchGroupMemberInfo = *itor;
		if (NULL == pPatchGroupMemberInfo)
		{
			LOG_RUN_ERROR("pPatchGroupMemberInfo is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		(void)xml.AddChildElem("GroupNumber");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupMemberInfo->GroupNumber).c_str());
		(void)xml.AddElem("MemberGroupId");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pPatchGroupMemberInfo->MemberGroupId).c_str());

		xml.OutOfElem();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetDcGroups_Rsp(const DcGroups* pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupInfoList");
	(void)xml.IntoElem();

	DcGroups::const_iterator it_b = pInfo->begin();
	DcGroups::const_iterator it_e = pInfo->end();
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
		if (NULL == pGroupInfo)
		{
			LOG_RUN_ERROR("pGroupInfo is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		(void)xml.AddChildElem("GroupID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grpid).c_str());
		(void)xml.AddElem("GroupCategory");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grpcategory).c_str());
		(void)xml.AddElem("GroupPriority");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pGroupInfo->grppriority).c_str());
		(void)xml.AddElem("GroupName");
		(void)xml.SetElemValue(pGroupInfo->grpname.c_str());

		xml.OutOfElem();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetDcUsers_Rsp(const DcUsers* pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserInfoList");
	(void)xml.IntoElem();

	DcUsers::const_iterator it_b = pInfo->begin();
	DcUsers::const_iterator it_e = pInfo->end();
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
		if (NULL == pUserInfo)
		{
			LOG_RUN_ERROR("pUserInfo is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		(void)xml.AddChildElem("UserID");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->userid).c_str());
		(void)xml.AddElem("UserCategory");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->usercategory).c_str());
		(void)xml.AddElem("UserPriority");
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pUserInfo->userpriority).c_str());
		(void)xml.AddElem("UserName");
		(void)xml.SetElemValue(pUserInfo->username.c_str());

		xml.OutOfElem();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetGroupUsers_Rsp(const GrpUserList* pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupUserInfoList");
	(void)xml.IntoElem();

	GrpUserList::const_iterator it_b = pInfo->begin();
	GrpUserList::const_iterator it_e = pInfo->end();
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
		if (NULL == grpUserInfo)
		{
			LOG_RUN_ERROR("grpUserInfo is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

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
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetGroupInfo_Rsp(const GroupInfo* pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupInfo>
			<GroupID></GroupID>
			<GroupCategory></GroupCategory>
			<GroupPriority></GroupPriority>
			<GroupName></GroupName>
			<GroupCreator></GroupCreator> //creator, valid if it's dynamic, current
		</GroupInfo>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("GroupID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->grpid).c_str());
	(void)xml.AddElem("GroupCategory");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->grpcategory).c_str());
	(void)xml.AddElem("GroupPriority");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->grppriority).c_str());
	(void)xml.AddElem("GroupName");
	(void)xml.SetElemValue(pInfo->grpname.c_str());
	(void)xml.AddElem("GroupCreator");
	if(GRP_DYNAMICGRP == pInfo->grpcategory)
	{
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->setupDcId).c_str());
	}
	else
	{
		(void)xml.SetElemValue("");
	}
	xml.OutOfElem();
	xml.OutOfElem();

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetUserInfo_Rsp(const UserInfo* pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("UserInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("UserID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->userid).c_str());
	(void)xml.AddElem("UserCategory");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->usercategory).c_str());
	(void)xml.AddElem("UserPriority");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->userpriority).c_str());
	(void)xml.AddElem("UserName");
	(void)xml.SetElemValue(pInfo->username.c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetDcInfo_Rsp(const DcProperty* pInfo, std::string& xmlStr)
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

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("DcInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("DcID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->dcid).c_str());
	(void)xml.AddElem("Priviledge");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->priviledge).c_str());
	(void)xml.AddElem("Role");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->role).c_str());
	(void)xml.AddElem("Alias");
	(void)xml.SetElemValue(pInfo->Alias.c_str());
	xml.OutOfElem();
	xml.OutOfElem();

	GET_XML_STR(eLTE_SVC_ERR_XML_CONSTRUCT);
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetUserRECFileInfoList_Rsp(const UserRecFileInfoList* pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
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
		</RECFileInfoList> 
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("RECFileInfoList");
	(void)xml.IntoElem();

	UserRecFileInfoList::const_iterator it_b = pInfo->begin();
	UserRecFileInfoList::const_iterator it_e = pInfo->end();
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
		(void)xml.SetElemValue(recFileInfo->urlFTP.c_str());
		(void)xml.AddElem("UrlRTSP");
		(void)xml.SetElemValue(recFileInfo->urlRTSP.c_str());

		xml.OutOfElem();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	//INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetTempGroupID_Rsp(const int groupId, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<GroupID>资源ID</GroupID>
	</Content>
	************************************************************************/

	LOG_TRACE();

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("GroupID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(eLTE_Tool::Int2String(groupId).c_str());
	xml.OutOfElem();

	unsigned int uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_SVC_ERR_XML_CONSTRUCT;
	}

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_StartVWall_Req(const char* xmlStr, int& resId, StartVWALL_PARAM& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<VideoParam>
			<DstObjId ></DstObjId >//视频上墙的目的地，为一个视频通道
			<StrFmt ></StrFmt >//预留,2.0版本不使用
		</VideoParam>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("VideoParam"))
	{
		LOG_RUN_ERROR("FindElem VideoParam failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DstObjId", srcValue, elemValue, uiMaxLen);
	param.DstObjId = eLTE_Tool::String2Int(elemValue);

	param.SrcObjId = resId;

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "StrFmt", srcValue, elemValue, uiMaxLen);
	param.StrFmt = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::SetXml_GetDcVWallIDList_Rsp(const VWallInfoList* pInfo, std::string& xmlStr)
{
	/************************************************************************
	--- XML 格式 ---
	<Content> 
		<VWallIDList>
			<VWallID>
				<DstObjId></DstObjId>
				<IDState></IDState>
				<Alias></Alias>
			</VWallID>
		</VWallIDList>
	</Content>
	************************************************************************/

	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("VWallIDList");
	(void)xml.IntoElem();

	VWallInfoList::const_iterator it_b = pInfo->begin();
	VWallInfoList::const_iterator it_e = pInfo->end();

	unsigned int t=pInfo->size();

	LOG_RUN_INFO("list:%d.",t);

	for (VWallInfoList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		if (it_b == itor)
		{
			(void)xml.AddChildElem("VWallID");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("VWallID");
		}

		VWallInfo* const &pVWallIDList = *itor;
		if (NULL == pVWallIDList)
		{
			LOG_RUN_ERROR("pVWallIDList is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}
		
		(void)xml.AddChildElem("DstObjId");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pVWallIDList->userid).c_str());
		(void)xml.AddElem("IDState");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(eLTE_Tool::Int2String(pVWallIDList->Status).c_str());
		(void)xml.AddElem("Alias");
		(void)xml.IntoElem();
		(void)xml.SetElemValue((pVWallIDList->Alias + eLTE_Tool::Int2String(pVWallIDList->ChnNo)).c_str());
		
		xml.OutOfElem();
	}
	IS_POINTER_EMPTY_AND_GETXMLSTREAM(pInfo,xml);

	xmlStr = pXmlStr;
	INFO_PARAM1(xmlStr);
	return eLTE_SVC_ERR_SUCCESS;
}


int XMLProcess::GetXml_VWallStop_Req(const char* xmlStr, int& resId, VWallStop_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<DstObjId>视频上墙的目的地，为一个视频通道</DstObjId>
	</Content>
	************************************************************************/
	PARSE_XML_DATA(xmlStr);

	LOG_RUN_INFO("XMLProcess::GetXml_VWallStop_Req xmlStr:%s",xmlStr);	

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DstObjId", srcValue, elemValue, uiMaxLen);
	param.DstObjId = eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	param.SrcObjId = eLTE_Tool::String2Int(elemValue);
	resId = param.SrcObjId;
	
	return eLTE_SVC_ERR_SUCCESS;
}

// int XMLProcess::GetXml_TelephoneDial_Req(const char* xmlStr, std::string& PhoneNumStr)
// {
// 	/************************************************************************
// 	--- XML 格式 ---
// 	<Content>
// 		<PhoneNum>PSTN/PLMN电话拨号</PhoneNum>		
// 	</Content>
// 	************************************************************************/
// 	PARSE_XML_DATA(xmlStr);
// 	LOG_RUN_INFO("XMLProcess::GetXml_TelephoneDial_Req xmlStr:%s",xmlStr);	
// 
// 	const unsigned int XML_VAR_LENGTH = 20;
// 	char elemValue[XML_VAR_LENGTH] = {0};
// 	const char* srcValue;
// 	unsigned int uiMaxLen = XML_VAR_LENGTH;
// 	
// 	GET_XML_ELEM_VALUE_CHAR(xmlParse, "TelNumber", srcValue, elemValue, uiMaxLen);
// 	PhoneNumStr = elemValue;
// 
// 	return eLTE_SVC_ERR_SUCCESS;
// }

// int XMLProcess::GetXml_TelephoneHangup_Req(const char* xmlStr, std::string& PhoneNumStr)
// {
// 	/************************************************************************
// 	--- XML 格式 ---
// 	<Content>
// 		<PhoneNum>PSTN/PLMN电话拨号</PhoneNum>		
// 	</Content>
// 	************************************************************************/
// 	PARSE_XML_DATA(xmlStr);
// 	LOG_RUN_INFO("XMLProcess::GetXml_TelephoneHangup_Req xmlStr:%s",xmlStr);	
// 
// 	const unsigned int XML_VAR_LENGTH = 20;
// 	char elemValue[XML_VAR_LENGTH] = {0};
// 	const char* srcValue;
// 	unsigned int uiMaxLen = XML_VAR_LENGTH;
// 	
// 	GET_XML_ELEM_VALUE_CHAR(xmlParse, "TelNumber", srcValue, elemValue, uiMaxLen);
// 	PhoneNumStr = elemValue;
// 
// 	return eLTE_SVC_ERR_SUCCESS;
// }

int XMLProcess::GetXml_GetResourceID_Req(const char* xmlStr, std::string& ResourceID)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源对象，可以是群组或用户</ResourceID>		
	</Content>
	************************************************************************/
	PARSE_XML_DATA(xmlStr);

	LOG_RUN_INFO("XMLProcess::GetXml_StartDiscreetListen_Req xmlStr:%s",xmlStr);	

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	ResourceID = elemValue;

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_P2PTransfer_Req(const char* xmlStr,int& resId,transfer_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>		
		<DcID>表示发起人工转接的调度员的用户ID</DcID>
		<SpeakerID>当前正在通话中的对端的ID</SpeakerID>
		<ObjectID>需要转接的号码ID</ObjectID>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	LOG_RUN_INFO("XMLProcess::GetXml_P2PTransfer_Req xmlStr:%s",xmlStr);	

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "DcID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);
	param.dcid =eLTE_Tool::String2Int(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "SpeakerID", srcValue, elemValue, uiMaxLen);
	m_speakerID = elemValue;
	param.speakerid = m_speakerID.c_str();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ObjectID", srcValue, elemValue, uiMaxLen);
	m_transferNum = elemValue;
	param.objectid = m_transferNum.c_str();

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_StartRecord_Req(const char* xmlStr, int& resId, Record_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<RecordParam>
		<tasktype ></tasktype >//0表示录音 1表示录像
		<calltype ></calltype >//0表示点呼 1表示组呼 2视频回传
		</RecordParam>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("RecordParam"))
	{
		LOG_RUN_ERROR("FindElem RecordParam failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "tasktype", srcValue, elemValue, uiMaxLen);
	param.task_type = eLTE_Tool::String2Int(elemValue);

	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "calltype", srcValue, elemValue, uiMaxLen);
	param.call_type = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}

int XMLProcess::GetXml_StopRecord_Req(const char* xmlStr, int& resId, Record_parameter& param)
{
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResourceID>资源ID</ResourceID>
		<RecordParam>
		<tasktype ></tasktype >//0表示录音 1表示录像
		<calltype ></calltype >//0表示点呼 1表示组呼 2视频回传
		</RecordParam>
	</Content>
	************************************************************************/

	PARSE_XML_DATA(xmlStr);

	const unsigned int XML_VAR_LENGTH = 20;
	unsigned int uiMaxLen = XML_VAR_LENGTH;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "ResourceID", srcValue, elemValue, uiMaxLen);
	resId = eLTE_Tool::String2Int(elemValue);

	if (!xmlParse.FindElem("RecordParam"))
	{
		LOG_RUN_ERROR("FindElem RecordParam failed.");
		return eLTE_SVC_ERR_XML_FIND_ELEM;
	}
	(void)xmlParse.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "tasktype", srcValue, elemValue, uiMaxLen);
	param.task_type = eLTE_Tool::String2Int(elemValue);

	eSDK_MEMSET(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "calltype", srcValue, elemValue, uiMaxLen);
	param.call_type = eLTE_Tool::String2Int(elemValue);

	return eLTE_SVC_ERR_SUCCESS;
}